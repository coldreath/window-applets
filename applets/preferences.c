#include <glib/gstdio.h>
#include <gtk/gtk.h>

#include "applets/preferences.h"

#ifdef WIBUTI_WITH_BUTTONS
static void wibuti_prefs_set_theme(WibutiPrefs *, WibutiConfig *);
static void wibuti_prefs_reload_buttons(WibutiPrefs *, GdkPixbuf ****, gboolean);
#endif // WIBUTI_WITH_BUTTONS

// callbacks
static gboolean wibuti_prefs_delete_cb(GtkWidget *, GdkEvent *, WibutiPrefs *);
static void wibuti_prefs_close_cb(GtkWidget *, WibutiPrefs *);


G_DEFINE_TYPE(WibutiPrefs, wibuti_prefs, GTK_TYPE_WINDOW);

static void wibuti_prefs_init(WibutiPrefs *self) {
	GError *err = NULL;
	char *markup = "";
	GtkBox *main_box = GTK_BOX(gtk_vbox_new(FALSE, 5));
	GtkNotebook *notebook = GTK_NOTEBOOK(gtk_notebook_new());
	GtkButton *btn_close = GTK_BUTTON(gtk_button_new_from_stock(GTK_STOCK_CLOSE));
	GtkButtonBox *btnbox = GTK_BUTTON_BOX(gtk_hbutton_box_new());
	GtkLabel *label_layout_tip = GTK_LABEL(gtk_label_new(""));
	self->builder = gtk_builder_new();

#ifdef WIBUTI_WITH_BUTTONS
	GtkAlignment *alignment_theme, *alignment_buttons;
	GtkLabel *label_theme, *label_buttons;

	gtk_builder_add_from_file(self->builder, WIBUTI_PATH_BUTTONS_UI, &err);

	if (err != NULL) {
		g_fprintf(stderr, "%s\n", err->message);
		g_error_free(err);
	} else {
		alignment_theme = GTK_ALIGNMENT(gtk_builder_get_object(self->builder, "alignment_theme"));
		alignment_buttons = GTK_ALIGNMENT(gtk_builder_get_object(self->builder, "alignment_buttons"));

		self->combo_theme = GTK_COMBO_BOX(gtk_builder_get_object(self->builder, CFG_THEME));
		self->chkb_hover_effect = GTK_TOGGLE_BUTTON(gtk_builder_get_object(self->builder, CFG_HOVER_EFFECT));
		self->chkb_click_effect = GTK_TOGGLE_BUTTON(gtk_builder_get_object(self->builder, CFG_CLICK_EFFECT));

		// fill themes combo
		GtkListStore *list_store = gtk_list_store_new(2, G_TYPE_STRING, G_TYPE_INT);
		GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
		gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(self->combo_theme), renderer, TRUE);
		gtk_combo_box_set_model(self->combo_theme, GTK_TREE_MODEL(list_store));

		const gchar *subdir;
		GDir *dir = g_dir_open(WIBUTI_PATH_THEMES, 0, NULL);
		while ((subdir = g_dir_read_name(dir)) != NULL) {
			GtkTreeIter iter;
			gtk_list_store_append(list_store, &iter);
			gtk_list_store_set(list_store, &iter, 0, subdir, 1, 0, -1);
		}
		g_dir_close(dir);

		// pack to notebook
		label_theme = GTK_LABEL(gtk_label_new("Themes"));
		gtk_notebook_append_page(notebook, GTK_WIDGET(alignment_theme), GTK_WIDGET(label_theme));
		label_buttons = GTK_LABEL(gtk_label_new("Buttons"));
		gtk_notebook_append_page(notebook, GTK_WIDGET(alignment_buttons), GTK_WIDGET(label_buttons));
		
		markup = g_strconcat(markup, "<b>M</b> - ", "minimize", "\n<b>R</b> - ", "maximize/restore", 
		                             "\n<b>X</b> - ", "close", "\n", NULL);
	}
#endif // WIBUTI_WITH_BUTTONS

#ifdef WIBUTI_WITH_TITLE
	GtkAlignment *alignment_title;
	GtkLabel *label_title;
	gtk_builder_add_from_file(self->builder, WIBUTI_PATH_TITLE_UI, &err);

	if (err != NULL) {
		g_fprintf(stderr, "%s\n", err->message);
		g_error_free(err);
	} else {
		alignment_title = GTK_ALIGNMENT(gtk_builder_get_object(self->builder, "alignment_title"));
		
		self->chkb_expand_title = GTK_TOGGLE_BUTTON(gtk_builder_get_object(self->builder, CFG_EXPAND_TITLE));
		self->chkb_custom_style = GTK_TOGGLE_BUTTON(gtk_builder_get_object(self->builder, CFG_CUSTOM_STYLE));
		
		self->btn_font_active = GTK_FONT_BUTTON(gtk_builder_get_object(self->builder, "btn_font_active"));
		self->btn_font_inactive = GTK_FONT_BUTTON(gtk_builder_get_object(self->builder, "btn_font_inactive"));
		self->btn_color_active = GTK_COLOR_BUTTON(gtk_builder_get_object(self->builder, "btn_color_active"));
		self->btn_color_inactive = GTK_COLOR_BUTTON(gtk_builder_get_object(self->builder, "btn_color_inactive"));

		self->scale_alignment = GTK_SCALE(gtk_builder_get_object(self->builder, "scale_alignment"));

		// pack to notebook
		label_title = GTK_LABEL(gtk_label_new("Title"));
		gtk_notebook_append_page(notebook, GTK_WIDGET(alignment_title), GTK_WIDGET(label_title));
		
		markup = g_strconcat(markup, "<b>I</b> - ", "icon", "\n<b>T</b> - ", "title", "\n", NULL);
	}
#endif // WIBUTI_WITH_TITLE
	
	GtkAlignment *alignment_general;
	GtkLabel *label_general;
	gtk_builder_add_from_file(self->builder, WIBUTI_PATH_GENERAL_UI, &err);

	if (err != NULL) {
		g_fprintf(stderr, "%s\n", err->message);
		g_error_free(err);
	} else {
		alignment_general = GTK_ALIGNMENT(gtk_builder_get_object(self->builder, "alignment_general"));
		label_layout_tip = GTK_LABEL(gtk_builder_get_object(self->builder, "label_layout_tip"));

		self->chkb_only_maximized = GTK_TOGGLE_BUTTON(gtk_builder_get_object(self->builder, CFG_ONLY_MAXIMIZED));
		self->chkb_hide_unmaximized = GTK_TOGGLE_BUTTON(gtk_builder_get_object(self->builder, CFG_HIDE_ON_UNMAXIMIZED));
		self->btn_reload_layout = GTK_BUTTON(gtk_builder_get_object(self->builder, "btn_reload_layout"));
		self->entry_layout = GTK_ENTRY(gtk_builder_get_object(self->builder, "entry_layout"));
		
		// pack to notebook
		label_general = GTK_LABEL(gtk_label_new("General"));
		gtk_notebook_append_page(notebook, GTK_WIDGET(alignment_general), GTK_WIDGET(label_general));
	}

	// gui appearance
	gtk_notebook_set_current_page(notebook, 0);
	gtk_button_box_set_layout(btnbox, GTK_BUTTONBOX_END);
	gtk_label_set_markup(label_layout_tip, markup);
	g_free(markup);

	g_object_set(GTK_WINDOW(self), "window-position", GTK_WIN_POS_CENTER_ALWAYS, NULL);
	g_object_set(GTK_WINDOW(self), "border-width", 3, NULL);

	// pack gui
	gtk_box_pack_start(GTK_BOX(btnbox), GTK_WIDGET(btn_close), FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(main_box), GTK_WIDGET(notebook), TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(main_box), GTK_WIDGET(btnbox), FALSE, TRUE, 0);
	gtk_container_add(GTK_CONTAINER(self), GTK_WIDGET(main_box));

	// connect signals
	g_signal_connect(btn_close, "clicked", G_CALLBACK(wibuti_prefs_close_cb), self);
	g_signal_connect(self, "delete-event", G_CALLBACK(wibuti_prefs_delete_cb), self);
}

static void wibuti_prefs_class_init(WibutiPrefsClass *klass) {
}

WibutiPrefs* wibuti_prefs_new(void) {
	return WIBUTI_PREFS(g_object_new(WIBUTI_TYPE_PREFS, NULL));
}


/**********************************************************************************************************************/


void wibuti_prefs_set_from_config(WibutiPrefs *self, WibutiConfig *config) {
	gtk_toggle_button_set_active(self->chkb_only_maximized, config->only_maximized);
	gtk_toggle_button_set_active(self->chkb_hide_unmaximized, config->hide_on_unmaximized);
	gtk_entry_set_text(self->entry_layout, config->layout);

#ifdef WIBUTI_WITH_BUTTONS
	gtk_toggle_button_set_active(self->chkb_click_effect, config->click_effect);
	gtk_toggle_button_set_active(self->chkb_hover_effect, config->hover_effect);
	wibuti_prefs_set_theme(self, config);
#endif // WIBUTI_WITH_BUTTONS

#ifdef WIBUTI_WITH_TITLE
	gtk_toggle_button_set_active(self->chkb_expand_title, config->expand_title);
	gtk_toggle_button_set_active(self->chkb_custom_style, config->custom_style);
	
	GdkColor color;
	gdk_color_parse(config->title_active_color, &color);
	gtk_color_button_set_color(self->btn_color_active, &color);
	gdk_color_parse(config->title_inactive_color, &color);
	gtk_color_button_set_color(self->btn_color_inactive, &color);
	gtk_font_button_set_font_name(self->btn_font_active, config->title_active_font);
	gtk_font_button_set_font_name(self->btn_font_inactive, config->title_inactive_font);
	
	gtk_range_set_value(GTK_RANGE(self->scale_alignment), config->alignment);
#endif // WIBUTI_WITH_TITLE
}

#ifdef WIBUTI_WITH_BUTTONS

static void wibuti_prefs_set_theme(WibutiPrefs *self, WibutiConfig *config) {
	gchar *name;
	int local;
	GtkListStore *list_store = GTK_LIST_STORE(gtk_combo_box_get_model(self->combo_theme));
	GtkTreeIter *iter = g_new(GtkTreeIter, 1);
	if (gtk_tree_model_get_iter_first(GTK_TREE_MODEL(list_store), iter)) {
		do {
			gtk_tree_model_get(GTK_TREE_MODEL(list_store), iter, 0, &name, 1, &local, -1);
			if (g_strcmp0(config->theme, name) == 0) {
				gtk_combo_box_set_active_iter(self->combo_theme, iter);
				wibuti_prefs_reload_buttons(self, wibuti_config_get_buttons(config), local);
				g_free(name);
				return;				
			}
		} while (gtk_tree_model_iter_next(GTK_TREE_MODEL(list_store), iter));
	} else {
		// TODO: show error
	}
}

static void wibuti_prefs_reload_buttons(WibutiPrefs *self, GdkPixbuf ****buttons, gboolean local) {

}

#endif // WIBUTI_WITH_BUTTONS


/**********************************************************************************************************************/


static gboolean wibuti_prefs_delete_cb(GtkWidget *widget, GdkEvent *event, WibutiPrefs *self) {
	gtk_widget_hide(GTK_WIDGET(self));
	return TRUE;
}

static void wibuti_prefs_close_cb(GtkWidget *widget, WibutiPrefs *self) {
	gtk_widget_hide(GTK_WIDGET(self));
}


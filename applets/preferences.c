#include <glib/gstdio.h>
#include <gtk/gtk.h>

#include "applets/preferences.h"

#define PATH_MAIN			"/usr/share/windowapplets"
#define PATH_TITLE_UI		PATH_MAIN"/title-preferences.ui"
#define PATH_BUTTONS_UI		PATH_MAIN"/buttons-preferences.ui"

// callbacks
static gboolean wibuti_prefs_delete_cb(GtkWidget *widget, GdkEvent *event, WibutiPrefs *self);
static void wibuti_prefs_close_cb(GtkWidget *widget, WibutiPrefs *self);


G_DEFINE_TYPE(WibutiPrefs, wibuti_prefs, GTK_TYPE_WINDOW);

static void wibuti_prefs_init(WibutiPrefs *self) {
	GError *err = NULL;
	self->builder = gtk_builder_new();
#ifdef WIBUTI_WITH_BUTTONS
	gtk_builder_add_from_file(self->builder, PATH_BUTTONS_UI, &err);
#endif // WIBUTI_WITH_BUTTONS
#ifdef WIBUTI_WITH_TITLE
	gtk_builder_add_from_file(self->builder, PATH_TITLE_UI, &err);
#endif // WIBUTI_WITH_TITLE
	
	if (err != NULL) {
		g_fprintf(stderr, "%s\n", err->message);
		g_error_free(err);
	} else {
#ifdef WIBUTI_WITH_BUTTONS

#endif // WIBUTI_WITH_BUTTONS
#ifdef WIBUTI_WITH_TITLE
		self->main_box = GTK_BOX(gtk_builder_get_object(self->builder, "main_box"));
		self->btn_close = GTK_BUTTON(gtk_builder_get_object(self->builder, "btn_close"));
		
		self->chkb_only_maximized = GTK_TOGGLE_BUTTON(gtk_builder_get_object(self->builder, CFG_ONLY_MAXIMIZED));
		self->chkb_swap_order = GTK_TOGGLE_BUTTON(gtk_builder_get_object(self->builder, CFG_SWAP_ORDER));
		self->chkb_expand_title = GTK_TOGGLE_BUTTON(gtk_builder_get_object(self->builder, CFG_EXPAND_TITLE));
		self->chkb_hide_title = GTK_TOGGLE_BUTTON(gtk_builder_get_object(self->builder, CFG_HIDE_TITLE));
		self->chkb_hide_icon = GTK_TOGGLE_BUTTON(gtk_builder_get_object(self->builder, CFG_HIDE_ICON));
		self->chkb_custom_style = GTK_TOGGLE_BUTTON(gtk_builder_get_object(self->builder, CFG_CUSTOM_STYLE));
		
		self->btn_font_active = GTK_FONT_BUTTON(gtk_builder_get_object(self->builder, "btn_font_active"));
		self->btn_font_inactive = GTK_FONT_BUTTON(gtk_builder_get_object(self->builder, "btn_font_inactive"));
		self->btn_color_active = GTK_COLOR_BUTTON(gtk_builder_get_object(self->builder, "btn_color_active"));
		self->btn_color_inactive = GTK_COLOR_BUTTON(gtk_builder_get_object(self->builder, "btn_color_inactive"));

		self->scale_alignment = GTK_SCALE(gtk_builder_get_object(self->builder, "scale_alignment"));
#endif // WIBUTI_WITH_TITLE
		
		gtk_container_add(GTK_CONTAINER(self), GTK_WIDGET(self->main_box));

		g_signal_connect(self->btn_close, "clicked", G_CALLBACK(wibuti_prefs_close_cb), self);
	}
	
	g_signal_connect(self, "delete-event", G_CALLBACK(wibuti_prefs_delete_cb), self);

	g_object_set(GTK_WINDOW(self), "window-position", GTK_WIN_POS_CENTER_ALWAYS, NULL);
	g_object_set(GTK_WINDOW(self), "border-width", 3, NULL);
	g_object_set(GTK_WINDOW(self), "title", "Window Applets Preferences", NULL);
}

static void wibuti_prefs_class_init(WibutiPrefsClass *klass) {
}

WibutiPrefs* wibuti_prefs_new(void) {
	return WIBUTI_PREFS(g_object_new(WIBUTI_TYPE_PREFS, NULL));
}


/**********************************************************************************************************************/


void wibuti_prefs_set_from_config(WibutiPrefs *self, WibutiConfig *config) {
#ifdef WIBUTI_WITH_BUTTONS

#endif // WIBUTI_WITH_BUTTONS
#ifdef WIBUTI_WITH_TITLE
	gtk_toggle_button_set_active(self->chkb_only_maximized, config->only_maximized);
	gtk_toggle_button_set_active(self->chkb_expand_title, config->expand_title);
	gtk_toggle_button_set_active(self->chkb_swap_order, config->swap_order);
	gtk_toggle_button_set_active(self->chkb_hide_title, config->hide_title);
	gtk_toggle_button_set_active(self->chkb_hide_icon, config->hide_icon);
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


/**********************************************************************************************************************/


static gboolean wibuti_prefs_delete_cb(GtkWidget *widget, GdkEvent *event, WibutiPrefs *self) {
	gtk_widget_hide(GTK_WIDGET(self));
	return TRUE;
}

static void wibuti_prefs_close_cb(GtkWidget *widget, WibutiPrefs *self) {
	gtk_widget_hide(GTK_WIDGET(self));
}


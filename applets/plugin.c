#include "applets/plugin.h"

#define ICON_WIDTH	16
#define ICON_HEIGHT	16

static void wibuti_plugin_init(WibutiPlugin *);

// calbacks
static void wibuti_plugin_redraw_cb(WibutiWatcher *, WibutiPlugin *);
static void wibuti_plugin_maximized_toggled_cb(GtkToggleButton *, WibutiPlugin *);
static void wibuti_plugin_hide_unmaximized_toggled_cb(GtkToggleButton *, WibutiPlugin *);
static void wibuti_plugin_reload_layout_cb(GtkButton *, WibutiPlugin *);

#ifdef WIBUTI_WITH_BUTTONS
static void wibuti_plugin_minimize_cb(WibutiWidget *, WibutiPlugin *);
static void wibuti_plugin_maximize_restore_cb(WibutiWidget *, WibutiPlugin *);
static void wibuti_plugin_maximize_restore_cb(WibutiWidget *, WibutiPlugin *);
static void wibuti_plugin_close_cb(WibutiWidget *, WibutiPlugin *);

static void wibuti_plugin_click_effect_toggled_cb(GtkToggleButton *, WibutiPlugin *);
static void wibuti_plugin_hover_effect_toggled_cb(GtkToggleButton *, WibutiPlugin *);
static void wibuti_plugin_theme_changed_cb(GtkComboBox *, WibutiPlugin *);
#endif // WIBUTI_WITH_BUTTONS

#ifdef WIBUTI_WITH_TITLE
static void wibuti_plugin_update_title_cb(WibutiWatcher *, WibutiPlugin *);
static void wibuti_plugin_update_icon_cb(WibutiWatcher *, WibutiPlugin *);

static void wibuti_plugin_alignment_changed_cb(GtkScale *, WibutiPlugin *);
static void wibuti_plugin_expand_title_toggled_cb(GtkToggleButton *, WibutiPlugin *);
static void wibuti_plugin_custom_style_toggled_cb(GtkToggleButton *, WibutiPlugin *);
static void wibuti_plugin_active_font_set_cb(GtkFontButton *, WibutiPlugin *);
static void wibuti_plugin_inactive_font_set_cb(GtkFontButton *, WibutiPlugin *);
static void wibuti_plugin_active_color_set_cb(GtkColorButton *, WibutiPlugin *);
static void wibuti_plugin_inactive_color_set_cb(GtkColorButton *, WibutiPlugin *);
#endif // WIBUTI_WITH_TITLE


/**********************************************************************************************************************/


GType wibuti_plugin_get_type(void) {
	static GType type = 0;
	if (type == 0) {
		static const GTypeInfo info = {
					sizeof(WibutiPluginClass),
					NULL,
					NULL,
					(GClassInitFunc) NULL,
					NULL,
					NULL,
					sizeof(WibutiPlugin),
					0,
					(GInstanceInitFunc) wibuti_plugin_init
				};
		type = g_type_register_static(G_TYPE_OBJECT, "WibutiPlugin", &info, 0);
	}
	return type;
}

static void wibuti_plugin_init(WibutiPlugin *self) {
	self->widget = wibuti_widget_new();
	self->prefs = wibuti_prefs_new();
	self->watcher = wibuti_watcher_new();

	wibuti_config_load_defaults(&self->config);
	wibuti_watcher_set_only_maximized(self->watcher, self->config.only_maximized);
	wibuti_widget_repack_with_string(self->widget, self->config.layout);

	// connect to windows signals
	g_signal_connect(G_OBJECT(self->watcher), "window-changed",
	                 G_CALLBACK(wibuti_plugin_redraw_cb), self);
	                 
#ifdef WIBUTI_WITH_BUTTONS
	g_signal_connect(G_OBJECT(self->widget), "minimize-clicked",
	                 G_CALLBACK(wibuti_plugin_minimize_cb), self);
	g_signal_connect(G_OBJECT(self->widget), "maximize-clicked",
	                 G_CALLBACK(wibuti_plugin_maximize_restore_cb), self);
	g_signal_connect(G_OBJECT(self->widget), "restore-clicked",
	                 G_CALLBACK(wibuti_plugin_maximize_restore_cb), self);
	g_signal_connect(G_OBJECT(self->widget), "close-clicked",
	                 G_CALLBACK(wibuti_plugin_close_cb), self);
#endif // WIBUTI_WITH_BUTTONS

#ifdef WIBUTI_WITH_TITLE
	g_signal_connect(G_OBJECT(self->watcher), "name-changed",
	                 G_CALLBACK(wibuti_plugin_update_title_cb), self);
	g_signal_connect(G_OBJECT(self->watcher), "icon-changed",
	                 G_CALLBACK(wibuti_plugin_update_icon_cb), self);
#endif // WIBUTI_WITH_TITLE
}

WibutiPlugin *wibuti_plugin_new() {
	return WIBUTI_PLUGIN(g_object_new(WIBUTI_TYPE_PLUGIN, NULL));
}


/**********************************************************************************************************************/


void wibuti_plugin_show_preferences(WibutiPlugin *self) {
	static gboolean connected = FALSE;
	if (!connected) {
		wibuti_prefs_set_from_config(self->prefs, &self->config);
		
		g_signal_connect(G_OBJECT(self->prefs->chkb_only_maximized), "toggled",
		                 G_CALLBACK(wibuti_plugin_maximized_toggled_cb), self);
		g_signal_connect(G_OBJECT(self->prefs->chkb_hide_unmaximized), "toggled",
		                 G_CALLBACK(wibuti_plugin_hide_unmaximized_toggled_cb), self);
		g_signal_connect(G_OBJECT(self->prefs->btn_reload_layout), "clicked",
		                 G_CALLBACK(wibuti_plugin_reload_layout_cb), self);

#ifdef WIBUTI_WITH_BUTTONS
		g_signal_connect(G_OBJECT(self->prefs->chkb_hover_effect), "toggled",
		                 G_CALLBACK(wibuti_plugin_hover_effect_toggled_cb), self);
		g_signal_connect(G_OBJECT(self->prefs->chkb_click_effect), "toggled",
		                 G_CALLBACK(wibuti_plugin_click_effect_toggled_cb), self);
		g_signal_connect(G_OBJECT(self->prefs->combo_theme), "changed",
		                 G_CALLBACK(wibuti_plugin_theme_changed_cb), self);
#endif // WIBUTI_WITH_BUTTONS

#ifdef WIBUTI_WITH_TITLE
		g_signal_connect(G_OBJECT(self->prefs->chkb_expand_title), "toggled",
		                 G_CALLBACK(wibuti_plugin_expand_title_toggled_cb), self);
		g_signal_connect(G_OBJECT(self->prefs->chkb_custom_style), "toggled",
		                 G_CALLBACK(wibuti_plugin_custom_style_toggled_cb), self);
	
		g_signal_connect(G_OBJECT(self->prefs->btn_font_active), "font-set",
		                G_CALLBACK(wibuti_plugin_active_font_set_cb), self);
		g_signal_connect(G_OBJECT(self->prefs->btn_font_inactive), "font-set",
		                 G_CALLBACK(wibuti_plugin_inactive_font_set_cb), self);
		g_signal_connect(G_OBJECT(self->prefs->btn_color_active), "color-set",
		                 G_CALLBACK(wibuti_plugin_active_color_set_cb), self);
		g_signal_connect(G_OBJECT(self->prefs->btn_color_inactive), "color-set",
		                 G_CALLBACK(wibuti_plugin_inactive_color_set_cb), self);

		g_signal_connect(G_OBJECT(self->prefs->scale_alignment), "value-changed",
		                 G_CALLBACK(wibuti_plugin_alignment_changed_cb), self);	
#endif // WIBUTI_WITH_TITLE
		
		connected = TRUE;
	}

	gtk_widget_show_all(GTK_WIDGET(self->prefs));
}

void wibuti_plugin_update(WibutiPlugin *self) {
	wibuti_watcher_set_only_maximized(self->watcher, self->config.only_maximized);
	wibuti_widget_set_only_maximized(self->widget, self->config.only_maximized);
	wibuti_widget_set_is_active(self->widget, wibuti_watcher_is_active(self->watcher));

#ifdef WIBUTI_WITH_BUTTONS
	wibuti_widget_change_theme(self->widget, wibuti_config_get_buttons(&self->config));
	wibuti_widget_set_click_effect(self->widget, self->config.click_effect);
	wibuti_widget_set_hover_effect(self->widget, self->config.hover_effect);
	wibuti_plugin_redraw_buttons(self);
#endif // WIBUTI_WITH_BUTTONS

#ifdef WIBUTI_WITH_TITLE
	wibuti_widget_repack_with_string(self->widget, self->config.layout);
	wibuti_widget_set_alignment(self->widget, self->config.alignment);
	wibuti_widget_set_expand_title(self->widget, self->config.expand_title);
	wibuti_plugin_redraw_title(self);
	wibuti_plugin_redraw_icon(self);
#endif // WIBUTI_WITH_TITLE
}

void wibuti_plugin_redraw(WibutiPlugin *self) {
	if (self->config.hide_on_unmaximized && !wibuti_watcher_is_active(self->watcher)) {
		gtk_widget_hide(GTK_WIDGET(self->widget->box));
	} else {
		gtk_widget_show(GTK_WIDGET(self->widget->box));
	}

#ifdef WIBUTI_WITH_BUTTONS
	wibuti_plugin_redraw_buttons(self);
#endif // WIBUTI_WITH_BUTTONS

#ifdef WIBUTI_WITH_TITLE
	wibuti_plugin_redraw_title(self);
	wibuti_plugin_redraw_icon(self);
#endif // WIBUTI_WITH_TITLE
}

#ifdef WIBUTI_WITH_BUTTONS

void wibuti_plugin_redraw_buttons(WibutiPlugin *self) {
	wibuti_widget_set_maximized(self->widget, wibuti_watcher_is_maximized(self->watcher));
	wibuti_widget_redraw_buttons(self->widget);
}

#endif // WIBUTI_WITH_BUTTONS

#ifdef WIBUTI_WITH_TITLE

void wibuti_plugin_redraw_title(WibutiPlugin *self) {
	gboolean is_active = wibuti_watcher_is_active(self->watcher);
	if (self->config.custom_style) {
		wibuti_widget_set_markup_title(
					self->widget,
					wibuti_watcher_get_title(self->watcher),
					is_active ? self->config.title_active_font : self->config.title_inactive_font,
					is_active ? self->config.title_active_color : self->config.title_inactive_color
				);
	} else {
		wibuti_widget_set_title(self->widget, wibuti_watcher_get_title(self->watcher), is_active);
	}
}

void wibuti_plugin_redraw_icon(WibutiPlugin *self) {
	wibuti_widget_set_icon(self->widget, wibuti_watcher_get_icon(self->watcher));
}

#endif // WIBUTI_WITH_TITLE


/**********************************************************************************************************************/


static void wibuti_plugin_redraw_cb(WibutiWatcher *watcher, WibutiPlugin *self) {
	wibuti_plugin_redraw(self);
}

static void wibuti_plugin_maximized_toggled_cb(GtkToggleButton *btn, WibutiPlugin *self) {
	gboolean active = gtk_toggle_button_get_active(btn);
	self->config.only_maximized = active;
	wibuti_watcher_set_only_maximized(self->watcher, active);
	wibuti_plugin_redraw(self);
	wibuti_config_save_plain(&self->config);
}

static void wibuti_plugin_hide_unmaximized_toggled_cb(GtkToggleButton *btn, WibutiPlugin *self) {
	self->config.hide_on_unmaximized = gtk_toggle_button_get_active(btn);
	wibuti_plugin_redraw(self);
	wibuti_config_save_plain(&self->config);
}

static void wibuti_plugin_reload_layout_cb(GtkButton *btn, WibutiPlugin *self) {
	self->config.layout = gtk_entry_get_text(self->prefs->entry_layout);
	wibuti_widget_repack_with_string(self->widget, self->config.layout);
	wibuti_config_save_plain(&self->config);
}

#ifdef WIBUTI_WITH_BUTTONS

static void wibuti_plugin_minimize_cb(WibutiWidget *widget, WibutiPlugin *self) {
	wibuti_watcher_minimize(self->watcher);
}

static void wibuti_plugin_maximize_restore_cb(WibutiWidget *widget, WibutiPlugin *self) {
	wibuti_watcher_maximize_restore(self->watcher);
}

static void wibuti_plugin_close_cb(WibutiWidget *widget, WibutiPlugin *self) {
	wibuti_watcher_close(self->watcher);
}

static void wibuti_plugin_click_effect_toggled_cb(GtkToggleButton *btn, WibutiPlugin *self) {
	wibuti_widget_set_click_effect(self->widget, gtk_toggle_button_get_active(btn));
}

static void wibuti_plugin_hover_effect_toggled_cb(GtkToggleButton *btn, WibutiPlugin *self) {
	wibuti_widget_set_hover_effect(self->widget, gtk_toggle_button_get_active(btn));
}

static void wibuti_plugin_theme_changed_cb(GtkComboBox *combo, WibutiPlugin *self) {
	gint local;
	GtkTreeIter iter;
	GtkTreeModel *model = gtk_combo_box_get_model(combo); 
	gtk_combo_box_get_active_iter(combo, &iter);
	gtk_tree_model_get(model, &iter, 0, &self->config.theme, 1, &local, -1);
	wibuti_widget_change_theme(self->widget, wibuti_config_get_buttons(&self->config));
}

#endif // WIBUTI_WITH_BUTTONS

#ifdef WIBUTI_WITH_TITLE

static void wibuti_plugin_update_title_cb(WibutiWatcher *watcher, WibutiPlugin *self) {
	wibuti_plugin_redraw_title(self);
}

static void wibuti_plugin_update_icon_cb(WibutiWatcher *watcher, WibutiPlugin *self) {
	wibuti_plugin_redraw_icon(self);
}

static void wibuti_plugin_alignment_changed_cb(GtkScale *scale, WibutiPlugin *self) {
	gdouble value = gtk_range_get_value(GTK_RANGE(scale));
	self->config.alignment = value;
	wibuti_widget_set_alignment(self->widget, value);
	wibuti_config_save_plain(&self->config);
}

static void wibuti_plugin_expand_title_toggled_cb(GtkToggleButton *btn, WibutiPlugin *self) {
	gboolean expand_title = gtk_toggle_button_get_active(btn);
	self->config.expand_title = expand_title;
	wibuti_widget_set_expand_title(self->widget, expand_title);
	wibuti_config_save_plain(&self->config);
}

static void wibuti_plugin_custom_style_toggled_cb(GtkToggleButton *btn, WibutiPlugin *self) {
	self->config.custom_style = gtk_toggle_button_get_active(btn);
	wibuti_plugin_redraw_title(self);
	wibuti_config_save_plain(&self->config);
}

static void wibuti_plugin_active_font_set_cb(GtkFontButton *btn, WibutiPlugin *self) {
	self->config.title_active_font = g_strdup(gtk_font_button_get_font_name(btn));
	wibuti_plugin_redraw_title(self);
	wibuti_config_save_plain(&self->config);
}

static void wibuti_plugin_inactive_font_set_cb(GtkFontButton *btn, WibutiPlugin *self) {
	self->config.title_inactive_font = g_strdup(gtk_font_button_get_font_name(btn));
	wibuti_plugin_redraw_title(self);
	wibuti_config_save_plain(&self->config);
}

static void wibuti_plugin_active_color_set_cb(GtkColorButton *btn, WibutiPlugin *self) {
	GdkColor color;
	gtk_color_button_get_color(btn, &color);
	self->config.title_active_color = gdk_color_to_string(&color);
	wibuti_plugin_redraw_title(self);
	wibuti_config_save_plain(&self->config);
}

static void wibuti_plugin_inactive_color_set_cb(GtkColorButton *btn, WibutiPlugin *self) {
	GdkColor color;
	gtk_color_button_get_color(btn, &color);
	self->config.title_inactive_color = gdk_color_to_string(&color);
	wibuti_plugin_redraw_title(self);
	wibuti_config_save_plain(&self->config);
}

#endif // WIBUTI_WITH_TITLE


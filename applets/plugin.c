#include "applets/plugin.h"

#define ICON_WIDTH	16
#define ICON_HEIGHT	16

static void wibuti_plugin_init(WibutiPlugin *self);

// calbacks
static void wibuti_plugin_update_cb(Watcher *watcher, WibutiPlugin *self);
static void wibuti_plugin_maximized_toggled_cb(GtkToggleButton *btn, WibutiPlugin *self);

#ifdef WIBUTI_WITH_BUTTONS

#endif // WIBUTI_WITH_BUTTONS
#ifdef WIBUTI_WITH_TITLE
static void wibuti_plugin_update_title_cb(Watcher *watcher, WibutiPlugin *self);
static void wibuti_plugin_update_icon_cb(Watcher *watcher, WibutiPlugin *self);

static void wibuti_plugin_alignment_changed_cb(GtkScale *scale, WibutiPlugin *self);
static void wibuti_plugin_expand_title_toggled_cb(GtkToggleButton *btn, WibutiPlugin *self);
static void wibuti_plugin_swap_order_toggled_cb(GtkToggleButton *btn, WibutiPlugin *self);
static void wibuti_plugin_hide_title_toggled_cb(GtkToggleButton *btn, WibutiPlugin *self);
static void wibuti_plugin_hide_icon_toggled_cb(GtkToggleButton *btn, WibutiPlugin *self);
static void wibuti_plugin_custom_style_toggled_cb(GtkToggleButton *btn, WibutiPlugin *self);
static void wibuti_plugin_active_font_set_cb(GtkFontButton *btn, WibutiPlugin *self);
static void wibuti_plugin_inactive_font_set_cb(GtkFontButton *btn, WibutiPlugin *self);
static void wibuti_plugin_active_color_set_cb(GtkColorButton *btn, WibutiPlugin *self);
static void wibuti_plugin_inactive_color_set_cb(GtkColorButton *btn, WibutiPlugin *self);
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
	self->watcher = watcher_new();

	wibuti_config_load_defaults(&self->config);
	watcher_set_only_maximized(self->watcher, self->config.only_maximized);
	
	// connect to windows signals
	g_signal_connect(G_OBJECT(self->watcher), "window-changed",
	                 G_CALLBACK(wibuti_plugin_update_cb), self);
#ifdef WIBUTI_WITH_BUTTONS
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

#ifdef WIBUTI_WITH_BUTTONS
#endif // WIBUTI_WITH_BUTTONS
#ifdef WIBUTI_WITH_TITLE
		g_signal_connect(G_OBJECT(self->prefs->chkb_expand_title), "toggled",
			             G_CALLBACK(wibuti_plugin_expand_title_toggled_cb), self);
		g_signal_connect(G_OBJECT(self->prefs->chkb_swap_order), "toggled",
			             G_CALLBACK(wibuti_plugin_swap_order_toggled_cb), self);
		g_signal_connect(G_OBJECT(self->prefs->chkb_hide_title), "toggled",
			             G_CALLBACK(wibuti_plugin_hide_title_toggled_cb), self);
		g_signal_connect(G_OBJECT(self->prefs->chkb_hide_icon), "toggled",
			             G_CALLBACK(wibuti_plugin_hide_icon_toggled_cb), self);
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
#ifdef WIBUTI_WITH_BUTTONS
#endif // WIBUTI_WITH_BUTTONS
#ifdef WIBUTI_WITH_TITLE
	wibuti_widget_set_alignment(self->widget, self->config.alignment);
	wibuti_widget_set_packing(self->widget, self->config.swap_order, self->config.expand_title);

	wibuti_widget_hide_icon(self->widget, self->config.hide_icon);
	wibuti_widget_hide_title(self->widget, self->config.hide_title);

	watcher_set_only_maximized(self->watcher, self->config.only_maximized);

	wibuti_plugin_update_title(self);
	wibuti_plugin_update_icon(self);
#endif // WIBUTI_WITH_TITLE
}

#ifdef WIBUTI_WITH_BUTTONS
#endif // WIBUTI_WITH_BUTTONS
#ifdef WIBUTI_WITH_TITLE
void wibuti_plugin_update_title(WibutiPlugin *self) {
	gboolean is_active = watcher_is_active(self->watcher);
	if (self->config.custom_style) {
		wibuti_widget_set_title_with_markup(
					self->widget,
					watcher_get_title(self->watcher),
					is_active ? self->config.title_active_font : self->config.title_inactive_font,
					is_active ? self->config.title_active_color : self->config.title_inactive_color
				);
	} else {
		wibuti_widget_set_title(self->widget, watcher_get_title(self->watcher), is_active);
	}
}

void wibuti_plugin_update_icon(WibutiPlugin *self) {
	wibuti_widget_set_icon(self->widget, watcher_get_icon(self->watcher), watcher_is_active(self->watcher));
}
#endif // WIBUTI_WITH_TITLE


/**********************************************************************************************************************/


static void wibuti_plugin_update_cb(Watcher *watcher, WibutiPlugin *self) {
#ifdef WIBUTI_WITH_BUTTONS
#endif // WIBUTI_WITH_BUTTONS
#ifdef WIBUTI_WITH_TITLE
	wibuti_plugin_update_title(self);
	wibuti_plugin_update_icon(self);
#endif // WIBUTI_WITH_TITLE
}

static void wibuti_plugin_maximized_toggled_cb(GtkToggleButton *btn, WibutiPlugin *self) {
	gboolean active = gtk_toggle_button_get_active(btn);
	self->config.only_maximized = active;
	watcher_set_only_maximized(self->watcher, active);
#ifdef WIBUTI_WITH_BUTTONS
#endif // WIBUTI_WITH_BUTTONS
#ifdef WIBUTI_WITH_TITLE
	wibuti_plugin_update_title(self);
	wibuti_plugin_update_icon(self);
	wibuti_config_save_plain(&self->config);
#endif // WIBUTI_WITH_TITLE
}

#ifdef WIBUTI_WITH_BUTTONS
#endif // WIBUTI_WITH_BUTTONS
#ifdef WIBUTI_WITH_TITLE
static void wibuti_plugin_update_title_cb(Watcher *watcher, WibutiPlugin *self) {
	wibuti_plugin_update_title(self);
}

static void wibuti_plugin_update_icon_cb(Watcher *watcher, WibutiPlugin *self) {
	wibuti_plugin_update_icon(self);
}

static void wibuti_plugin_alignment_changed_cb(GtkScale *scale, WibutiPlugin *self) {
	gdouble value = gtk_range_get_value(GTK_RANGE(scale));
	self->config.alignment = value;
	wibuti_widget_set_alignment(self->widget, value);
	wibuti_config_save_plain(&self->config);
}

static void wibuti_plugin_expand_title_toggled_cb(GtkToggleButton *btn, WibutiPlugin *self) {
	self->config.expand_title = gtk_toggle_button_get_active(btn);
	wibuti_widget_set_packing(self->widget, self->config.swap_order, self->config.expand_title);
	wibuti_config_save_plain(&self->config);
}

static void wibuti_plugin_swap_order_toggled_cb(GtkToggleButton *btn, WibutiPlugin *self) {
	self->config.swap_order = gtk_toggle_button_get_active(btn);
	wibuti_widget_set_packing(self->widget, self->config.swap_order, self->config.expand_title);
	wibuti_config_save_plain(&self->config);
}

static void wibuti_plugin_hide_icon_toggled_cb(GtkToggleButton *btn, WibutiPlugin *self) {
	self->config.hide_icon = gtk_toggle_button_get_active(btn);
	wibuti_widget_hide_icon(self->widget, self->config.hide_icon);
	wibuti_config_save_plain(&self->config);
}

static void wibuti_plugin_hide_title_toggled_cb(GtkToggleButton *btn, WibutiPlugin *self) {
	self->config.hide_title = gtk_toggle_button_get_active(btn);
	wibuti_widget_hide_title(self->widget, self->config.hide_title);
	wibuti_config_save_plain(&self->config);
}

static void wibuti_plugin_custom_style_toggled_cb(GtkToggleButton *btn, WibutiPlugin *self) {
	self->config.custom_style = gtk_toggle_button_get_active(btn);
	wibuti_plugin_update_title(self);
	wibuti_config_save_plain(&self->config);
}

static void wibuti_plugin_active_font_set_cb(GtkFontButton *btn, WibutiPlugin *self) {
	self->config.title_active_font = g_strdup(gtk_font_button_get_font_name(btn));
	wibuti_plugin_update_title(self);
	wibuti_config_save_plain(&self->config);
}

static void wibuti_plugin_inactive_font_set_cb(GtkFontButton *btn, WibutiPlugin *self) {
	self->config.title_inactive_font = g_strdup(gtk_font_button_get_font_name(btn));
	wibuti_plugin_update_title(self);
	wibuti_config_save_plain(&self->config);
}

static void wibuti_plugin_active_color_set_cb(GtkColorButton *btn, WibutiPlugin *self) {
	GdkColor color;
	gtk_color_button_get_color(btn, &color);
	self->config.title_active_color = gdk_color_to_string(&color);
	wibuti_plugin_update_title(self);
	wibuti_config_save_plain(&self->config);
}

static void wibuti_plugin_inactive_color_set_cb(GtkColorButton *btn, WibutiPlugin *self) {
	GdkColor color;
	gtk_color_button_get_color(btn, &color);
	self->config.title_inactive_color = gdk_color_to_string(&color);
	wibuti_plugin_update_title(self);
	wibuti_config_save_plain(&self->config);
}
#endif // WIBUTI_WITH_TITLE


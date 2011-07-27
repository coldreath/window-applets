#include <title/title-plugin.h>

#define ICON_WIDTH	16
#define ICON_HEIGHT	16

static void wt_plugin_init(WTPlugin *self);

// calbacks
static void wt_plugin_update_cb(Watcher *watcher, WTPlugin *self);
static void wt_plugin_update_title_cb(Watcher *watcher, WTPlugin *self);
static void wt_plugin_update_icon_cb(Watcher *watcher, WTPlugin *self);
static void wt_plugin_alignment_changed_cb(GtkScale *scale, WTPlugin *self);
static void wt_plugin_maximized_toggled_cb(GtkToggleButton *btn, WTPlugin *self);
static void wt_plugin_expand_title_toggled_cb(GtkToggleButton *btn, WTPlugin *self);
static void wt_plugin_swap_order_toggled_cb(GtkToggleButton *btn, WTPlugin *self);
static void wt_plugin_hide_title_toggled_cb(GtkToggleButton *btn, WTPlugin *self);
static void wt_plugin_hide_icon_toggled_cb(GtkToggleButton *btn, WTPlugin *self);
static void wt_plugin_custom_style_toggled_cb(GtkToggleButton *btn, WTPlugin *self);
static void wt_plugin_active_font_set_cb(GtkFontButton *btn, WTPlugin *self);
static void wt_plugin_inactive_font_set_cb(GtkFontButton *btn, WTPlugin *self);
static void wt_plugin_active_color_set_cb(GtkColorButton *btn, WTPlugin *self);
static void wt_plugin_inactive_color_set_cb(GtkColorButton *btn, WTPlugin *self);


/**********************************************************************************************************************/


GType wt_plugin_get_type(void) {
	static GType type = 0;
	if (type == 0) {
		static const GTypeInfo info = {
					sizeof(WTPluginClass),
					NULL,   // base_init
					NULL,   // base_finalize
					(GClassInitFunc) NULL,   // class_init
					NULL,   // class_finalize
					NULL,   // class_data
					sizeof(WTPlugin),
					0,      // n_preallocs
					(GInstanceInitFunc) wt_plugin_init    // instance_init
				};
		type = g_type_register_static(G_TYPE_OBJECT, "WTPluginType", &info, 0);
	}
	return type;
}

static void wt_plugin_init(WTPlugin *self) {
	self->widget = wt_widget_new();
	self->prefs = wt_preferences_new();
	self->watcher = watcher_new();

	wt_config_load_defaults(&self->config);
	watcher_set_only_maximized(self->watcher, self->config.only_maximized);
	
	// connect to windows signals
	g_signal_connect(G_OBJECT(self->watcher), "window-changed",
	                 G_CALLBACK(wt_plugin_update_cb), self);
	g_signal_connect(G_OBJECT(self->watcher), "name-changed",
	                 G_CALLBACK(wt_plugin_update_title_cb), self);
	g_signal_connect(G_OBJECT(self->watcher), "icon-changed",
	                 G_CALLBACK(wt_plugin_update_icon_cb), self);
}

WTPlugin *wt_plugin_new() {
	return WT_PLUGIN(g_object_new(WT_TYPE_PLUGIN, NULL));
}


/**********************************************************************************************************************/


void wt_plugin_show_preferences(WTPlugin *self) {
	static gboolean connected = FALSE;
	if (!connected) {
		wt_preferences_set_from_config(self->prefs, &self->config);
		
		g_signal_connect(G_OBJECT(self->prefs->chkb_only_maximized), "toggled",
			             G_CALLBACK(wt_plugin_maximized_toggled_cb), self);
		g_signal_connect(G_OBJECT(self->prefs->chkb_expand_title), "toggled",
			             G_CALLBACK(wt_plugin_expand_title_toggled_cb), self);
		g_signal_connect(G_OBJECT(self->prefs->chkb_swap_order), "toggled",
			             G_CALLBACK(wt_plugin_swap_order_toggled_cb), self);
		g_signal_connect(G_OBJECT(self->prefs->chkb_hide_title), "toggled",
			             G_CALLBACK(wt_plugin_hide_title_toggled_cb), self);
		g_signal_connect(G_OBJECT(self->prefs->chkb_hide_icon), "toggled",
			             G_CALLBACK(wt_plugin_hide_icon_toggled_cb), self);
		g_signal_connect(G_OBJECT(self->prefs->chkb_custom_style), "toggled",
			             G_CALLBACK(wt_plugin_custom_style_toggled_cb), self);
	
		g_signal_connect(G_OBJECT(self->prefs->btn_font_active), "font-set",
			             G_CALLBACK(wt_plugin_active_font_set_cb), self);
		g_signal_connect(G_OBJECT(self->prefs->btn_font_inactive), "font-set",
			             G_CALLBACK(wt_plugin_inactive_font_set_cb), self);
		g_signal_connect(G_OBJECT(self->prefs->btn_color_active), "color-set",
			             G_CALLBACK(wt_plugin_active_color_set_cb), self);
		g_signal_connect(G_OBJECT(self->prefs->btn_color_inactive), "color-set",
			             G_CALLBACK(wt_plugin_inactive_color_set_cb), self);

		g_signal_connect(G_OBJECT(self->prefs->scale_alignment), "value-changed",
			             G_CALLBACK(wt_plugin_alignment_changed_cb), self);	
			             
		connected = TRUE;
	}

	gtk_widget_show_all(GTK_WIDGET(self->prefs));
}

void wt_plugin_update(WTPlugin *self) {
	wt_widget_set_alignment(self->widget, self->config.alignment);
	wt_widget_set_packing(self->widget, self->config.swap_order, self->config.expand_title);

	wt_widget_hide_icon(self->widget, self->config.hide_icon);
	wt_widget_hide_title(self->widget, self->config.hide_title);

	watcher_set_only_maximized(self->watcher, self->config.only_maximized);

	wt_plugin_update_title(self);
	wt_plugin_update_icon(self);
}

void wt_plugin_update_title(WTPlugin *self) {
	gboolean is_active = watcher_is_active(self->watcher);
	if (self->config.custom_style) {
		wt_widget_set_title_with_markup(
					self->widget,
					watcher_get_title(self->watcher),
					is_active ? self->config.title_active_font : self->config.title_inactive_font,
					is_active ? self->config.title_active_color : self->config.title_inactive_color
				);
	} else {
		wt_widget_set_title(self->widget, watcher_get_title(self->watcher), is_active);
	}
}

void wt_plugin_update_icon(WTPlugin *self) {
	wt_widget_set_icon(self->widget, watcher_get_icon(self->watcher), watcher_is_active(self->watcher));
}


/**********************************************************************************************************************/


static void wt_plugin_update_cb(Watcher *watcher, WTPlugin *self) {
	wt_plugin_update_title(self);
	wt_plugin_update_icon(self);
}

static void wt_plugin_update_title_cb(Watcher *watcher, WTPlugin *self) {
	wt_plugin_update_title(self);
}

static void wt_plugin_update_icon_cb(Watcher *watcher, WTPlugin *self) {
	wt_plugin_update_icon(self);
}

static void wt_plugin_alignment_changed_cb(GtkScale *scale, WTPlugin *self) {
	gdouble value = gtk_range_get_value(GTK_RANGE(scale));
	self->config.alignment = value;
	wt_widget_set_alignment(self->widget, value);
	wt_config_save_plain(&self->config);
}

static void wt_plugin_maximized_toggled_cb(GtkToggleButton *btn, WTPlugin *self) {
	gboolean active = gtk_toggle_button_get_active(btn);
	self->config.only_maximized = active;
	watcher_set_only_maximized(self->watcher, active);
	wt_plugin_update_title(self);
	wt_plugin_update_icon(self);
	wt_config_save_plain(&self->config);
}

static void wt_plugin_expand_title_toggled_cb(GtkToggleButton *btn, WTPlugin *self) {
	self->config.expand_title = gtk_toggle_button_get_active(btn);
	wt_widget_set_packing(self->widget, self->config.swap_order, self->config.expand_title);
	wt_config_save_plain(&self->config);
}

static void wt_plugin_swap_order_toggled_cb(GtkToggleButton *btn, WTPlugin *self) {
	self->config.swap_order = gtk_toggle_button_get_active(btn);
	wt_widget_set_packing(self->widget, self->config.swap_order, self->config.expand_title);
	wt_config_save_plain(&self->config);
}

static void wt_plugin_hide_icon_toggled_cb(GtkToggleButton *btn, WTPlugin *self) {
	self->config.hide_icon = gtk_toggle_button_get_active(btn);
	wt_widget_hide_icon(self->widget, self->config.hide_icon);
	wt_config_save_plain(&self->config);
}

static void wt_plugin_hide_title_toggled_cb(GtkToggleButton *btn, WTPlugin *self) {
	self->config.hide_title = gtk_toggle_button_get_active(btn);
	wt_widget_hide_title(self->widget, self->config.hide_title);
	wt_config_save_plain(&self->config);
}

static void wt_plugin_custom_style_toggled_cb(GtkToggleButton *btn, WTPlugin *self) {
	self->config.custom_style = gtk_toggle_button_get_active(btn);
	wt_plugin_update_title(self);
	wt_config_save_plain(&self->config);
}

static void wt_plugin_active_font_set_cb(GtkFontButton *btn, WTPlugin *self) {
	self->config.title_active_font = g_strdup(gtk_font_button_get_font_name(btn));
	wt_plugin_update_title(self);
	wt_config_save_plain(&self->config);
}

static void wt_plugin_inactive_font_set_cb(GtkFontButton *btn, WTPlugin *self) {
	self->config.title_inactive_font = g_strdup(gtk_font_button_get_font_name(btn));
	wt_plugin_update_title(self);
	wt_config_save_plain(&self->config);
}

static void wt_plugin_active_color_set_cb(GtkColorButton *btn, WTPlugin *self) {
	GdkColor color;
	gtk_color_button_get_color(btn, &color);
	self->config.title_active_color = gdk_color_to_string(&color);
	wt_plugin_update_title(self);
	wt_config_save_plain(&self->config);
}

static void wt_plugin_inactive_color_set_cb(GtkColorButton *btn, WTPlugin *self) {
	GdkColor color;
	gtk_color_button_get_color(btn, &color);
	self->config.title_inactive_color = gdk_color_to_string(&color);
	wt_plugin_update_title(self);
	wt_config_save_plain(&self->config);
}

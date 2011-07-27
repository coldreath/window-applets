#include <xfce/xfce4-title-plugin.h>

static void wt_xfce_plugin_init(WTXfcePlugin *self);

// callbacks
static void wt_xfce_plugin_show_preferences_cb(WTXfcePlugin *panel_plugin, WTPlugin *title_plugin);
static void wt_xfce_plugin_position_changed_cb(XfcePanelPlugin *panel, XfceScreenPosition position, WTPlugin *title);


/**********************************************************************************************************************/


GType wt_xfce_plugin_get_type(void) {
	static GType type = 0;
	if (type == 0) {
		static const GTypeInfo info = {
					sizeof(WTXfcePluginClass),
					NULL,
					NULL,
					(GClassInitFunc) NULL,
					NULL,
					NULL,
					sizeof(WTXfcePlugin),
					0,
					(GInstanceInitFunc) wt_xfce_plugin_init
				};
		type = g_type_register_static(WT_TYPE_PLUGIN, "WTXfcePluginType", &info, 0);
	}
	return type;
}

static void wt_xfce_plugin_init(WTXfcePlugin *self) {
	self->panel_plugin = NULL;
}

WTXfcePlugin *wt_xfce_plugin_new_with_xfce_panel_plugin(XfcePanelPlugin *panel_plugin) {
	WTXfcePlugin *title_plugin = WT_XFCE_PLUGIN(g_object_new(WT_TYPE_XFCE_PLUGIN, NULL));
	title_plugin->panel_plugin = panel_plugin;

	xfce_panel_plugin_menu_show_configure(title_plugin->panel_plugin);
	g_signal_connect(G_OBJECT(panel_plugin), "configure-plugin",
	                 G_CALLBACK(wt_xfce_plugin_show_preferences_cb), title_plugin);
	
	g_signal_connect(G_OBJECT(panel_plugin), "screen-position-changed",
	                 G_CALLBACK(wt_xfce_plugin_position_changed_cb), title_plugin);
	
	gtk_container_add(GTK_CONTAINER(panel_plugin), GTK_WIDGET(WT_PLUGIN(title_plugin)->widget));
	gtk_widget_show_all(GTK_WIDGET(WT_PLUGIN(title_plugin)->widget));

	wt_config_load_plain(&WT_PLUGIN(title_plugin)->config);
	wt_preferences_set_from_config(WT_PLUGIN(title_plugin)->prefs, &WT_PLUGIN(title_plugin)->config);
	wt_plugin_update(WT_PLUGIN(title_plugin));
	xfce_panel_plugin_set_expand(panel_plugin, WT_PLUGIN(title_plugin)->config.expand_title);

	return title_plugin;
}


/**********************************************************************************************************************/


static void wt_xfce_plugin_show_preferences_cb(WTXfcePlugin *panel_plugin, WTPlugin *title_plugin) {
	wt_plugin_show_preferences(title_plugin);
}

static void wt_xfce_plugin_position_changed_cb(XfcePanelPlugin *panel, XfceScreenPosition position, WTPlugin *title) {
	WTWidgetAngle angle;
	if (XFCE_SCREEN_POSITION_NW_H == position ||
				XFCE_SCREEN_POSITION_N == position ||
				XFCE_SCREEN_POSITION_NE_H == position ||
				XFCE_SCREEN_POSITION_FLOATING_H == position) {
		angle = WT_WIDGET_ANGLE_UP;
	} else if (XFCE_SCREEN_POSITION_NW_V == position ||
				XFCE_SCREEN_POSITION_W == position ||
				XFCE_SCREEN_POSITION_SW_V == position ||
				XFCE_SCREEN_POSITION_FLOATING_V == position) {
		angle = WT_WIDGET_ANGLE_LEFT;
	} else if (XFCE_SCREEN_POSITION_NE_V == position ||
				XFCE_SCREEN_POSITION_E == position ||
				XFCE_SCREEN_POSITION_SE_V == position) {
		angle = WT_WIDGET_ANGLE_RIGHT;
	} else if (XFCE_SCREEN_POSITION_SW_H == position ||
				XFCE_SCREEN_POSITION_S == position ||
				XFCE_SCREEN_POSITION_SE_H == position) {
		angle = WT_WIDGET_ANGLE_DOWN;
	} else {
		angle = WT_WIDGET_ANGLE_UP;
	}
	wt_widget_change_angle(title->widget, angle);
}


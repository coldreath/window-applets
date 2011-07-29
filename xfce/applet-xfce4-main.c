#include <libxfce4panel/xfce-panel-plugin.h>

#include "config.h"
#include "applets/plugin.h"

#define WIBUTI_TYPE_XFCE				(wibuti_xfce_get_type())
#define WIBUTI_XFCE(obj)				(G_TYPE_CHECK_INSTANCE_CAST((obj), WIBUTI_TYPE_XFCE, WibutiXfce))
#define WIBUTI_IS_XFCE(obj)				(G_TYPE_CHECK_INSTANCE_TYPE((obj), WIBUTI_TYPE_XFCE))
#define WIBUTI_XFCE_CLASS(klass)		(G_TYPE_CHECK_CLASS_CAST((klass), WIBUTI_TYPE_XFCE, WibutiXfceClass))
#define WIBUTI_IS_XFCE_CLASS(klass)		(G_TYPE_CHECK_CLASS_TYPE((klass), WIBUTI_TYPE_XFCE))
#define WIBUTI_XFCE_GET_CLASS(obj)		(G_TYPE_INSTANCE_GET_CLASS((obj), WIBUTI_TYPE_XFCE, WibutiXfceClass))

typedef struct {
	WibutiPlugin parent;
	XfcePanelPlugin *panel;
} WibutiXfce;

typedef struct {
	WibutiPluginClass parent;
} WibutiXfceClass;

static void wibuti_xfce_init(WibutiXfce *self);

// callbacks
static void wibuti_xfce_show_preferences_cb(XfcePanelPlugin *panel, WibutiXfce *self);
static void wibuti_xfce_position_changed_cb(XfcePanelPlugin *panel, XfceScreenPosition position, WibutiXfce *self);

/**********************************************************************************************************************/

GType wibuti_xfce_get_type(void) {
	static GType type = 0;
	if (type == 0) {
		static const GTypeInfo info = {
					sizeof(WibutiXfceClass),
					NULL,
					NULL,
					(GClassInitFunc) NULL,
					NULL,
					NULL,
					sizeof(WibutiXfce),
					0,
					(GInstanceInitFunc) wibuti_xfce_init
				};
		type = g_type_register_static(WIBUTI_TYPE_PLUGIN, "WibutiXfce", &info, 0);
	}
	return type;
}

static void wibuti_xfce_init(WibutiXfce *self) {
	self->panel = NULL;
}

WibutiXfce *wibuti_xfce_new_with_xfce_panel(XfcePanelPlugin *panel) {
	WibutiXfce *applet = WIBUTI_XFCE(g_object_new(WIBUTI_TYPE_XFCE, NULL));
	applet->panel = panel;
	
	xfce_panel_plugin_menu_show_configure(panel);

	g_signal_connect(G_OBJECT(panel), "configure-plugin", G_CALLBACK(wibuti_xfce_show_preferences_cb), applet);
	g_signal_connect(G_OBJECT(panel), "screen-position-changed", G_CALLBACK(wibuti_xfce_position_changed_cb), applet);

	gtk_container_add(GTK_CONTAINER(panel), GTK_WIDGET(WIBUTI_PLUGIN(applet)->widget));
	gtk_widget_show_all(GTK_WIDGET(WIBUTI_PLUGIN(applet)->widget));

	wibuti_config_load_plain(&WIBUTI_PLUGIN(applet)->config);
	wibuti_prefs_set_from_config(WIBUTI_PLUGIN(applet)->prefs, &WIBUTI_PLUGIN(applet)->config);
	wibuti_plugin_update(WIBUTI_PLUGIN(applet));

#ifdef WIBUTI_WITH_BUTTONS
	xfce_panel_plugin_set_expand(panel, FALSE);
#endif // WIBUTI_WITH_BUTTONS
#ifdef WIBUTI_WITH_TITLE
	xfce_panel_plugin_set_expand(panel, WIBUTI_PLUGIN(applet)->config.expand_title);
#endif // WIBUTI_WITH_TITLE

	return applet;
}

/**********************************************************************************************************************/

static void wibuti_xfce_show_preferences_cb(XfcePanelPlugin *panel, WibutiXfce *self) {
	wibuti_plugin_show_preferences(WIBUTI_PLUGIN(self));
}

static void wibuti_xfce_position_changed_cb(XfcePanelPlugin *panel, XfceScreenPosition position, WibutiXfce *self) {
	WibutiWidgetAngle angle;
	if (XFCE_SCREEN_POSITION_NW_H == position
	|| XFCE_SCREEN_POSITION_N == position
	|| XFCE_SCREEN_POSITION_NE_H == position
	|| XFCE_SCREEN_POSITION_FLOATING_H == position) {
		angle = WIBUTI_WIDGET_ANGLE_UP;
	} else if (XFCE_SCREEN_POSITION_NW_V == position
	|| XFCE_SCREEN_POSITION_W == position
	|| XFCE_SCREEN_POSITION_SW_V == position
	|| XFCE_SCREEN_POSITION_FLOATING_V == position) {
		angle = WIBUTI_WIDGET_ANGLE_LEFT;
	} else if (XFCE_SCREEN_POSITION_NE_V == position
	|| XFCE_SCREEN_POSITION_E == position
	|| XFCE_SCREEN_POSITION_SE_V == position) {
		angle = WIBUTI_WIDGET_ANGLE_RIGHT;
	} else if (XFCE_SCREEN_POSITION_SW_H == position
	|| XFCE_SCREEN_POSITION_S == position
	|| XFCE_SCREEN_POSITION_SE_H == position) {
		angle = WIBUTI_WIDGET_ANGLE_DOWN;
	} else {
		angle = WIBUTI_WIDGET_ANGLE_UP;
	}
	wibuti_widget_change_angle(WIBUTI_PLUGIN(self)->widget, angle);
}

/**********************************************************************************************************************/

WibutiXfce *applet;

static void wibuti_xfce_panel_plugin_init(XfcePanelPlugin *panel) {
	applet = wibuti_xfce_new_with_xfce_panel(panel);
}

XFCE_PANEL_PLUGIN_REGISTER_EXTERNAL(wibuti_xfce_panel_plugin_init);


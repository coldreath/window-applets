#ifndef __WIBUTI_PLUGIN_H__
#define __WIBUTI_PLUGIN_H__

#include <glib-object.h>

#include <applets/preferences.h>
#include <applets/widget.h>
#include <applets/watcher.h>

#define WIBUTI_TYPE_PLUGIN				(wibuti_plugin_get_type ())
#define WIBUTI_PLUGIN(obj)				(G_TYPE_CHECK_INSTANCE_CAST((obj), WIBUTI_TYPE_PLUGIN, WibutiPlugin))
#define WIBUTI_IS_PLUGIN(obj)			(G_TYPE_CHECK_INSTANCE_TYPE((obj), WIBUTI_TYPE_PLUGIN))
#define WIBUTI_PLUGIN_CLASS(klass)		(G_TYPE_CHECK_CLASS_CAST((klass), WIBUTI_TYPE_PLUGIN, WibutiPluginClass))
#define WIBUTI_IS_PLUGIN_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE((klass), WIBUTI_TYPE_PLUGIN))
#define WIBUTI_PLUGIN_GET_CLASS(obj)	(G_TYPE_INSTANCE_GET_CLASS((obj), WIBUTI_TYPE_PLUGIN, WibutiPluginClass))

typedef struct {
	GObject parent;

	WibutiWidget *widget;
	WibutiPrefs *prefs;
	WibutiConfig config;
	WibutiWatcher *watcher;
} WibutiPlugin;

typedef struct {
	GObjectClass parent;
} WibutiPluginClass;

GType wibuti_plugin_get_type(void);
WibutiPlugin* wibuti_plugin_new(void);

void wibuti_plugin_show_preferences(WibutiPlugin *);
void wibuti_plugin_update(WibutiPlugin *self);
void wibuti_plugin_redraw(WibutiPlugin *self);

#ifdef WIBUTI_WITH_BUTTONS
void wibuti_plugin_redraw_buttons(WibutiPlugin *);
#endif // WIBUTI_WITH_BUTTONS

#ifdef WIBUTI_WITH_TITLE
void wibuti_plugin_redraw_title(WibutiPlugin *);
void wibuti_plugin_redraw_icon(WibutiPlugin *);
#endif // WIBUTI_WITH_TITLE

#endif // __WIBUTI_PLUGIN_H__

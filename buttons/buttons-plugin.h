#ifndef __BUTTONS_PLUGIN_H__
#define __BUTTONS_PLUGIN_H__

#include <glib-object.h>

#include <buttons/buttons-preferences.h>
#include <buttons/buttons-widget.h>
#include <common/watcher.h>

#define WB_TYPE_PLUGIN				(wb_plugin_get_type ())
#define WB_PLUGIN(obj)				(G_TYPE_CHECK_INSTANCE_CAST((obj), WB_TYPE_PLUGIN, WBPlugin))
#define WB_IS_PLUGIN(obj)			(G_TYPE_CHECK_INSTANCE_TYPE((obj), WB_TYPE_PLUGIN))
#define WB_PLUGIN_CLASS(klass)		(G_TYPE_CHECK_CLASS_CAST((klass), WB_TYPE_PLUGIN, WBPluginClass))
#define WB_IS_PLUGIN_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE((klass), WB_TYPE_PLUGIN))
#define WB_PLUGIN_GET_CLASS(obj)	(G_TYPE_INSTANCE_GET_CLASS((obj), WB_TYPE_PLUGIN, WBPluginClass))

typedef struct {
	GObject parent;

	WBWidget *widget;
	WBPreferences *prefs;
	WBConfig config;
	Watcher *watcher;
} WBPlugin;

typedef struct {
	GObjectClass parent;
} WBPluginClass;

GType wb_plugin_get_type(void);
WBPlugin* wb_plugin_new(void);

void wb_plugin_show_preferences(WBPlugin *self);

#endif /* __BUTTONS_PLUGIN_H__ */

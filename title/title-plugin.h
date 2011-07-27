#ifndef __TITLE_PLUGIN_H__
#define __TITLE_PLUGIN_H__

#include <glib-object.h>

#include <title/title-preferences.h>
#include <title/title-widget.h>
#include <common/watcher.h>

#define WT_TYPE_PLUGIN				(wt_plugin_get_type ())
#define WT_PLUGIN(obj)				(G_TYPE_CHECK_INSTANCE_CAST((obj), WT_TYPE_PLUGIN, WTPlugin))
#define WT_IS_PLUGIN(obj)			(G_TYPE_CHECK_INSTANCE_TYPE((obj), WT_TYPE_PLUGIN))
#define WT_PLUGIN_CLASS(klass)		(G_TYPE_CHECK_CLASS_CAST((klass), WT_TYPE_PLUGIN, WTPluginClass))
#define WT_IS_PLUGIN_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE((klass), WT_TYPE_PLUGIN))
#define WT_PLUGIN_GET_CLASS(obj)	(G_TYPE_INSTANCE_GET_CLASS((obj), WT_TYPE_PLUGIN, WTPluginClass))

typedef struct {
	GObject parent;

	WTWidget *widget;
	WTPreferences *prefs;
	WTConfig config;
	Watcher *watcher;
} WTPlugin;

typedef struct {
	GObjectClass parent;
} WTPluginClass;

GType wt_plugin_get_type(void);
WTPlugin* wt_plugin_new(void);

void wt_plugin_show_preferences(WTPlugin *self);
void wt_plugin_update(WTPlugin *self);
void wt_plugin_update_title(WTPlugin *self);
void wt_plugin_update_icon(WTPlugin *self);

#endif /* __TITLE_PLUGIN_H__ */

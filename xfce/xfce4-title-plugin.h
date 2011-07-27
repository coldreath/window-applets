#ifndef __XFCE4_TITLE_PLUGIN_H__
#define __XFCE4_TITLE_PLUGIN_H__

#include <libxfce4panel/xfce-panel-plugin.h>

#include <title/title-plugin.h>

#define WT_TYPE_XFCE_PLUGIN				(wt_xfce_plugin_get_type())
#define WT_XFCE_PLUGIN(obj)				(G_TYPE_CHECK_INSTANCE_CAST((obj), WT_TYPE_XFCE_PLUGIN, WTXfcePlugin))
#define WT_IS_XFCE_PLUGIN(obj)			(G_TYPE_CHECK_INSTANCE_TYPE((obj), WT_TYPE_XFCE_PLUGIN))
#define WT_XFCE_PLUGIN_CLASS(klass)		(G_TYPE_CHECK_CLASS_CAST((klass), WT_TYPE_XFCE_PLUGIN, WTXfcePluginClass))
#define WT_IS_XFCE_PLUGIN_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE((klass), WT_TYPE_XFCE_PLUGIN))
#define WT_XFCE_PLUGIN_GET_CLASS(obj)	(G_TYPE_INSTANCE_GET_CLASS((obj), WT_TYPE_XFCE_PLUGIN, WTXfcePluginClass))

typedef struct {
	WTPlugin parent;
	XfcePanelPlugin *panel_plugin;
} WTXfcePlugin;

typedef struct {
	WTPluginClass parent;
} WTXfcePluginClass;

GType wt_xfce_plugin_get_type(void);
WTXfcePlugin *wt_xfce_plugin_new_with_xfce_panel_plugin(XfcePanelPlugin *panel_plugin);

#endif /* __XFCE4_TITLE_PLUGIN_H__ */

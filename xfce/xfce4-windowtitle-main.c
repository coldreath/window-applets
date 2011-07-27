#include <xfce/xfce4-title-plugin.h>

WTXfcePlugin *title_plugin;
static void xfce_panel_plugin_init(XfcePanelPlugin *panel_plugin) {
	title_plugin = wt_xfce_plugin_new_with_xfce_panel_plugin(panel_plugin);
}

XFCE_PANEL_PLUGIN_REGISTER_EXTERNAL(xfce_panel_plugin_init);


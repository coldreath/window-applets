#include <lxpanel/plugin.h>

#include <title/title-plugin.h>

typedef struct {
	WTPlugin *title;
	Plugin *panel;
} WTLxdePlugin;

static int window_title_constructor(Plugin *p, char** fp) {
	WTLxdePlugin *plugin = g_new0(WTLxdePlugin, 1);
	plugin->panel = p;
	plugin->title = WT_PLUGIN(g_object_new(WT_TYPE_PLUGIN, NULL));

	plugin->panel->pwid = GTK_WIDGET(plugin->title->widget);
	gtk_widget_show_all(GTK_WIDGET(plugin->title->widget));
	wt_plugin_update(plugin->title);
	
	return TRUE;
}

static void window_title_destructor(Plugin *p) {
}

static void window_title_config(Plugin *p, GtkWindow *parent) {
}

static void window_title_save(Plugin *p, FILE *fp) {
}

static void window_title_panel_config_changed(Plugin *p) {
}

PluginClass windowtitle_plugin_class = {
    PLUGINCLASS_VERSIONING,

    type : "windowtitle",
    name : "Window Title Plugin",
    version : "0.1",
    description : "Shows window title on panel",

    constructor : window_title_constructor,
    destructor  : window_title_destructor,
    config      : window_title_config,
    save        : window_title_save,
    panel_configuration_changed : window_title_panel_config_changed
};


#include <lxpanel/plugin.h>

#include <title/title-plugin.h>

static gboolean window_title_press_event(GtkWidget* widget, GdkEventButton* evt, Plugin* p);

static int window_title_constructor(Plugin *p, char** fp) {
	p->priv = WT_PLUGIN(g_object_new(WT_TYPE_PLUGIN, NULL));
	p->pwid = GTK_WIDGET(WT_PLUGIN(p->priv)->widget);
	p->expand = TRUE;
	gtk_widget_show_all(GTK_WIDGET(WT_PLUGIN(p->priv)->widget));
	wt_config_load_plain(&WT_PLUGIN(p->priv)->config);
	wt_preferences_set_from_config(WT_PLUGIN(p->priv)->prefs, &WT_PLUGIN(p->priv)->config);
	wt_plugin_update(WT_PLUGIN(p->priv));
	
    g_signal_connect(WT_WIDGET(p->pwid)->evbox_title, "button-press-event", G_CALLBACK(window_title_press_event), p);
	
	return TRUE;
}

static void window_title_destructor(Plugin *p) {
	g_object_unref(WT_PLUGIN(p->priv)->widget);
}

static void window_title_config(Plugin *p, GtkWindow *parent) {
	wt_plugin_show_preferences(WT_PLUGIN(p->priv));
}

static void window_title_save(Plugin *p, FILE *fp) {
}

static void window_title_panel_config_changed(Plugin *p) {
	if (p->panel->orientation == ORIENT_HORIZ) {
		wt_widget_change_angle(WT_PLUGIN(p->priv)->widget, WT_WIDGET_ANGLE_UP);
	} else {
		wt_widget_change_angle(WT_PLUGIN(p->priv)->widget, WT_WIDGET_ANGLE_LEFT);
	}
}

static gboolean window_title_press_event(GtkWidget* widget, GdkEventButton* evt, Plugin* p) {
	if (evt->button == 1) { 
		wt_plugin_show_preferences(WT_PLUGIN(p->priv));
		return TRUE;
	}
	return FALSE;
}

PluginClass libwindowtitle_plugin_class = {
    PLUGINCLASS_VERSIONING,

    type : "libwindowtitle",
    name : "Window Title Plugin",
    version : "0.1",
    description : "Shows window title on panel",

    constructor : window_title_constructor,
    destructor  : window_title_destructor,
    config      : window_title_config,
    save        : window_title_save,
    panel_configuration_changed : window_title_panel_config_changed
};


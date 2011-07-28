#include <buttons/buttons-plugin.h>

static void wb_plugin_init(WBPlugin *self);

// calbacks
static void wb_plugin_update_cb(Watcher *watcher, WBPlugin *self);

/**********************************************************************************************************************/


GType wb_plugin_get_type(void) {
	static GType type = 0;
	if (type == 0) {
		static const GTypeInfo info = {
					sizeof(WBPluginClass),
					NULL,   // base_init
					NULL,   // base_finalize
					(GClassInitFunc) NULL,   // class_init
					NULL,   // class_finalize
					NULL,   // class_data
					sizeof(WBPlugin),
					0,      // n_preallocs
					(GInstanceInitFunc) wb_plugin_init    // instance_init
				};
		type = g_type_register_static(G_TYPE_OBJECT, "WBPluginType", &info, 0);
	}
	return type;
}

static void wb_plugin_init(WBPlugin *self) {
	self->widget = wb_widget_new();
	self->prefs = wb_preferences_new();
	self->watcher = watcher_new();

	wb_config_load_defaults(&self->config);
//	watcher_set_only_maximized(self->watcher, self->config.only_maximized);
	
	// connect to windows signals
	g_signal_connect(G_OBJECT(self->watcher), "window-changed",
	                 G_CALLBACK(wb_plugin_update_cb), self);
}

WBPlugin *wt_plugin_new() {
	return WB_PLUGIN(g_object_new(WB_TYPE_PLUGIN, NULL));
}


/**********************************************************************************************************************/


void wb_plugin_show_preferences(WBPlugin *self) {
	static gboolean connected = FALSE;
	if (!connected) {
		wb_preferences_set_from_config(self->prefs, &self->config);
		connected = TRUE;
	}

	gtk_widget_show_all(GTK_WIDGET(self->prefs));
}

void wb_plugin_update(WBPlugin *self) {
}

static void wb_plugin_update_cb(Watcher *watcher, WBPlugin *self) {
}


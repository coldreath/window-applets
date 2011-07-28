#include <buttons/buttons-plugin.h>

static void wb_plugin_init(WBPlugin *self);

// calbacks


/**********************************************************************************************************************/


GType wb_plugin_get_type(void) {
}

static void wt_plugin_init(WBPlugin *self) {
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


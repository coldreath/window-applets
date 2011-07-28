#include <glib/gstdio.h>
#include <gtk/gtk.h>

#include <buttons/buttons-preferences.h>

#define PATH_MAIN		"/usr/share/windowbuttons"
#define PATH_UI_PREFS	PATH_MAIN"/buttons-preferences.ui"

// callbacks
gboolean wb_preferences_delete_cb(GtkWidget *widget, GdkEvent *event, WBPreferences *self);
void wb_preferences_close_cb(GtkWidget *widget, WBPreferences *self);


G_DEFINE_TYPE(WBPreferences, wb_preferences, GTK_TYPE_WINDOW);

static void wb_preferences_init(WBPreferences *self) {
	GError *err = NULL;
	self->builder = gtk_builder_new();
	gtk_builder_add_from_file(self->builder, PATH_UI_PREFS, &err);
	
	if (err != NULL) {
		g_fprintf(stderr, "%s\n", err->message);
		g_error_free(err);
	} else {
		self->main_box = GTK_BOX(gtk_builder_get_object(self->builder, "main_box"));
		self->btn_close = GTK_BUTTON(gtk_builder_get_object(self->builder, "btn_close"));
	}
	
	g_object_set(GTK_WINDOW(self), "window-position", GTK_WIN_POS_CENTER_ALWAYS, NULL);
	g_object_set(GTK_WINDOW(self), "border-width", 3, NULL);
	g_object_set(GTK_WINDOW(self), "title", "Window Buttons Plugin Preferences", NULL);
}

static void wb_preferences_class_init(WBPreferencesClass *klass) {
}

WBPreferences* wb_preferences_new(void) {
	return WB_PREFERENCES(g_object_new(WB_TYPE_PREFERENCES, NULL));
}

void wb_preferences_set_from_config(WBPreferences *self, WBConfig *config) {
}


/**********************************************************************************************************************/


gboolean wb_preferences_delete_cb(GtkWidget *widget, GdkEvent *event, WBPreferences *self) {
	gtk_widget_hide(GTK_WIDGET(&self->parent));
	return TRUE;
}

void wb_preferences_close_cb(GtkWidget *widget, WBPreferences *self) {
	gtk_widget_hide(GTK_WIDGET(&self->parent));
}


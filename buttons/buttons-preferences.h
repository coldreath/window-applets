#ifndef __BUTTONS_PREFERENCES_H__
#define __BUTTONS_PREFERENCES_H__

#include <gtk/gtk.h>

#include <buttons/buttons-config.h>

#define WB_TYPE_PREFERENCES				(wb_preferences_get_type())
#define WB_PREFERENCES(obj)				(G_TYPE_CHECK_INSTANCE_CAST((obj), WB_TYPE_PREFERENCES, WBPreferences))
#define WB_IS_PREFERENCES(obj)			(G_TYPE_CHECK_INSTANCE_TYPE((obj), WB_TYPE_PREFERENCES))
#define WB_PREFERENCES_CLASS(klass)		(G_TYPE_CHECK_CLASS_CAST((klass), WB_TYPE_PREFERENCES, WBPreferencesClass))
#define WB_IS_PREFERENCES_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE((klass), WB_TYPE_PREFERENCES))
#define WB_PREFERENCES_GET_CLASS(obj)	(G_TYPE_INSTANCE_GET_CLASS((obj), WB_TYPE_PREFERENCES, WBPreferencesClass))

typedef struct {
	GtkWindow parent;

	GtkBuilder *builder;
	GtkBox *main_box;
	GtkButton *btn_close;
} WBPreferences;

typedef struct {
	GtkWindowClass parent;
} WBPreferencesClass;

GType wb_preferences_get_type(void);
WBPreferences* wb_preferences_new(void);

void wb_preferences_set_from_config(WBPreferences *self, WBConfig *config);

#endif /* __BUTTONS_PREFERENCES_H__ */

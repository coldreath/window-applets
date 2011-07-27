#ifndef __TITLE_PREFERENCES_H__
#define __TITLE_PREFERENCES_H__

#include <gtk/gtk.h>

#include <title/title-config.h>

#define WT_TYPE_PREFERENCES				(wt_preferences_get_type())
#define WT_PREFERENCES(obj)				(G_TYPE_CHECK_INSTANCE_CAST((obj), WT_TYPE_PREFERENCES, WTPreferences))
#define WT_IS_PREFERENCES(obj)			(G_TYPE_CHECK_INSTANCE_TYPE((obj), WT_TYPE_PREFERENCES))
#define WT_PREFERENCES_CLASS(klass)		(G_TYPE_CHECK_CLASS_CAST((klass), WT_TYPE_PREFERENCES, WTPreferencesClass))
#define WT_IS_PREFERENCES_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE((klass), WT_TYPE_PREFERENCES))
#define WT_PREFERENCES_GET_CLASS(obj)	(G_TYPE_INSTANCE_GET_CLASS((obj), WT_TYPE_PREFERENCES, WTPreferencesClass))

typedef struct {
	GtkWindow parent;

	GtkBuilder *builder;
	GtkBox *main_box;
	GtkButton *btn_close;

	GtkToggleButton *chkb_only_maximized;
	GtkToggleButton *chkb_swap_order;
	GtkToggleButton *chkb_expand_title;
	GtkToggleButton *chkb_hide_icon;
	GtkToggleButton *chkb_hide_title;
	GtkToggleButton *chkb_custom_style;
	
	GtkFontButton *btn_font_active;
	GtkFontButton *btn_font_inactive;
	GtkColorButton *btn_color_active;
	GtkColorButton *btn_color_inactive;

	GtkScale *scale_alignment;
} WTPreferences;

typedef struct {
	GtkWindowClass parent;
} WTPreferencesClass;

GType wt_preferences_get_type(void);
WTPreferences* wt_preferences_new(void);

void wt_preferences_set_from_config(WTPreferences *self, WTConfig *config);

#endif /* __TITLE_PREFERENCES_H__ */

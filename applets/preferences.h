#ifndef __WIBUTI_PREFERENCES_H__
#define __WIBUTI_PREFERENCES_H__

#include <gtk/gtk.h>

#include "applets/configuration.h"

#define WIBUTI_TYPE_PREFS				(wibuti_prefs_get_type())
#define WIBUTI_PREFS(obj)				(G_TYPE_CHECK_INSTANCE_CAST((obj), WIBUTI_TYPE_PREFS, WibutiPrefs))
#define WIBUTI_IS_PREFS(obj)			(G_TYPE_CHECK_INSTANCE_TYPE((obj), WIBUTI_TYPE_PREFS))
#define WIBUTI_PREFS_CLASS(klass)		(G_TYPE_CHECK_CLASS_CAST((klass), WIBUTI_TYPE_PREFS, WibutiPrefsClass))
#define WIBUTI_IS_PREFS_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE((klass), WIBUTI_TYPE_PREFS))
#define WIBUTI_PREFS_GET_CLASS(obj)		(G_TYPE_INSTANCE_GET_CLASS((obj), WIBUTI_TYPE_PREFS, WibutiPrefsClass))

typedef struct {
	GtkWindow parent;

	GtkBuilder   *builder;

	// settings
	GtkButton       *btn_reload_layout;
	GtkEntry        *entry_layout;
	GtkToggleButton *chkb_only_maximized;
	GtkToggleButton *chkb_hide_unmaximized;

#ifdef WIBUTI_WITH_BUTTONS
	GtkComboBox     *combo_theme; 
	GtkToggleButton *chkb_hover_effect;
	GtkToggleButton *chkb_click_effect;
#endif // WIBUTI_WITH_BUTTONS

#ifdef WIBUTI_WITH_TITLE
	GtkToggleButton *chkb_expand_title;
	GtkToggleButton *chkb_custom_style;
	
	GtkFontButton  *btn_font_active;
	GtkFontButton  *btn_font_inactive;
	GtkColorButton *btn_color_active;
	GtkColorButton *btn_color_inactive;

	GtkScale *scale_alignment;
#endif // WIBUTI_WITH_TITLE
} WibutiPrefs;

typedef struct {
	GtkWindowClass parent;
} WibutiPrefsClass;

GType wibuti_prefs_get_type(void);
WibutiPrefs *wibuti_prefs_new(void);

void wibuti_prefs_set_from_config(WibutiPrefs *, WibutiConfig *);

#endif // __WIBUTI_PREFERENCES_H__

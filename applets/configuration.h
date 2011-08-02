#ifndef __WIBUTI_CONFIG_H__
#define __WIBUTI_CONFIG_H__

#include <glib.h>
#include <gtk/gtk.h>

#include "config.h"

#define CFG_ONLY_MAXIMIZED				"only_maximized"
#define CFG_HIDE_ON_UNMAXIMIZED			"hide_on_unmaximized"
#define CFG_LAYOUT						"layout"

#define CFG_CLICK_EFFECT				"click_effect"
#define CFG_HOVER_EFFECT				"hover_effect"
#define CFG_THEME						"theme"

#define CFG_EXPAND_TITLE				"expand_title"
#define CFG_CUSTOM_STYLE				"custom_style"
#define CFG_SHOW_WINDOW_MENU			"show_window_menu"
#define CFG_TITLE_ACTIVE_FONT			"title_active_font"
#define CFG_TITLE_ACTIVE_COLOR_FG		"title_active_color_fg"
#define CFG_TITLE_INACTIVE_FONT			"title_inactive_font"
#define CFG_TITLE_INACTIVE_COLOR_FG		"title_inactive_color_fg"
#define CFG_ALIGNMENT					"alignment"

#ifdef WIBUTI_WITH_BUTTONS

typedef enum {
	WIBUTI_BUTTON_MINIMIZE,
	WIBUTI_BUTTON_MAXIMIZE,
	WIBUTI_BUTTON_RESTORE,
	WIBUTI_BUTTON_CLOSE,
} WibutiButton;

#define WIBUTI_BUTTON_FIRST		WIBUTI_BUTTON_MINIMIZE
#define WIBUTI_BUTTON_LAST		WIBUTI_BUTTON_CLOSE
#define WIBUTI_BUTTON_NUM		(WIBUTI_BUTTON_LAST+1)

typedef enum {
	WIBUTI_STATE_NORMAL,
	WIBUTI_STATE_HOVER,
	WIBUTI_STATE_CLICKED,
} WibutiState;

#define WIBUTI_STATE_FIRST		WIBUTI_STATE_NORMAL
#define WIBUTI_STATE_LAST		WIBUTI_STATE_CLICKED
#define WIBUTI_STATE_NUM		(WIBUTI_STATE_CLICKED+1)

typedef enum {
	WIBUTI_FOCUS_FOCUSED,
	WIBUTI_FOCUS_UNFOCUSED,
} WibutiFocus;

#define WIBUTI_FOCUS_FIRST		WIBUTI_FOCUS_FOCUSED
#define WIBUTI_FOCUS_LAST		WIBUTI_FOCUS_UNFOCUSED
#define WIBUTI_FOCUS_NUM		(WIBUTI_FOCUS_UNFOCUSED+1)

#endif // WIBUTI_WITH_BUTTONS

typedef struct {
	gboolean  only_maximized;		// [T/F] Only track maximized windows
	gboolean  hide_on_unmaximized;	// [T/F] Hide when no maximized windows present
	const gchar *layout;

#ifdef WIBUTI_WITH_BUTTONS
	gboolean  click_effect;
	gboolean  hover_effect;
	gchar    *theme;
#endif // WIBUTI_WITH_BUTTONS

#ifdef WIBUTI_WITH_TITLE
	gboolean  expand_title;			// [T/F] Expand the title
	gboolean  custom_style;			// [T/F] Use custom style
	gboolean  show_window_menu;		// [T/F] Show window action menu on right click
	gchar    *title_active_font;	// Custom active title font
	gchar    *title_active_color;	// Custom active title color
	gchar    *title_inactive_font;	// Custom inactive title font
	gchar    *title_inactive_color;	// Custom inactive title color
	gdouble   alignment;			// Title alignment [0=left, 0.5=center, 1=right]
#endif // WIBUTI_WITH_TITLE
} WibutiConfig;

void wibuti_config_load_defaults(WibutiConfig *self);

void wibuti_config_load_plain(WibutiConfig *self);
void wibuti_config_save_plain(WibutiConfig *self);
// TODO
//void wibuti_config_load_gconf(WibutiConfig *self);
//void wibuti_config_save_gconf(WibutiConfig *self);

#ifdef WIBUTI_WITH_BUTTONS
GdkPixbuf ****wibuti_config_get_buttons(WibutiConfig *self);
#endif // WIBUTI_WITH_BUTTONS

#endif // __WIBUTI_CONFIG_H__

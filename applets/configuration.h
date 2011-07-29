#ifndef __WIBUTI_CONFIG_H__
#define __WIBUTI_CONFIG_H__

#include <glib.h>

#include "config.h"

#define CFG_ONLY_MAXIMIZED				"only_maximized"
#define CFG_HIDE_ON_UNMAXIMIZED 		"hide_on_unmaximized"
#define CFG_HIDE_ICON					"hide_icon"
#define CFG_HIDE_TITLE					"hide_title"
#define CFG_SWAP_ORDER					"swap_order"
#define CFG_EXPAND_TITLE				"expand_title"
#define CFG_CUSTOM_STYLE				"custom_style"
#define CFG_SHOW_WINDOW_MENU			"show_window_menu"
#define CFG_TITLE_ACTIVE_FONT			"title_active_font"
#define CFG_TITLE_ACTIVE_COLOR_FG		"title_active_color_fg"
#define CFG_TITLE_INACTIVE_FONT			"title_inactive_font"
#define CFG_TITLE_INACTIVE_COLOR_FG		"title_inactive_color_fg"
#define CFG_ALIGNMENT					"alignment"

typedef struct {
	gboolean	only_maximized;			// [T/F] Only track maximized windows

#ifdef WIBUTI_WITH_BUTTONS

#endif // WIBUTI_WITH_BUTTONS
#ifdef WIBUTI_WITH_TITLE
	gboolean	hide_on_unmaximized,	// [T/F] Hide when no maximized windows present
				hide_icon,				// [T/F] Hide the icon
				hide_title,				// [T/F] Hide the title
				swap_order,				// [T/F] Swap title/icon
				expand_title,			// [T/F] Expand the title
				custom_style,			// [T/F] Use custom style
				show_window_menu;		// [T/F] Show window action menu on right click
	gchar		*title_active_font;		// Custom active title font
	gchar		*title_active_color;	// Custom active title color
	gchar		*title_inactive_font;	// Custom inactive title font
	gchar		*title_inactive_color;	// Custom inactive title color
	gdouble		alignment;				// Title alignment [0=left, 0.5=center, 1=right]
#endif // WIBUTI_WITH_TITLE
} WibutiConfig;

void wibuti_config_load_defaults(WibutiConfig *self);

void wibuti_config_load_plain(WibutiConfig *self);
void wibuti_config_save_plain(WibutiConfig *self);
// TODO
//void wibuti_config_load_gconf(WibutiConfig *self);
//void wibuti_config_save_gconf(WibutiConfig *self);

#endif // __WIBUTI_CONFIG_H__

#include <glib/gstdio.h>

#include "applets/configuration.h"
#include "common/tools.h"

#define FILE_BUTTONS_CONFIG	"/.config/windowbuttons.conf"
#define FILE_TITLE_CONFIG	"/.config/windowtitle.conf"

void wibuti_config_load_defaults(WibutiConfig *self) {
	self->only_maximized = TRUE;
#ifdef WIBUTI_WITH_BUTTONS
#endif // WIBUTI_WITH_BUTTONS
#ifdef WIBUTI_WITH_TITLE
	self->hide_on_unmaximized = FALSE;
	self->hide_icon = FALSE;
	self->hide_title = FALSE;
	self->alignment = 0.5;
	self->swap_order = FALSE;
	self->expand_title = TRUE;
	self->custom_style = FALSE;
	self->show_window_menu = FALSE;
	self->title_active_font = "Sans 10";
	self->title_active_color = "#FFFFFF";
	self->title_inactive_font = "Sans 10";
	self->title_inactive_color = "#808080";
#endif // WIBUTI_WITH_TITLE
}

void wibuti_config_load_plain(WibutiConfig *self) {
#ifdef WIBUTI_WITH_BUTTONS
#endif // WIBUTI_WITH_BUTTONS
#ifdef WIBUTI_WITH_TITLE
	FILE *cfg = g_fopen(g_strconcat(g_get_home_dir(), FILE_TITLE_CONFIG, NULL), "r");

	if (cfg) {
		self->only_maximized = g_ascii_strtod(get_cfg_value(cfg, CFG_ONLY_MAXIMIZED), NULL);
		self->hide_on_unmaximized = g_ascii_strtod(get_cfg_value(cfg, CFG_HIDE_ON_UNMAXIMIZED), NULL);
		self->hide_icon = g_ascii_strtod(get_cfg_value(cfg, CFG_HIDE_ICON), NULL);
		self->hide_title = g_ascii_strtod(get_cfg_value(cfg, CFG_HIDE_TITLE), NULL);
		self->alignment = g_ascii_strtod(get_cfg_value(cfg, CFG_ALIGNMENT), NULL);
		self->swap_order = g_ascii_strtod(get_cfg_value(cfg, CFG_SWAP_ORDER), NULL);
		self->expand_title = g_ascii_strtod(get_cfg_value(cfg, CFG_EXPAND_TITLE), NULL);
		self->custom_style = g_ascii_strtod(get_cfg_value(cfg, CFG_CUSTOM_STYLE), NULL);
		self->show_window_menu = g_ascii_strtod(get_cfg_value(cfg, CFG_SHOW_WINDOW_MENU), NULL);
		self->title_active_font = get_cfg_value(cfg, CFG_TITLE_ACTIVE_FONT);
		self->title_active_color = get_cfg_value(cfg, CFG_TITLE_ACTIVE_COLOR_FG);
		self->title_inactive_font = get_cfg_value(cfg, CFG_TITLE_INACTIVE_FONT);
		self->title_inactive_color = get_cfg_value(cfg, CFG_TITLE_INACTIVE_COLOR_FG);
		
		fclose(cfg);
	}
#endif // WIBUTI_WITH_TITLE
}

void wibuti_config_save_plain(WibutiConfig *self) {
#ifdef WIBUTI_WITH_BUTTONS
#endif // WIBUTI_WITH_BUTTONS
#ifdef WIBUTI_WITH_TITLE
	FILE *cfg = g_fopen(g_strconcat(g_get_home_dir(), FILE_TITLE_CONFIG, NULL), "w");
	char buf[10];
	
	if (cfg) {
		fprintf(cfg, "%s %d\n", CFG_ONLY_MAXIMIZED, self->only_maximized);
		fprintf(cfg, "%s %d\n", CFG_HIDE_ON_UNMAXIMIZED, self->hide_on_unmaximized);
		fprintf(cfg, "%s %d\n", CFG_HIDE_ICON, self->hide_icon);
		fprintf(cfg, "%s %d\n", CFG_HIDE_TITLE, self->hide_title);
		fprintf(cfg, "%s %d\n", CFG_SWAP_ORDER, self->swap_order);
		fprintf(cfg, "%s %d\n", CFG_EXPAND_TITLE, self->expand_title);
		fprintf(cfg, "%s %d\n", CFG_CUSTOM_STYLE, self->custom_style);
		fprintf(cfg, "%s %d\n", CFG_SHOW_WINDOW_MENU, self->show_window_menu);
		fprintf(cfg, "%s %s\n", CFG_ALIGNMENT, g_ascii_dtostr(buf, sizeof(buf), self->alignment));
		fprintf(cfg, "%s %s\n", CFG_TITLE_ACTIVE_FONT, self->title_active_font);
		fprintf(cfg, "%s %s\n", CFG_TITLE_ACTIVE_COLOR_FG, self->title_active_color);
		fprintf(cfg, "%s %s\n", CFG_TITLE_INACTIVE_FONT, self->title_inactive_font);
		fprintf(cfg, "%s %s\n", CFG_TITLE_INACTIVE_COLOR_FG, self->title_inactive_color);

		fclose(cfg);
	}
#endif // WIBUTI_WITH_TITLE
}


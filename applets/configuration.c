#include <string.h>
#include <glib/gstdio.h>

#include "applets/configuration.h"

#define FILE_BUTTONS_CONFIG	"/.config/windowbuttons.conf"
#define FILE_TITLE_CONFIG	"/.config/windowtitle.conf"

static gchar* wibuti_config_get_value(FILE *f, gchar *key);


/**********************************************************************************************************************/


void wibuti_config_load_defaults(WibutiConfig *self) {
	self->only_maximized = TRUE;
	self->hide_on_unmaximized = FALSE;
	self->layout = "ITMRX";

#ifdef WIBUTI_WITH_BUTTONS
	self->click_effect = FALSE;
	self->hover_effect = TRUE;
	self->theme = "default";
#endif // WIBUTI_WITH_BUTTONS

#ifdef WIBUTI_WITH_TITLE
	self->expand_title = TRUE;
	self->custom_style = FALSE;
	self->show_window_menu = FALSE;
	self->title_active_font = "Sans 10";
	self->title_active_color = "#FFFFFF";
	self->title_inactive_font = "Sans 10";
	self->title_inactive_color = "#808080";
	self->alignment = 0.5;
#endif // WIBUTI_WITH_TITLE
}

void wibuti_config_load_plain(WibutiConfig *self) {
#ifdef WIBUTI_WITH_BUTTONS
	FILE *bcfg = g_fopen(g_strconcat(g_get_home_dir(), FILE_BUTTONS_CONFIG, NULL), "r");

	if (bcfg) {
		self->only_maximized = g_ascii_strtod(wibuti_config_get_value(bcfg, CFG_ONLY_MAXIMIZED), NULL);
		self->hide_on_unmaximized = g_ascii_strtod(wibuti_config_get_value(bcfg, CFG_HIDE_ON_UNMAXIMIZED), NULL);
		self->layout = wibuti_config_get_value(bcfg, CFG_LAYOUT);
		self->click_effect = g_ascii_strtod(wibuti_config_get_value(bcfg, CFG_CLICK_EFFECT), NULL);
		self->hover_effect = g_ascii_strtod(wibuti_config_get_value(bcfg, CFG_HOVER_EFFECT), NULL);
		self->theme = wibuti_config_get_value(bcfg, CFG_THEME);
		
		fclose(bcfg);
	}
#endif // WIBUTI_WITH_BUTTONS

#ifdef WIBUTI_WITH_TITLE
	FILE *tcfg = g_fopen(g_strconcat(g_get_home_dir(), FILE_TITLE_CONFIG, NULL), "r");

	if (tcfg) {
		self->only_maximized = g_ascii_strtod(wibuti_config_get_value(tcfg, CFG_ONLY_MAXIMIZED), NULL);
		self->hide_on_unmaximized = g_ascii_strtod(wibuti_config_get_value(tcfg, CFG_HIDE_ON_UNMAXIMIZED), NULL);
		self->layout = wibuti_config_get_value(tcfg, CFG_LAYOUT);
		self->expand_title = g_ascii_strtod(wibuti_config_get_value(tcfg, CFG_EXPAND_TITLE), NULL);
		self->custom_style = g_ascii_strtod(wibuti_config_get_value(tcfg, CFG_CUSTOM_STYLE), NULL);
		self->show_window_menu = g_ascii_strtod(wibuti_config_get_value(tcfg, CFG_SHOW_WINDOW_MENU), NULL);
		self->title_active_font = wibuti_config_get_value(tcfg, CFG_TITLE_ACTIVE_FONT);
		self->title_active_color = wibuti_config_get_value(tcfg, CFG_TITLE_ACTIVE_COLOR_FG);
		self->title_inactive_font = wibuti_config_get_value(tcfg, CFG_TITLE_INACTIVE_FONT);
		self->title_inactive_color = wibuti_config_get_value(tcfg, CFG_TITLE_INACTIVE_COLOR_FG);
		self->alignment = g_ascii_strtod(wibuti_config_get_value(tcfg, CFG_ALIGNMENT), NULL);
		
		fclose(tcfg);
	}
#endif // WIBUTI_WITH_TITLE
}

void wibuti_config_save_plain(WibutiConfig *self) {
#ifdef WIBUTI_WITH_BUTTONS
	FILE *bcfg = g_fopen(g_strconcat(g_get_home_dir(), FILE_BUTTONS_CONFIG, NULL), "w");
	
	if (bcfg) {
		fprintf(bcfg, "%s %d\n", CFG_ONLY_MAXIMIZED, self->only_maximized);
		fprintf(bcfg, "%s %d\n", CFG_HIDE_ON_UNMAXIMIZED, self->hide_on_unmaximized);
		fprintf(bcfg, "%s %s\n", CFG_LAYOUT, self->layout);
		fprintf(bcfg, "%s %d\n", CFG_CLICK_EFFECT, self->click_effect);
		fprintf(bcfg, "%s %d\n", CFG_HOVER_EFFECT, self->hover_effect);
		fprintf(bcfg, "%s %s\n", CFG_THEME, self->theme);

		fclose(bcfg);
	}
#endif // WIBUTI_WITH_BUTTONS

#ifdef WIBUTI_WITH_TITLE
	FILE *tcfg = g_fopen(g_strconcat(g_get_home_dir(), FILE_TITLE_CONFIG, NULL), "w");
	char buf[10];
	
	if (tcfg) {
		fprintf(tcfg, "%s %d\n", CFG_ONLY_MAXIMIZED, self->only_maximized);
		fprintf(tcfg, "%s %d\n", CFG_HIDE_ON_UNMAXIMIZED, self->hide_on_unmaximized);
		fprintf(tcfg, "%s %s\n", CFG_LAYOUT, self->layout);
		fprintf(tcfg, "%s %d\n", CFG_EXPAND_TITLE, self->expand_title);
		fprintf(tcfg, "%s %d\n", CFG_CUSTOM_STYLE, self->custom_style);
		fprintf(tcfg, "%s %d\n", CFG_SHOW_WINDOW_MENU, self->show_window_menu);
		fprintf(tcfg, "%s %s\n", CFG_TITLE_ACTIVE_FONT, self->title_active_font);
		fprintf(tcfg, "%s %s\n", CFG_TITLE_ACTIVE_COLOR_FG, self->title_active_color);
		fprintf(tcfg, "%s %s\n", CFG_TITLE_INACTIVE_FONT, self->title_inactive_font);
		fprintf(tcfg, "%s %s\n", CFG_TITLE_INACTIVE_COLOR_FG, self->title_inactive_color);
		fprintf(tcfg, "%s %s\n", CFG_ALIGNMENT, g_ascii_dtostr(buf, sizeof(buf), self->alignment));

		fclose(tcfg);
	}
#endif // WIBUTI_WITH_TITLE
}


/**********************************************************************************************************************/


/* 
 * Returns a string value of the specified configuration parameter (key)
 */
static gchar* wibuti_config_get_value(FILE *f, gchar *key) {
	gchar tmp[256] = {0x0};
	long int pos = ftell(f);
	
	while (f != NULL && fgets(tmp, sizeof(tmp), f) != NULL) {
		if (g_strrstr(tmp, key)) {
			break;
		}
	}

	gchar *r = g_strndup(tmp + strlen(key) + 1, strlen(tmp) - strlen(key) + 1);
	g_strstrip(r);

	fseek(f, pos, SEEK_SET);
	return r;
}

#ifdef WIBUTI_WITH_BUTTONS
GdkPixbuf ****wibuti_config_get_buttons(WibutiConfig *self) {
	GError *error = NULL;
	GdkPixbuf ****pixbufs = g_new(GdkPixbuf ***, WIBUTI_FOCUS_NUM);

	gchar *buttons[] = {"minimize", "maximize", "unmaximize", "close"};
	gchar *states[] = {"normal", "hover", "clicked"};
	gchar *focus[] = {"focused", "unfocused"};

	gchar *name;
	int f, s, b;
	for (f = WIBUTI_FOCUS_FIRST; f < WIBUTI_FOCUS_NUM; ++f) {
		pixbufs[f] = g_new(GdkPixbuf **, WIBUTI_STATE_NUM);
		for (s = WIBUTI_STATE_FIRST; s < WIBUTI_STATE_NUM; ++s) {
			pixbufs[f][s] = g_new(GdkPixbuf *, WIBUTI_BUTTON_NUM);
			for (b = WIBUTI_BUTTON_FIRST; b < WIBUTI_BUTTON_NUM; ++b) {
				name = g_strconcat(WIBUTI_PATH_THEMES, "/", self->theme, "/", buttons[b],
				                   "-", focus[f], "-", states[s], ".png", NULL);
				pixbufs[f][s][b] = gdk_pixbuf_new_from_file(name, &error);
				if (error) {
					pixbufs[f][s][b] = NULL;
					printf("Error loading image \"%s\": %s\n", name, error->message);
				}
			}
		}
	}
	
	return pixbufs;
}
#endif // WIBUTI_WITH_BUTTONS



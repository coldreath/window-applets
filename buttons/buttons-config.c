#include <glib/gstdio.h>

#include <buttons/buttons-config.h>
#include <common/tools.h>

#define FILE_CONFIGFILE		"/.config/windowbuttons.conf"

void wb_config_load_defaults(WBConfig *self) {
}

void wb_config_load_plain(WBConfig *self) {
}

void wb_config_save_plain(WBConfig *self) {
	FILE *cfg = g_fopen(g_strconcat(g_get_home_dir(), FILE_CONFIGFILE, NULL), "w");
	
	if (cfg) {
		fclose (cfg);
	}
}



#ifndef __BUTTONS_CONFIG_H__
#define __BUTTONS_CONFIG_H__

#include <glib.h>

typedef struct {
} WBConfig;

void wb_config_load_defaults(WBConfig *self);

void wb_config_load_plain(WBConfig *self);
void wb_config_save_plain(WBConfig *self);
// TODO
//void wb_config_load_gconf(WBConfig *self);
//void wb_config_save_gconf(WBConfig *self);

#endif /* __BUTTONS_CONFIG_H__ */

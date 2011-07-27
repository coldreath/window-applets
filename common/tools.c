#include <string.h>

#include <common/tools.h>

/* 
 * Returns a string value of the specified configuration parameter (key)
 */
gchar* get_cfg_value(FILE *f, gchar *key) {
    gchar tmp[256] = {0x0};
	long int pos = ftell(f);
	
    while (f != NULL && fgets(tmp, sizeof(tmp), f) != NULL) {
		if (g_strrstr(tmp, key))
		    break;
    }

	gchar *r = g_strndup(tmp + strlen(key) + 1, strlen(tmp) - strlen(key) + 1);
	g_strstrip(r);

	fseek(f, pos, SEEK_SET);
    return r;
}


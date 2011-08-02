#ifndef __WIBUTI_WATCHER_H__
#define __WIBUTI_WATCHER_H__

#ifndef WNCK_I_KNOW_THIS_IS_UNSTABLE
#define WNCK_I_KNOW_THIS_IS_UNSTABLE
#endif
#include <libwnck/libwnck.h>

#include "config.h"

#define WIBUTI_TYPE_WATCHER				(wibuti_watcher_get_type())
#define WIBUTI_WATCHER(obj)				(G_TYPE_CHECK_INSTANCE_CAST((obj), WIBUTI_TYPE_WATCHER, WibutiWatcher))
#define WIBUTI_IS_WATCHER(obj)			(G_TYPE_CHECK_INSTANCE_TYPE((obj), WIBUTI_TYPE_WATCHER))
#define WIBUTI_WATCHER_CLASS(klass)		(G_TYPE_CHECK_CLASS_CAST((klass), WIBUTI_TYPE_WATCHER, WibutiWatcherClass))
#define WIBUTI_IS_WATCHER_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE((klass), WIBUTI_TYPE_WATCHER))
#define WIBUTI_WATCHER_GET_CLASS(obj)	(G_TYPE_INSTANCE_GET_CLASS((obj), WIBUTI_TYPE_WATCHER, WibutiWatcherClass))

typedef struct {
	GObject parent;

	WnckWindow *tracked;
	WnckWindow *active;
	WnckScreen *screen;
	
	gulong handler_tracked_state_changed;
	gulong handler_active_state_changed;

#ifdef WIBUTI_WITH_TITLE
	gulong handler_name_changed;
	gulong handler_icon_changed;
#endif // WIBUTI_WITH_TITLE

	gboolean only_maximized;
} WibutiWatcher;

typedef struct {
	GObjectClass parent;
} WibutiWatcherClass;

GType wibuti_watcher_get_type(void);
WibutiWatcher* wibuti_watcher_new(void);

void wibuti_watcher_set_only_maximized(WibutiWatcher *, gboolean);
gboolean wibuti_watcher_is_active(WibutiWatcher *);

#ifdef WIBUTI_WITH_BUTTONS
void wibuti_watcher_minimize(WibutiWatcher *);
void wibuti_watcher_maximize_restore(WibutiWatcher *);
void wibuti_watcher_close(WibutiWatcher *);
gboolean wibuti_watcher_is_maximized(WibutiWatcher *);
#endif // WIBUTI_WITH_BUTTONS

#ifdef WIBUTI_WITH_TITLE
const gchar *wibuti_watcher_get_title(WibutiWatcher *);
GdkPixbuf *wibuti_watcher_get_icon(WibutiWatcher *);
#endif // WIBUTI_WITH_TITLE

#endif // __WIBUTI_WATCHER_H__

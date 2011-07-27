#ifndef __WATCHER_H__
#define __WATCHER_H__

#ifndef WNCK_I_KNOW_THIS_IS_UNSTABLE
#define WNCK_I_KNOW_THIS_IS_UNSTABLE
#endif
#include <libwnck/libwnck.h>

#define TYPE_WATCHER			(watcher_get_type())
#define WATCHER(obj)			(G_TYPE_CHECK_INSTANCE_CAST((obj), TYPE_WATCHER, Watcher))
#define IS_WATCHER(obj)			(G_TYPE_CHECK_INSTANCE_TYPE((obj), TYPE_WATCHER))
#define WATCHER_CLASS(klass)	(G_TYPE_CHECK_CLASS_CAST((klass), TYPE_WATCHER, WatcherClass))
#define IS_WATCHER_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE((klass), TYPE_WATCHER))
#define WATCHER_GET_CLASS(obj)	(G_TYPE_INSTANCE_GET_CLASS((obj), TYPE_WATCHER, WatcherClass))

typedef struct {
	GObject parent;

	WnckWindow *tracked;
	WnckWindow *active;
	WnckScreen *screen;
	
	gulong handler_name_changed;
	gulong handler_icon_changed;
	gulong handler_tracked_state_changed;
	gulong handler_active_state_changed;

	gboolean only_maximized;
} Watcher;

typedef struct {
	GObjectClass parent;
} WatcherClass;

GType watcher_get_type(void);
Watcher* watcher_new(void);

void watcher_set_only_maximized(Watcher *self, gboolean only_maximized);
const gchar *watcher_get_title(Watcher *self);
GdkPixbuf *watcher_get_icon(Watcher *self);
gboolean watcher_is_active(Watcher *self);

#endif /* __WATCHER_H__ */

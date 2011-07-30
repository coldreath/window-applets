#include "applets/watcher.h"

#define WATCHER_SIGNAL_WINDOW_CHANGED	"window-changed"
#define WATCHER_SIGNAL_NAME_CHANGED		"name-changed"
#define WATCHER_SIGNAL_ICON_CHANGED		"icon-changed"

static void wibuti_watcher_find_window(WibutiWatcher *self);

// callbacks
static void wibuti_watcher_active_workspace_changed_cb(WnckScreen *screen, WnckWorkspace *workspace,
                                                       WibutiWatcher *self);
static void wibuti_watcher_active_window_changed_cb(WnckScreen *screen, WnckWindow *window, WibutiWatcher *self);
static void wibuti_watcher_viewports_changed_cb(WnckScreen *screen, WibutiWatcher *self);
static void wibuti_watcher_window_closed_cb(WnckScreen *screen, WnckWindow *window, WibutiWatcher *self);
static void wibuti_watcher_window_opened_cb(WnckScreen *screen, WnckWindow *window, WibutiWatcher *self);
static void wibuti_watcher_window_state_changed_cb(WnckWindow *window, WnckWindowState changed_mask, 
                                                   WnckWindowState new_state, WibutiWatcher *self);
static void wibuti_watcher_window_changed_emit(WibutiWatcher *self);
static void wibuti_watcher_name_changed_cb(WnckScreen *screen, WibutiWatcher *self);
static void wibuti_watcher_icon_changed_cb(WnckScreen *screen, WibutiWatcher *self);


/**********************************************************************************************************************/


G_DEFINE_TYPE(WibutiWatcher, wibuti_watcher, G_TYPE_OBJECT);

static void wibuti_watcher_class_init(WibutiWatcherClass *klass) {
	g_signal_newv(WATCHER_SIGNAL_WINDOW_CHANGED, WIBUTI_TYPE_WATCHER, G_SIGNAL_RUN_LAST,
				NULL, NULL, NULL, g_cclosure_marshal_VOID__VOID, G_TYPE_NONE, 0, NULL);
	g_signal_newv(WATCHER_SIGNAL_NAME_CHANGED, WIBUTI_TYPE_WATCHER, G_SIGNAL_RUN_LAST,
				NULL, NULL, NULL, g_cclosure_marshal_VOID__VOID, G_TYPE_NONE, 0, NULL);
	g_signal_newv(WATCHER_SIGNAL_ICON_CHANGED, WIBUTI_TYPE_WATCHER, G_SIGNAL_RUN_LAST,
				NULL, NULL, NULL, g_cclosure_marshal_VOID__VOID, G_TYPE_NONE, 0, NULL);
}

static void wibuti_watcher_init(WibutiWatcher *self) {
	self->only_maximized = FALSE;
	self->tracked = NULL;
	self->screen = wnck_screen_get_default();
	wnck_screen_force_update(self->screen);

	wibuti_watcher_find_window(self);
	
	g_signal_connect(G_OBJECT(self->screen), "active-workspace-changed",
	                 G_CALLBACK(wibuti_watcher_active_workspace_changed_cb), self);
	g_signal_connect(G_OBJECT(self->screen), "active-window-changed",
	                 G_CALLBACK(wibuti_watcher_active_window_changed_cb), self);
	g_signal_connect(G_OBJECT(self->screen), "viewports-changed",
	                 G_CALLBACK(wibuti_watcher_viewports_changed_cb), self);
	g_signal_connect(G_OBJECT(self->screen), "window-closed",
	                 G_CALLBACK(wibuti_watcher_window_closed_cb), self);
	g_signal_connect(G_OBJECT(self->screen), "window-opened",
	                 G_CALLBACK(wibuti_watcher_window_opened_cb), self);
	
	g_signal_emit_by_name(self, WATCHER_SIGNAL_WINDOW_CHANGED);
}

WibutiWatcher* wibuti_watcher_new(void) {
	return WIBUTI_WATCHER(g_object_new(WIBUTI_TYPE_WATCHER, NULL));
}


/**********************************************************************************************************************/


static void wibuti_watcher_find_window(WibutiWatcher *self) {
	WnckWorkspace *activeworkspace = wnck_screen_get_active_workspace(self->screen);
	WnckWindow *new_active = wnck_screen_get_active_window(self->screen);
	WnckWindow *new_tracked = NULL;

	// stop tracking the old active window
	if (G_IS_OBJECT(self->active)) {
		if (g_signal_handler_is_connected(G_OBJECT(self->active), self->handler_active_state_changed)) {
			g_signal_handler_disconnect(G_OBJECT(self->active), self->handler_active_state_changed);
		}
	}

	if (self->only_maximized) {
		// find upper maximized window
		GList *windows = wnck_screen_get_windows_stacked(self->screen);
		while (windows && windows->data) {
			if (!self->only_maximized || wnck_window_is_maximized(windows->data))
				if (!wnck_window_is_minimized(windows->data) && !wnck_window_is_skip_pager(windows->data))
					if (wnck_window_is_in_viewport(windows->data, activeworkspace))
						new_tracked = windows->data;
			windows = windows->next;
		}

		// start tracking the new active window
		if (new_active) {
			self->handler_active_state_changed = g_signal_connect(G_OBJECT(new_active), "state-changed",
			                                     G_CALLBACK(wibuti_watcher_window_state_changed_cb), self);
		}
	} else {
		new_tracked = new_active;
	}

	// stop tracking the old window
	if (G_IS_OBJECT(self->tracked)) {
		if (g_signal_handler_is_connected(G_OBJECT(self->tracked), self->handler_name_changed)) {
			g_signal_handler_disconnect(G_OBJECT(self->tracked), self->handler_name_changed);
		}
		if (g_signal_handler_is_connected(G_OBJECT(self->tracked), self->handler_icon_changed)) {
			g_signal_handler_disconnect(G_OBJECT(self->tracked), self->handler_icon_changed);
		}
		if (g_signal_handler_is_connected(G_OBJECT(self->tracked), self->handler_tracked_state_changed)) {
			g_signal_handler_disconnect(G_OBJECT(self->tracked), self->handler_tracked_state_changed);
		}
	}

	// start tracking the new window
	if (new_tracked) {
		self->handler_name_changed = g_signal_connect(G_OBJECT(new_tracked), "name-changed",
	                                                  G_CALLBACK(wibuti_watcher_name_changed_cb), self);
		self->handler_icon_changed = g_signal_connect(G_OBJECT(new_tracked), "icon-changed",
	                                                  G_CALLBACK(wibuti_watcher_icon_changed_cb), self);
		self->handler_tracked_state_changed = g_signal_connect(G_OBJECT(new_tracked), "state-changed",
                                                      G_CALLBACK(wibuti_watcher_window_state_changed_cb), self);
	}
	
	self->tracked = new_tracked;
	self->active = new_active;
}


/**********************************************************************************************************************/


static void wibuti_watcher_active_workspace_changed_cb(WnckScreen *screen, WnckWorkspace *workspace, WibutiWatcher *self) {
	wibuti_watcher_window_changed_emit(self);
}

static void wibuti_watcher_active_window_changed_cb(WnckScreen *screen, WnckWindow *window, WibutiWatcher *self) {
	wibuti_watcher_window_changed_emit(self);
}

static void wibuti_watcher_viewports_changed_cb(WnckScreen *screen, WibutiWatcher *self) {
	wibuti_watcher_window_changed_emit(self);
}

static void wibuti_watcher_window_closed_cb(WnckScreen *screen, WnckWindow *window, WibutiWatcher *self) {
	wibuti_watcher_window_changed_emit(self);
}

static void wibuti_watcher_window_opened_cb(WnckScreen *screen, WnckWindow *window, WibutiWatcher *self) {
	wibuti_watcher_window_changed_emit(self);
}

static void wibuti_watcher_window_state_changed_cb(WnckWindow *window, WnckWindowState changed_mask, 
                                            WnckWindowState new_state, WibutiWatcher *self) {
	wibuti_watcher_window_changed_emit(self);
}

static void wibuti_watcher_window_changed_emit(WibutiWatcher *self) {
	wibuti_watcher_find_window(self);
	g_signal_emit_by_name(self, WATCHER_SIGNAL_WINDOW_CHANGED);
}

static void wibuti_watcher_name_changed_cb(WnckScreen *screen, WibutiWatcher *self) {
	g_signal_emit_by_name(self, WATCHER_SIGNAL_NAME_CHANGED);
}

static void wibuti_watcher_icon_changed_cb(WnckScreen *screen, WibutiWatcher *self) {
	g_signal_emit_by_name(self, WATCHER_SIGNAL_ICON_CHANGED);
}


/**********************************************************************************************************************/


void wibuti_watcher_set_only_maximized(WibutiWatcher *self, gboolean only_maximized) {
	self->only_maximized = only_maximized;
	wibuti_watcher_find_window(self);
}

gboolean wibuti_watcher_is_active(WibutiWatcher *self) {
	return self->active == self->tracked;
}

#ifdef WIBUTI_WITH_BUTTONS
#endif // WIBUTI_WITH_BUTTONS
#ifdef WIBUTI_WITH_TITLE

const gchar *wibuti_watcher_get_title(WibutiWatcher *self) {
	if (WNCK_IS_WINDOW(self->tracked)) {
		return wnck_window_get_name(self->tracked);
	} else {
		return "Desktop";
	}
}

GdkPixbuf *wibuti_watcher_get_icon(WibutiWatcher *self) {
	if (WNCK_IS_WINDOW(self->tracked)) {
		return wnck_window_get_icon(self->tracked);
	} else {
		return NULL;
	}
}

#endif // WIBUTI_WITH_TITLE


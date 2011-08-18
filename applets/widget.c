#include "applets/widget.h"

#define ICON_WIDTH	16
#define ICON_HEIGHT	16

#define WIBUTI_SIGNAL_MINIMIZE	"minimize-clicked"
#define WIBUTI_SIGNAL_MAXIMIZE	"maximize-clicked"
#define WIBUTI_SIGNAL_RESTORE	"restore-clicked"
#define WIBUTI_SIGNAL_CLOSE		"close-clicked"

#ifdef WIBUTI_WITH_BUTTONS
static void wibuti_widget_release_cb(GtkEventBox *, GdkEvent *, WibutiWidget *);
static void wibuti_widget_press_cb(GtkEventBox *, GdkEvent *, WibutiWidget *);
static void wibuti_widget_enter_cb(GtkEventBox *, GdkEvent *, WibutiWidget *);
static void wibuti_widget_leave_cb(GtkEventBox *, GdkEvent *, WibutiWidget *);

static WibutiButton wibuti_widget_get_button_num(WibutiWidget *, GtkEventBox *);
static WibutiFocus wibuti_widget_get_draw_active(WibutiWidget *);
#endif // WIBUTI_WITH_BUTTONS


G_DEFINE_TYPE(WibutiWidget, wibuti_widget, GTK_TYPE_EVENT_BOX);

static void wibuti_widget_init(WibutiWidget *self) {
	self->box = GTK_BOX(gtk_hbox_new(FALSE, 0));
	
#ifdef WIBUTI_WITH_BUTTONS
	self->evbox = g_new(GtkEventBox *, WIBUTI_BUTTON_NUM);
	self->image = g_new(GtkImage *, WIBUTI_BUTTON_NUM);

	int i;
	for (i = WIBUTI_BUTTON_FIRST; i < WIBUTI_BUTTON_NUM; ++i) {
		self->evbox[i] = GTK_EVENT_BOX(gtk_event_box_new());
		self->image[i] = GTK_IMAGE(gtk_image_new());

		gtk_container_add(GTK_CONTAINER(self->evbox[i]), GTK_WIDGET(self->image[i]));
	
		g_signal_connect(G_OBJECT(self->evbox[i]), "button-release-event", G_CALLBACK(wibuti_widget_release_cb), self);
		g_signal_connect(G_OBJECT(self->evbox[i]), "button-press-event", G_CALLBACK(wibuti_widget_press_cb), self);
		g_signal_connect(G_OBJECT(self->evbox[i]), "enter-notify-event", G_CALLBACK(wibuti_widget_enter_cb), self);
		g_signal_connect(G_OBJECT(self->evbox[i]), "leave-notify-event", G_CALLBACK(wibuti_widget_leave_cb), self);	
	}
	
	self->use_close = self->use_maximize = self->use_minimize = FALSE;
	self->buttons = NULL;
#endif // WIBUTI_WITH_BUTTONS

#ifdef WIBUTI_WITH_TITLE
	self->evbox_icon = GTK_EVENT_BOX(gtk_event_box_new());
	self->evbox_title = GTK_EVENT_BOX(gtk_event_box_new());
	self->image_icon = GTK_IMAGE(gtk_image_new());
	self->label_title = GTK_LABEL(gtk_label_new(NULL));
	
	gtk_container_add(GTK_CONTAINER(self->evbox_icon), GTK_WIDGET(self->image_icon));
	gtk_container_add(GTK_CONTAINER(self->evbox_title), GTK_WIDGET(self->label_title));

	self->use_title = self->use_icon = FALSE;
#endif // WIBUTI_WITH_TITLE

	gtk_container_add(GTK_CONTAINER(self), GTK_WIDGET(self->box));
	wibuti_widget_repack_with_string(self, ""); // TODO: is it needed?
}

static void wibuti_widget_class_init(WibutiWidgetClass *klass) {
#ifdef WIBUTI_WITH_BUTTONS
	g_signal_newv(WIBUTI_SIGNAL_MINIMIZE, WIBUTI_TYPE_WIDGET, G_SIGNAL_RUN_LAST,
				NULL, NULL, NULL, g_cclosure_marshal_VOID__VOID, G_TYPE_NONE, 0, NULL);
	g_signal_newv(WIBUTI_SIGNAL_MAXIMIZE, WIBUTI_TYPE_WIDGET, G_SIGNAL_RUN_LAST,
				NULL, NULL, NULL, g_cclosure_marshal_VOID__VOID, G_TYPE_NONE, 0, NULL);
	g_signal_newv(WIBUTI_SIGNAL_RESTORE, WIBUTI_TYPE_WIDGET, G_SIGNAL_RUN_LAST,
				NULL, NULL, NULL, g_cclosure_marshal_VOID__VOID, G_TYPE_NONE, 0, NULL);
	g_signal_newv(WIBUTI_SIGNAL_CLOSE, WIBUTI_TYPE_WIDGET, G_SIGNAL_RUN_LAST,
				NULL, NULL, NULL, g_cclosure_marshal_VOID__VOID, G_TYPE_NONE, 0, NULL);
#endif // WIBUTI_WITH_BUTTONS
}

WibutiWidget* wibuti_widget_new(void) {
	return WIBUTI_WIDGET(g_object_new(WIBUTI_TYPE_WIDGET, NULL));
}


/**********************************************************************************************************************/


void wibuti_widget_repack_with_string(WibutiWidget *self, const gchar *string) {
	int i;
#ifdef WIBUTI_WITH_BUTTONS
	for (i = WIBUTI_BUTTON_FIRST; i < WIBUTI_BUTTON_NUM; ++i) {
		g_object_ref(self->evbox[i]);
	}

	if (self->use_minimize) {
		gtk_container_remove(GTK_CONTAINER(self->box), GTK_WIDGET(self->evbox[WIBUTI_BUTTON_MINIMIZE]));
	}
	if (self->use_maximize) {
		gtk_container_remove(GTK_CONTAINER(self->box), GTK_WIDGET(self->evbox[WIBUTI_BUTTON_MAXIMIZE]));
		gtk_container_remove(GTK_CONTAINER(self->box), GTK_WIDGET(self->evbox[WIBUTI_BUTTON_RESTORE]));
	}
	if (self->use_close) {
		gtk_container_remove(GTK_CONTAINER(self->box), GTK_WIDGET(self->evbox[WIBUTI_BUTTON_CLOSE]));
	}

	self->use_close = self->use_maximize = self->use_minimize = FALSE;
#endif // WIBUTI_WITH_BUTTONS

#ifdef WIBUTI_WITH_TITLE
	g_object_ref(self->evbox_title);
	g_object_ref(self->evbox_icon);

	if (self->use_title) {
		gtk_container_remove(GTK_CONTAINER(self->box), GTK_WIDGET(self->evbox_title));
	}
	if (self->use_icon) {
		gtk_container_remove(GTK_CONTAINER(self->box), GTK_WIDGET(self->evbox_icon));
	}

	self->use_title = self->use_icon = FALSE;
#endif // WIBUTI_WITH_TITLE

	i = 0;
	while (string[i] != '\0') {
		switch (string[i]) {
#ifdef WIBUTI_WITH_BUTTONS
			case 'M': {
				gtk_box_pack_start(GTK_BOX(self->box), GTK_WIDGET(self->evbox[WIBUTI_BUTTON_MINIMIZE]), FALSE, TRUE,0);
				self->use_minimize = TRUE;
				break;
			} case 'R': {
				gtk_box_pack_start(GTK_BOX(self->box), GTK_WIDGET(self->evbox[WIBUTI_BUTTON_MAXIMIZE]), FALSE, TRUE,0);
				gtk_box_pack_start(GTK_BOX(self->box), GTK_WIDGET(self->evbox[WIBUTI_BUTTON_RESTORE]), FALSE, TRUE, 0);
				self->use_maximize = TRUE;
				break;
			} case 'X': {
				gtk_box_pack_start(GTK_BOX(self->box), GTK_WIDGET(self->evbox[WIBUTI_BUTTON_CLOSE]), FALSE, TRUE, 0);
				self->use_close = TRUE;
				break;
			}
#endif // WIBUTI_WITH_BUTTONS

#ifdef WIBUTI_WITH_TITLE
			case 'T': {
				gtk_box_pack_start(GTK_BOX(self->box), GTK_WIDGET(self->evbox_title), FALSE, TRUE, 4);
				wibuti_widget_set_expand_title(self, self->expand_title);
				self->use_title = TRUE;
				break;
			} case 'I': {
				gtk_box_pack_start(GTK_BOX(self->box), GTK_WIDGET(self->evbox_icon), FALSE, TRUE, 0);
				self->use_icon = TRUE;
				break;
			}
#endif // WIBUTI_WITH_TITLE
		}
		++i;
	}

/*#ifdef WIBUTI_WITH_BUTTONS // TODO: why this breaks applet?
	g_object_unref(self->evbox_minimize);
	g_object_unref(self->evbox_maximize);
	g_object_unref(self->evbox_close);
#endif // WIBUTI_WITH_BUTTONS

#ifdef WIBUTI_WITH_TITLE
	g_object_unref(self->evbox_title);
	g_object_unref(self->evbox_icon);
#endif // WIBUTI_WITH_TITLE*/
}

void wibuti_widget_change_angle(WibutiWidget *self, WibutiWidgetAngle angle) {
	self->angle = angle;

	if (angle == 0) {
		gtk_orientable_set_orientation(GTK_ORIENTABLE(self->box), GTK_ORIENTATION_HORIZONTAL);
	} else {
		gtk_orientable_set_orientation(GTK_ORIENTABLE(self->box), GTK_ORIENTATION_VERTICAL);
	}

#ifdef WIBUTI_WITH_BUTTONS
	wibuti_widget_redraw_buttons(self);
#endif // WIBUTI_WITH_BUTTONS

#ifdef WIBUTI_WITH_TITLE
	gtk_label_set_angle(self->label_title, angle);
	wibuti_widget_set_icon(self, self->icon);
#endif // WIBUTI_WITH_TITLE
}

void wibuti_widget_set_only_maximized(WibutiWidget *self, gboolean only_maximized) {
	self->only_maximized = only_maximized;
}

void wibuti_widget_set_is_active(WibutiWidget *self, gboolean is_active) {
	self->is_active = is_active;
}


/**********************************************************************************************************************/


#ifdef WIBUTI_WITH_BUTTONS

void wibuti_widget_redraw_buttons(WibutiWidget *self) {
	WibutiFocus focus;
	if (self->is_active) {
		focus = WIBUTI_FOCUS_FOCUSED;
	} else {
		focus = WIBUTI_FOCUS_UNFOCUSED;
	}

	int b;
	for (b = WIBUTI_BUTTON_FIRST; b < WIBUTI_BUTTON_NUM; ++b) {
		gtk_image_set_from_pixbuf(self->image[b], self->buttons[focus][WIBUTI_STATE_NORMAL][b]);
	}
}

void wibuti_widget_change_theme(WibutiWidget *self, GdkPixbuf ****buttons) {
	if (buttons != NULL) {
		self->buttons = buttons;
	}
	wibuti_widget_redraw_buttons(self);
}

void wibuti_widget_set_click_effect(WibutiWidget *self, gboolean click_effect) {
	self->click_effect = click_effect;
}

void wibuti_widget_set_hover_effect(WibutiWidget *self, gboolean hover_effect) {
	self->hover_effect = hover_effect;
}

void wibuti_widget_set_maximized(WibutiWidget *self, gboolean is_maximized) {
	self->is_maximized = is_maximized;
	if (self->use_maximize) {
		if (is_maximized) {
			gtk_widget_hide(GTK_WIDGET(self->evbox[WIBUTI_BUTTON_MAXIMIZE]));
			gtk_widget_show(GTK_WIDGET(self->evbox[WIBUTI_BUTTON_RESTORE]));
		} else {
			gtk_widget_show(GTK_WIDGET(self->evbox[WIBUTI_BUTTON_MAXIMIZE]));
			gtk_widget_hide(GTK_WIDGET(self->evbox[WIBUTI_BUTTON_RESTORE]));
		}
	}
}

/* calbacks */

static void wibuti_widget_release_cb(GtkEventBox *evbox, GdkEvent *event, WibutiWidget *self) {
	WibutiButton b = wibuti_widget_get_button_num(self, evbox);

	if (self->click_effect) {
		WibutiFocus f = wibuti_widget_get_draw_active(self);
		gtk_image_set_from_pixbuf(self->image[b], self->buttons[f][WIBUTI_STATE_HOVER][b]);
	}
	
	switch (b) {
		case WIBUTI_BUTTON_MINIMIZE: {
			g_signal_emit_by_name(self, WIBUTI_SIGNAL_MINIMIZE);
			break;
		} case WIBUTI_BUTTON_MAXIMIZE: {
			g_signal_emit_by_name(self, WIBUTI_SIGNAL_MAXIMIZE);
			break;
		} case WIBUTI_BUTTON_RESTORE: {
			g_signal_emit_by_name(self, WIBUTI_SIGNAL_RESTORE);
			break;
		} case WIBUTI_BUTTON_CLOSE: {
			g_signal_emit_by_name(self, WIBUTI_SIGNAL_CLOSE);
			break;
		} 
	}	
}

static void wibuti_widget_press_cb(GtkEventBox *evbox, GdkEvent *event, WibutiWidget *self) {
	if (self->click_effect) {
		WibutiButton b = wibuti_widget_get_button_num(self, evbox);
		WibutiFocus f = wibuti_widget_get_draw_active(self);
		gtk_image_set_from_pixbuf(self->image[b], self->buttons[f][WIBUTI_STATE_CLICKED][b]);
	}
}

static void wibuti_widget_enter_cb(GtkEventBox *evbox, GdkEvent *event, WibutiWidget *self) {
	if (self->hover_effect) {
		WibutiButton b = wibuti_widget_get_button_num(self, evbox);
		WibutiFocus f = wibuti_widget_get_draw_active(self);
		gtk_image_set_from_pixbuf(self->image[b], self->buttons[f][WIBUTI_STATE_HOVER][b]);
	}
}

static void wibuti_widget_leave_cb(GtkEventBox *evbox, GdkEvent *event, WibutiWidget *self) {
	if (self->hover_effect) {
		WibutiButton b = wibuti_widget_get_button_num(self, evbox);
		WibutiFocus f = wibuti_widget_get_draw_active(self);
		gtk_image_set_from_pixbuf(self->image[b], self->buttons[f][WIBUTI_STATE_NORMAL][b]);
	}
}

/* private */

static WibutiButton wibuti_widget_get_button_num(WibutiWidget *self, GtkEventBox *evbox) {
	WibutiButton b;
	for (b = WIBUTI_BUTTON_FIRST; b < WIBUTI_BUTTON_NUM; b++) {
		if (evbox == self->evbox[b]) {
			return b;
		} 
	}
	return WIBUTI_BUTTON_FIRST;
}

static WibutiFocus wibuti_widget_get_draw_active(WibutiWidget *self) {
	return self->only_maximized && self->is_active ? WIBUTI_FOCUS_FOCUSED : WIBUTI_FOCUS_UNFOCUSED;
}

#endif // WIBUTI_WITH_BUTTONS


/**********************************************************************************************************************/


#ifdef WIBUTI_WITH_TITLE

void wibuti_widget_set_title(WibutiWidget *self, const gchar *title, gboolean is_active) {
	if (self->use_title) {
		gtk_label_set_text(self->label_title, title);
		gtk_widget_set_sensitive(GTK_WIDGET(self->label_title), is_active);
	}
}

void wibuti_widget_set_markup_title(WibutiWidget *self, const gchar *title, const gchar *font, const gchar *color) {
	if (self->use_title) {
		gchar *title_text = g_markup_printf_escaped("<span font=\"%s\" color=\"%s\">%s</span>", font, color, title);
		gtk_label_set_markup(GTK_LABEL(self->label_title), title_text);
		g_free(title_text);
	}
}

void wibuti_widget_set_expand_title(WibutiWidget *self, gboolean expand_title) {
	self->expand_title = expand_title;
	gtk_box_set_child_packing(GTK_BOX(self->box), GTK_WIDGET(self->evbox_title), expand_title, TRUE, 0, GTK_PACK_START);
}

void wibuti_widget_set_icon(WibutiWidget *self, GdkPixbuf *icon) { // TODO: fix memory leak
	if (self->use_icon) {
		if (icon == NULL) {
			gtk_image_clear(self->image_icon);
			self->icon = NULL;
		} else {
			GdkPixbuf *icon_tmp1 = gdk_pixbuf_scale_simple(icon, ICON_WIDTH, ICON_HEIGHT, GDK_INTERP_BILINEAR);
			self->icon = icon_tmp1;
			if (!self->is_active) {
				gdk_pixbuf_saturate_and_pixelate(icon_tmp1, icon_tmp1, 0, FALSE);
			}
			GdkPixbuf *icon_tmp2 = gdk_pixbuf_rotate_simple(icon_tmp1, self->angle);
			gtk_image_set_from_pixbuf(self->image_icon, icon_tmp2);
			g_object_unref(icon_tmp2);
		}
	}
}

void wibuti_widget_set_alignment(WibutiWidget *self, gdouble value) {
	gtk_misc_set_alignment(GTK_MISC(self->label_title), value, 0.5);
}

#endif // WIBUTI_WITH_TITLE


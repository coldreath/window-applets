#include "applets/widget.h"

#define ICON_WIDTH	16
#define ICON_HEIGHT	16

G_DEFINE_TYPE(WibutiWidget, wibuti_widget, GTK_TYPE_BOX);

static void wibuti_widget_init(WibutiWidget *self) {
#ifdef WIBUTI_WITH_BUTTONS

#endif // WIBUTI_WITH_BUTTONS
#ifdef WIBUTI_WITH_TITLE
	// create contents
	self->box = GTK_BOX(gtk_hbox_new(FALSE, 2));
	self->evbox_icon = GTK_EVENT_BOX(gtk_event_box_new());
	self->image_icon = GTK_IMAGE(gtk_image_new());
	self->evbox_title = GTK_EVENT_BOX(gtk_event_box_new());
	self->label_title = GTK_LABEL(gtk_label_new(NULL));
	
	// pack gui
	gtk_container_add(GTK_CONTAINER(self->evbox_icon), GTK_WIDGET(self->image_icon));
	gtk_container_add(GTK_CONTAINER(self->evbox_title), GTK_WIDGET(self->label_title));

	gtk_box_pack_start(GTK_BOX(self->box), GTK_WIDGET(self->evbox_icon), TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(self->box), GTK_WIDGET(self->evbox_title), TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(self), GTK_WIDGET(self->box), TRUE, TRUE, 0);
#endif // WIBUTI_WITH_TITLE
}

static void wibuti_widget_class_init(WibutiWidgetClass *klass) {
}

WibutiWidget* wibuti_widget_new(void) {
	return WIBUTI_WIDGET(g_object_new(WIBUTI_TYPE_WIDGET, NULL));
}


/**********************************************************************************************************************/

void wibuti_widget_change_angle(WibutiWidget *self, WibutiWidgetAngle angle) {
	self->angle = angle;
#ifdef WIBUTI_WITH_BUTTONS

#endif // WIBUTI_WITH_BUTTONS
#ifdef WIBUTI_WITH_TITLE
	gtk_label_set_angle(self->label_title, angle);
	if (self->icon != NULL) {
		gtk_image_set_from_pixbuf(self->image_icon, gdk_pixbuf_rotate_simple(self->icon, self->angle));
	}
	
	if (angle == 0) {
		gtk_orientable_set_orientation(GTK_ORIENTABLE(self->box), GTK_ORIENTATION_HORIZONTAL);
	} else {
		gtk_orientable_set_orientation(GTK_ORIENTABLE(self->box), GTK_ORIENTATION_VERTICAL);
	}
#endif // WIBUTI_WITH_TITLE
}


#ifdef WIBUTI_WITH_BUTTONS

#endif // WIBUTI_WITH_BUTTONS
#ifdef WIBUTI_WITH_TITLE
void wibuti_widget_set_title(WibutiWidget *self, const gchar *title, gboolean is_active) {
	gtk_label_set_text(self->label_title, title);
	gtk_widget_set_sensitive(GTK_WIDGET(self->label_title), is_active);
}

void wibuti_widget_set_title_with_markup(WibutiWidget *self, const gchar *title, const gchar *font, const gchar *color) {
	gchar *title_text = g_markup_printf_escaped("<span font=\"%s\" color=\"%s\">%s</span>", font, color, title);
	gtk_label_set_markup(GTK_LABEL(self->label_title), title_text);
	g_free(title_text);
}

void wibuti_widget_set_icon(WibutiWidget *self, GdkPixbuf *icon, gboolean is_active) {
	if (icon == NULL) {
		gtk_image_clear(self->image_icon);
		self->icon = NULL;
	} else {
		GdkPixbuf *icon_tmp1 = gdk_pixbuf_scale_simple(icon, ICON_WIDTH, ICON_HEIGHT, GDK_INTERP_BILINEAR);
		self->icon = icon_tmp1;
		if (!is_active) {
			gdk_pixbuf_saturate_and_pixelate(icon_tmp1, icon_tmp1, 0, FALSE);
		}
		GdkPixbuf *icon_tmp2 = gdk_pixbuf_rotate_simple(icon_tmp1, self->angle);
		g_object_unref(icon_tmp1);
		gtk_image_set_from_pixbuf(self->image_icon, icon_tmp2);
		g_object_unref(icon_tmp2);
	}
}

void wibuti_widget_set_packing(WibutiWidget *self, gboolean swap_order, gboolean expand_title) {
	GtkPackType pack_type = swap_order ? GTK_PACK_END : GTK_PACK_START;
	gtk_box_set_child_packing(GTK_BOX(self->box), GTK_WIDGET(self->evbox_title), expand_title, expand_title, 5, pack_type);
	gtk_box_set_child_packing(GTK_BOX(self->box), GTK_WIDGET(self->evbox_icon), FALSE, FALSE, 2, pack_type);
}

void wibuti_widget_set_alignment(WibutiWidget *self, gdouble value) {
	gtk_misc_set_alignment(GTK_MISC(self->label_title), value, 0.5);
}

void wibuti_widget_hide_title(WibutiWidget *self, gboolean hide) {
	if (hide) {
		gtk_widget_hide(GTK_WIDGET(self->label_title));
	} else {
		gtk_widget_show(GTK_WIDGET(self->label_title));
	}
}

void wibuti_widget_hide_icon(WibutiWidget *self, gboolean hide) {
	if (hide) {
		gtk_widget_hide(GTK_WIDGET(self->image_icon));
	} else {
		gtk_widget_show(GTK_WIDGET(self->image_icon));
	}
}
#endif // WIBUTI_WITH_TITLE


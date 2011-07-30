#ifndef __WIBUTI_WIDGET_H__
#define __WIBUTI_WIDGET_H__

#include <gtk/gtk.h>

#include "applets/configuration.h"

typedef enum {
	WIBUTI_WIDGET_ANGLE_UP = 0,
	WIBUTI_WIDGET_ANGLE_DOWN = 0,
	WIBUTI_WIDGET_ANGLE_LEFT = 90,
	WIBUTI_WIDGET_ANGLE_RIGHT = 270,
} WibutiWidgetAngle;

#define WIBUTI_TYPE_WIDGET				(wibuti_widget_get_type())
#define WIBUTI_WIDGET(obj)				(G_TYPE_CHECK_INSTANCE_CAST((obj), WIBUTI_TYPE_WIDGET, WibutiWidget))
#define WIBUTI_IS_WIDGET(obj)			(G_TYPE_CHECK_INSTANCE_TYPE((obj), WIBUTI_TYPE_WIDGET))
#define WIBUTI_WIDGET_CLASS(klass)		(G_TYPE_CHECK_CLASS_CAST((klass), WIBUTI_TYPE_WIDGET, WibutiWidgetClass))
#define WIBUTI_IS_WIDGET_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE((klass), WIBUTI_TYPE_WIDGET))
#define WIBUTI_WIDGET_GET_CLASS(obj)	(G_TYPE_INSTANCE_GET_CLASS((obj), WIBUTI_TYPE_WIDGET, WibutiWidgetClass))

typedef struct {
	GtkBox parent;

	GtkBox *box;
	WibutiWidgetAngle angle;
	
#ifdef WIBUTI_WITH_BUTTONS
	gboolean use_close;
	gboolean use_maximize;
	gboolean use_minimize;

	GtkEventBox		*evbox_minimize;
	GtkEventBox		*evbox_maximize;
	GtkEventBox		*evbox_close;
	GtkImage		*image_minimize;
	GtkImage		*image_maximize;
	GtkImage		*image_close;
	GdkPixbuf		***buttons;
#endif // WIBUTI_WITH_BUTTONS

#ifdef WIBUTI_WITH_TITLE
	gboolean use_title;
	gboolean use_icon;

	GtkEventBox		*evbox_title;
	GtkEventBox		*evbox_icon;
	GtkLabel		*label_title;
	GtkImage		*image_icon;
	GdkPixbuf		*icon;
#endif // WIBUTI_WITH_TITLE	
} WibutiWidget;

typedef struct {
	GtkBoxClass parent;
} WibutiWidgetClass;

GType wibuti_widget_get_type(void);
WibutiWidget* wibuti_widget_new(void);

void wibuti_widget_change_angle(WibutiWidget *self, WibutiWidgetAngle angle);
void wibuti_widget_repack_with_string(WibutiWidget *self, gchar *string);

#ifdef WIBUTI_WITH_BUTTONS
void wibuti_widget_change_theme(WibutiWidget *self, GdkPixbuf ***pixbufs);
#endif // WIBUTI_WITH_BUTTONS

#ifdef WIBUTI_WITH_TITLE
void wibuti_widget_set_title(WibutiWidget *self, const gchar *title, gboolean is_active);
void wibuti_widget_set_title_with_markup(WibutiWidget *self, const gchar *title, const gchar *font, const gchar *color);
void wibuti_widget_set_expand_title(WibutiWidget *self, gboolean expand);
void wibuti_widget_set_icon(WibutiWidget *self, GdkPixbuf *icon, gboolean is_active);
void wibuti_widget_set_alignment(WibutiWidget *self, gdouble value);
//void wibuti_widget_hide_title(WibutiWidget *self, gboolean hide);
//void wibuti_widget_hide_icon(WibutiWidget *self, gboolean hide);
#endif // WIBUTI_WITH_TITLE

#endif // __WIBUTI_WIDGET_H__


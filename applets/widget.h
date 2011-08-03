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
	GtkEventBox parent;

	GtkBox *box;
	WibutiWidgetAngle angle;
	
	gboolean only_maximized;
	gboolean is_active;
	
#ifdef WIBUTI_WITH_BUTTONS
	gboolean use_close;
	gboolean use_maximize;
	gboolean use_minimize;
	
	gboolean click_effect;
	gboolean hover_effect;
	gboolean is_maximized;

	GtkEventBox		**evbox;
	GtkImage		**image;
	GdkPixbuf		****buttons;
#endif // WIBUTI_WITH_BUTTONS

#ifdef WIBUTI_WITH_TITLE
	gboolean use_title;
	gboolean use_icon;
	
	gboolean expand_title;

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

void wibuti_widget_change_angle(WibutiWidget *, WibutiWidgetAngle);
void wibuti_widget_repack_with_string(WibutiWidget *, const gchar *);
void wibuti_widget_set_only_maximized(WibutiWidget *, gboolean);
void wibuti_widget_set_is_active(WibutiWidget *, gboolean);

#ifdef WIBUTI_WITH_BUTTONS
void wibuti_widget_change_theme(WibutiWidget *, GdkPixbuf ****);
void wibuti_widget_redraw_buttons(WibutiWidget *);
void wibuti_widget_set_click_effect(WibutiWidget *, gboolean);
void wibuti_widget_set_hover_effect(WibutiWidget *, gboolean);
void wibuti_widget_set_maximized(WibutiWidget *, gboolean);
#endif // WIBUTI_WITH_BUTTONS

#ifdef WIBUTI_WITH_TITLE
void wibuti_widget_set_title(WibutiWidget *, const gchar *, gboolean);
void wibuti_widget_set_markup_title(WibutiWidget *, const gchar *, const gchar *, const gchar *);
void wibuti_widget_set_icon(WibutiWidget *, GdkPixbuf *);
void wibuti_widget_set_expand_title(WibutiWidget *, gboolean);
void wibuti_widget_set_alignment(WibutiWidget *, gdouble);
#endif // WIBUTI_WITH_TITLE

#endif // __WIBUTI_WIDGET_H__


#ifndef __BUTTONS_WIDGET_H__
#define __BUTTONS_WIDGET_H__

#include <gtk/gtk.h>

#include <buttons/buttons-config.h>

typedef enum {
	WB_WIDGET_ANGLE_UP = 0,
	WB_WIDGET_ANGLE_DOWN = 0,
	WB_WIDGET_ANGLE_LEFT = 90,
	WB_WIDGET_ANGLE_RIGHT = 270,
} WBWidgetAngle;

#define WB_TYPE_WIDGET				(wb_widget_get_type())
#define WB_WIDGET(obj)				(G_TYPE_CHECK_INSTANCE_CAST((obj), WB_TYPE_WIDGET, WBWidget))
#define WB_IS_WIDGET(obj)			(G_TYPE_CHECK_INSTANCE_TYPE((obj), WB_TYPE_WIDGET))
#define WB_WIDGET_CLASS(klass)		(G_TYPE_CHECK_CLASS_CAST((klass), WB_TYPE_WIDGET, WBWidgetClass))
#define WB_IS_WIDGET_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE((klass), WB_TYPE_WIDGET))
#define WB_WIDGET_GET_CLASS(obj)	(G_TYPE_INSTANCE_GET_CLASS((obj), WB_TYPE_WIDGET, WBWidgetClass))

typedef struct {
	GtkBox			 parent;
	WBWidgetAngle 	 angle;
} WBWidget;

typedef struct {
	GtkBoxClass parent;
} WBWidgetClass;

GType wb_widget_get_type(void);
WBWidget* wb_widget_new(void);

#endif /* __BUTTONS_WIDGET_H__ */


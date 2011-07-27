#ifndef __TITLE_WIDGET_H__
#define __TITLE_WIDGET_H__

#include <gtk/gtk.h>

#include <title/title-config.h>

typedef enum {
	WT_WIDGET_ANGLE_UP = 0,
	WT_WIDGET_ANGLE_DOWN = 0,
	WT_WIDGET_ANGLE_LEFT = 90,
	WT_WIDGET_ANGLE_RIGHT = 270,
} WTWidgetAngle;

#define WT_TYPE_WIDGET				(wt_widget_get_type())
#define WT_WIDGET(obj)				(G_TYPE_CHECK_INSTANCE_CAST((obj), WT_TYPE_WIDGET, WTWidget))
#define WT_IS_WIDGET(obj)			(G_TYPE_CHECK_INSTANCE_TYPE((obj), WT_TYPE_WIDGET))
#define WT_WIDGET_CLASS(klass)		(G_TYPE_CHECK_CLASS_CAST((klass), WT_TYPE_WIDGET, WTWidgetClass))
#define WT_IS_WIDGET_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE((klass), WT_TYPE_WIDGET))
#define WT_WIDGET_GET_CLASS(obj)	(G_TYPE_INSTANCE_GET_CLASS((obj), WT_TYPE_WIDGET, WTWidgetClass))

typedef struct {
	GtkBox			 parent;
	
	GtkBox			*box;
	GtkEventBox		*evbox_title;
	GtkEventBox		*evbox_icon;
	GtkLabel		*label_title;
	GtkImage		*image_icon;
	
	WTWidgetAngle 	 angle;
	GdkPixbuf		*icon;
} WTWidget;

typedef struct {
	GtkBoxClass parent;
} WTWidgetClass;

GType wt_widget_get_type(void);
WTWidget* wt_widget_new(void);

void wt_widget_set_title(WTWidget *self, const gchar *title, gboolean is_active);
void wt_widget_set_title_with_markup(WTWidget *self, const gchar *title, const gchar *font, const gchar *color);
void wt_widget_set_icon(WTWidget *self, GdkPixbuf *icon, gboolean is_active);
void wt_widget_set_packing(WTWidget *self, gboolean swap_order, gboolean expand_title);
void wt_widget_set_alignment(WTWidget *self, gdouble value);
void wt_widget_hide_title(WTWidget *self, gboolean hide);
void wt_widget_hide_icon(WTWidget *self, gboolean hide);
void wt_widget_change_angle(WTWidget *self, WTWidgetAngle angle);

#endif /* __TITLE_WIDGET_H__ */


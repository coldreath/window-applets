#include <buttons/buttons-widget.h>

G_DEFINE_TYPE(WBWidget, wb_widget, GTK_TYPE_BOX);

static void wb_widget_init(WBWidget *self) {
}

static void wb_widget_class_init(WBWidgetClass *klass) {
}

WBWidget* wb_widget_new(void) {
	return WB_WIDGET(g_object_new(WB_TYPE_WIDGET, NULL));
}


/**********************************************************************************************************************/



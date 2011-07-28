#include <buttons/buttons-widget.h>

G_DEFINE_TYPE(WBWidget, wb_widget, GTK_TYPE_BOX);

static void wb_widget_init(WBWidget *self) {
	GtkLabel *label = GTK_LABEL(gtk_label_new("Here will be buttons :)"));
	gtk_box_pack_start(GTK_BOX(self), GTK_WIDGET(label), TRUE, TRUE, 0);
	gtk_widget_show_all(GTK_WIDGET(self));
}

static void wb_widget_class_init(WBWidgetClass *klass) {
}

WBWidget* wb_widget_new(void) {
	return WB_WIDGET(g_object_new(WB_TYPE_WIDGET, NULL));
}


/**********************************************************************************************************************/



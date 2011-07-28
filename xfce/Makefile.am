AM_CPPFLAGS = $(XFCE_PANEL_CFLAGS) $(WNCK_CFLAGS) -I$(srcdir)/.. -Wall

xfcelibexecdir = $(prefix)/libexec/xfce4/panel-plugins/
xfcelibexec_PROGRAMS = \
	xfce4-windowtitle-plugin \
	xfce4-windowbuttons-plugin

xfce4_windowtitle_plugin_LDADD = $(XFCE_PANEL_LIBS) $(WNCK_LIBS)
xfce4_windowtitle_plugin_SOURCES = \
	$(top_srcdir)/xfce/xfce4-windowtitle-main.c \
	$(top_srcdir)/xfce/xfce4-title-plugin.c \
	$(top_srcdir)/xfce/xfce4-title-plugin.h \
	$(top_srcdir)/title/title-widget.c \
	$(top_srcdir)/title/title-widget.h \
	$(top_srcdir)/title/title-plugin.c \
	$(top_srcdir)/title/title-plugin.h \
	$(top_srcdir)/title/title-preferences.c \
	$(top_srcdir)/title/title-preferences.h \
	$(top_srcdir)/title/title-config.c \
	$(top_srcdir)/title/title-config.h \
	$(top_srcdir)/common/watcher.c \
	$(top_srcdir)/common/watcher.h \
	$(top_srcdir)/common/tools.c \
	$(top_srcdir)/common/tools.h

xfce4_windowbuttons_plugin_LDADD = $(XFCE_PANEL_LIBS) $(WNCK_LIBS)
xfce4_windowbuttons_plugin_SOURCES = \
	$(top_srcdir)/xfce/xfce4-windowbuttons-main.c \
	$(top_srcdir)/xfce/xfce4-buttons-plugin.c \
	$(top_srcdir)/xfce/xfce4-buttons-plugin.h \
	$(top_srcdir)/buttons/buttons-widget.c \
	$(top_srcdir)/buttons/buttons-widget.h \
	$(top_srcdir)/buttons/buttons-plugin.c \
	$(top_srcdir)/buttons/buttons-plugin.h \
	$(top_srcdir)/buttons/buttons-preferences.c \
	$(top_srcdir)/buttons/buttons-preferences.h \
	$(top_srcdir)/buttons/buttons-config.c \
	$(top_srcdir)/buttons/buttons-config.h \
	$(top_srcdir)/common/watcher.c \
	$(top_srcdir)/common/watcher.h \
	$(top_srcdir)/common/tools.c \
	$(top_srcdir)/common/tools.h

desktopfiledir = $(prefix)/share/xfce4/panel-plugins
desktopfile_DATA = xfce4-windowtitle-plugin.desktop xfce4-windowbuttons-plugin.desktop


AM_CPPFLAGS = $(XFCE_PANEL_CFLAGS) $(WNCK_CFLAGS) -I$(srcdir)/.. -Wall

xfcelibexecdir = $(prefix)/libexec/xfce4/panel-plugins/
xfcelibexec_PROGRAMS = xfce4-windowtitle-plugin
xfce4_windowtitle_plugin_LDADD = $(XFCE_PANEL_LIBS) $(WNCK_LIBS)
xfce4_windowtitle_plugin_SOURCES = \
	xfce4-windowtitle-main.c \
	xfce4-title-plugin.c \
	xfce4-title-plugin.h \
	../title/title-widget.c \
	../title/title-widget.h \
	../title/title-plugin.c \
	../title/title-plugin.h \
	../title/title-preferences.c \
	../title/title-preferences.h \
	../title/title-config.c \
	../title/title-config.h \
	../common/watcher.c \
	../common/watcher.h \
	../common/tools.c \
	../common/tools.h

desktopfiledir = $(prefix)/share/xfce4/panel-plugins
desktopfile_DATA = xfce4-windowtitle-plugin.desktop


AM_CPPFLAGS = -I${top_srcdir} -Wall

WIBUTI_SRC = \
		${top_srcdir}/config.h \
		${top_srcdir}/xfce/applet-xfce4-main.c \
		${top_srcdir}/applets/widget.c \
		${top_srcdir}/applets/widget.h \
		${top_srcdir}/applets/plugin.c \
		${top_srcdir}/applets/plugin.h \
		${top_srcdir}/applets/preferences.c \
		${top_srcdir}/applets/preferences.h \
		${top_srcdir}/applets/configuration.c \
		${top_srcdir}/applets/configuration.h \
		${top_srcdir}/applets/watcher.c \
		${top_srcdir}/applets/watcher.h 

xfcelibexecdir = ${prefix}/libexec/xfce4/panel-plugins/
desktopfiledir = ${prefix}/share/xfce4/panel-plugins


xfce4_windowtitle_plugin_CFLAGS = ${XFCE_PANEL_CFLAGS} ${WNCK_CFLAGS}
xfce4_windowtitle_plugin_LDADD = ${XFCE_PANEL_LIBS} ${WNCK_LIBS}
xfce4_windowtitle_plugin_SOURCES = ${WIBUTI_SRC}

xfce4_windowbuttons_plugin_CFLAGS = ${XFCE_PANEL_CFLAGS} ${WNCK_CFLAGS}
xfce4_windowbuttons_plugin_LDADD = ${XFCE_PANEL_LIBS} ${WNCK_LIBS}
xfce4_windowbuttons_plugin_SOURCES = ${WIBUTI_SRC}


xfcelibexec_PROGRAMS = 
desktopfile_DATA = 

if WIBUTI_WITH_BUTTONS
xfcelibexec_PROGRAMS += xfce4-windowbuttons-plugin 
desktopfile_DATA += xfce4-windowbuttons-plugin.desktop
endif

if WIBUTI_WITH_TITLE
xfcelibexec_PROGRAMS += xfce4-windowtitle-plugin 
desktopfile_DATA += xfce4-windowtitle-plugin.desktop
endif


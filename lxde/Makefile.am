AM_CPPFLAGS = $(WNCK_CFLAGS) -Wall
INCLUDES = -I. -I$(top_srcdir)

lxdelibdir = $(libdir)/lxpanel/plugins
lxdelib_LTLIBRARIES = libwindowtitle.la

libwindowtitle_la_LIBADD = $(WNCK_LIBS)
libwindowtitle_la_SOURCES = \
	$(top_srcdir)/lxde/lxde-windowtitle-main.c \
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


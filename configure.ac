AC_PREREQ([2.66])
AC_INIT(window-applets, 0.3)
AM_INIT_AUTOMAKE(AC_PACKAGE_NAME, AC_PACKAGE_VERSION)
AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADERS([config.h])

AC_PROG_CC
AC_PROG_LIBTOOL

PKG_CHECK_MODULES(GLIB, glib-2.0)
PKG_CHECK_MODULES(GTK, gtk+-2.0)
PKG_CHECK_MODULES(WNCK, libwnck-1.0)
PKG_CHECK_MODULES(XFCE_PANEL, libxfce4panel-1.0)
PKG_CHECK_MODULES(LXDE_PANEL, lxpanel)

AC_CONFIG_FILES([Makefile
                 xfce/Makefile
                 lxde/Makefile
                 title/Makefile])
AC_OUTPUT

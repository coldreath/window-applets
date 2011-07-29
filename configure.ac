AC_PREREQ([2.66])
AC_INIT(window-applets, 0.3)
AM_INIT_AUTOMAKE(AC_PACKAGE_NAME, AC_PACKAGE_VERSION)

AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADERS([config.h])

AC_PROG_CC
AC_PROG_LIBTOOL


AC_MSG_CHECKING(for the panel we're compiling for)
AC_ARG_ENABLE(
	[panel],
	AC_HELP_STRING([--enable-panel=(xfce|ldxe)], [Enable compiling applet for specified panel]),
	[case "$enableval" in
		xfce) panel="xfce" ;;
		lxde) panel="lxde" ;;
		*) AC_MSG_ERROR([the value passed to --enable-panel should be "xfce" or "lxde"]) ;;
	esac],
	[panel="xfce"]
)
AC_MSG_RESULT($panel)
AC_SUBST(WIBUTI_PANEL_NAME, $panel)
AM_CONDITIONAL(WIBUTI_ENABLE_XFCE, test "$panel" = "xfce")
AM_CONDITIONAL(WIBUTI_ENABLE_LXDE, test "$panel" = "lxde")


AC_MSG_CHECKING(if we compile buttons)
AC_ARG_WITH(
	[buttons],
	AC_HELP_STRING([--with-buttons=(yes|no)], [Enable compiling buttons applet]),
	[case "$withval" in
		yes) buttons="true" ;;
		no)  buttons="false" ;;
		*) AC_MSG_ERROR([the value passed to --with-buttons should be "yes" or "no"]) ;;
	esac],
	[buttons="false"]
)
AC_MSG_RESULT($buttons)
AC_SUBST(WIBUTI_WITH_BUTTONS, $buttons)
AC_DEFINE(WIBUTI_WITH_BUTTONS, test "$buttons" = "true", Use buttons)
AM_CONDITIONAL(WIBUTI_WITH_BUTTONS, test "$buttons" = "true")


AC_MSG_CHECKING(if we compile title)
AC_ARG_WITH(
	[title],
	AC_HELP_STRING([--with-title=(yes|no)], [Enable compiling title applet]),
	[case "$withval" in
		yes) title=true ;;
		no)  title=false ;;
		*) AC_MSG_ERROR([the value passed to --with-title should be "yes" or "no"]) ;;
	esac],
	[title=false]
)
AC_MSG_RESULT($withval)
AC_SUBST(WIBUTI_WITH_TITLE, $title)
AC_DEFINE(WIBUTI_WITH_TITLE, test "$title" = "true", Use title)
AM_CONDITIONAL(WIBUTI_WITH_TITLE, test "$title" = "true")


if test "$title" = "false" && test "$buttons" = "false"
then
	AC_MSG_ERROR(you have to specify the applet with --with-title=yes or --with-buttons=yes)
fi


# Check for avaible modules
PKG_CHECK_MODULES(GLIB, glib-2.0)
PKG_CHECK_MODULES(GTK, gtk+-2.0)
PKG_CHECK_MODULES(WNCK, libwnck-1.0)
case "$panel" in
	xfce) PKG_CHECK_MODULES(XFCE_PANEL, libxfce4panel-1.0) ;;
	lxde) PKG_CHECK_MODULES(LXDE_PANEL, lxpanel) ;;
esac


AC_CONFIG_FILES([Makefile
                 xfce/Makefile
                 lxde/Makefile
                 title/Makefile
                 buttons/Makefile])
AC_OUTPUT


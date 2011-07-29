SUBDIRS = \
		share \
		${WIBUTI_PANEL_NAME}

if WIBUTI_ENABLE_LXDE
install-exec-hook:
	rm -f ${DESTDIR}${libdir}/lxpanel/plugins/*.la
	rm -f ${DESTDIR}${libdir}/lxpanel/plugins/*.a
endif

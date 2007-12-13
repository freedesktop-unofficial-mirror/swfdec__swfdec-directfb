lib_LTLIBRARIES = libswfdec-directfb-@SWFDEC_MAJORMINOR@.la

libswfdec_directfb_@SWFDEC_MAJORMINOR@_la_SOURCES = \
	swfdec_source.c \
	swfdec_directfb_main.c \
	swfdec_directfb_player.c

noinst_HEADERS = \
	swfdec_source.h

libswfdec_directfb_@SWFDEC_MAJORMINOR@_la_CFLAGS = \
	-I$(top_srcdir) $(GLOBAL_CFLAGS) $(SWFDEC_CFLAGS) $(DIRECTFB_CFLAGS) \
	$(CAIRO_CFLAGS) $(AUDIO_CFLAGS) \
	-DG_LOG_DOMAIN=\"Swfdec-DirectFB\"

libswfdec_directfb_@SWFDEC_MAJORMINOR@_la_LDFLAGS = \
	-version-info $(SWFDEC_LIBVERSION) \
	-export-symbols-regex '^(swfdec_.*)' \
	$(GTK_LIBS) $(SWFDEC_LIBS) $(AUDIO_LIBS) $(CAIRO_LIBS)

libswfdec_@SWFDEC_MAJORMINOR@includedir = $(includedir)/swfdec-@SWFDEC_MAJORMINOR@/libswfdec-directfb
libswfdec_@SWFDEC_MAJORMINOR@include_HEADERS = \
	swfdec-directfb.h \
	swfdec_directfb_main.h \
	swfdec_directfb_player.h


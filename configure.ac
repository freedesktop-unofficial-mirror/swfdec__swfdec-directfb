AC_PREREQ([2.58])
AC_INIT(swfdec-directfb,0.7.3)

[is_dev=$(echo $PACKAGE_VERSION | sed 's/[0-9]\.[0-9][0-9]*\.[0-9]*[13579]/1/')]
if test x"$is_dev" = x1 ; then
  SWFDEC_CVS="yes"
else
  SWFDEC_CVS="no"
fi


AM_INIT_AUTOMAKE(1.6)
dnl AC_CANONICAL_TARGET([])
AM_MAINTAINER_MODE

SWFDEC_MAJORMINOR=0.7
AC_SUBST(SWFDEC_MAJORMINOR)

AM_CONFIG_HEADER(config.h)
AC_SUBST(ACLOCAL_AMFLAGS, "-I m4")

dnl decide on error flags
dnl if we support them, we set them unconditionally
AS_COMPILER_FLAG(-Wall, GLOBAL_CFLAGS="-Wall", GLOBAL_CFLAGS="")
AS_COMPILER_FLAG(-Wextra -Wno-missing-field-initializers -Wno-unused-parameter -Wold-style-definition -Wdeclaration-after-statement -Wmissing-declarations -Wmissing-prototypes -Wredundant-decls -Wmissing-noreturn -Wshadow -Wpointer-arith -Wcast-align -Wwrite-strings -Winline -Wformat-nonliteral -Wformat-security -Wswitch-enum -Wswitch-default, GLOBAL_CFLAGS="$GLOBAL_CFLAGS -Wextra -Wno-missing-field-initializers -Wno-unused-parameter -Wold-style-definition -Wdeclaration-after-statement -Wmissing-declarations -Wmissing-prototypes -Wredundant-decls -Wmissing-noreturn -Wshadow -Wpointer-arith -Wcast-align -Wwrite-strings -Winline -Wformat-nonliteral -Wformat-security -Wswitch-enum -Wswitch-default")
dnl if we're in nano >= 1, add -Werror if supported
if test x$SWFDEC_CVS = xyes ; then
  AS_COMPILER_FLAG(-Werror, GLOBAL_CFLAGS="$GLOBAL_CFLAGS -Werror")
fi

SWFDEC_LIBVERSION="0:0:0"
AC_SUBST(SWFDEC_LIBVERSION)
AM_PROG_LIBTOOL

AC_PROG_CC
AM_PROG_CC_STDC
AM_PROG_CC_C_O

AC_HEADER_STDC([])

dnl ##############################
dnl # Do automated configuration #
dnl ##############################

dnl Check for tools:
dnl ================

dnl modify pkg-config path
AC_ARG_WITH(pkg-config-path, 
   AC_HELP_STRING([--with-pkg-config-path],[colon-separated list of pkg-config(1) dirs]),
   [export PKG_CONFIG_PATH=${withval}])

dnl Check for essential libraries first:
dnl ====================================

SWFDEC_REQUIRES=0.7.3
AC_SUBST(SWFDEC_REQUIRES)
PKG_CHECK_MODULES(SWFDEC, swfdec-$SWFDEC_MAJORMINOR = $SWFDEC_REQUIRES, HAVE_SWFDEC=yes, HAVE_SWFDEC=no)
if test "$HAVE_SWFDEC" = "no"; then
  AC_MSG_ERROR([cannot find swfdec-$SWFDEC_MAJORMINOR $SWFDEC_REQUIRES, which is required for build])
fi

DIRECTFB_REQUIRES=0.9.25
AC_SUBST(DIRECTFB_REQUIRES)
PKG_CHECK_MODULES(DIRECTFB, directfb >= $DIRECTFB_REQUIRES, HAVE_DIRECTFB=yes, HAVE_DIRECTFB=no)
if test "$HAVE_DIRECTFB" = "no"; then
  AC_MSG_ERROR([cannot find directfb >= $DIECTFB_REQUIRES, which is required for build])
fi

PKG_CHECK_MODULES(CAIRO, cairo-directfb, HAVE_CAIRO=yes, HAVE_CAIRO=no)
if test "$HAVE_CAIRO" = "no"; then
  AC_MSG_ERROR([Cairo needs to have built-in DirectFB support to build])
fi

AC_SUBST(GLOBAL_CFLAGS)

SWFDEC_DFB_CFLAGS="-I\$(top_srcdir) $SWFDEC_CFLAGS $DIRECTFB_CFLAGS $CAIRO_CFLAGS"
SWFDEC_DFB_LIBS="\$(top_builddir)/swfdec-directfb/libswfdec-directfb-$SWFDEC_MAJORMINOR.la $SWFDEC_LIBS $DIRECTFB_LIBS $CAIRO_LIBS"
AC_SUBST(SWFDEC_DFB_CFLAGS)
AC_SUBST(SWFDEC_DFB_LIBS)

AC_CONFIG_FILES(
  Makefile
  data/swfdec-directfb.pc
  data/Makefile
  swfdec-directfb/Makefile
  player/Makefile
  test/Makefile
)

AC_OUTPUT


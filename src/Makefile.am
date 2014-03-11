lib_LTLIBRARIES = libzcodec.la

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libzcodec.pc

include_HEADERS = \
    ../include/zcodec.h \
    ../include/zcodec_msg.h

libzcodec_la_SOURCES = \
    platform.h \
    zcodec_msg.c

AM_CFLAGS = -g
AM_CPPFLAGS = -I$(top_srcdir)/include
bin_PROGRAMS = zcodec_selftest
zcodec_selftest_LDADD = libzcodec.la
zcodec_selftest_SOURCES = zcodec_selftest.c
libzcodec_la_LDFLAGS = -version-info @LTVER@
TESTS = zcodec_selftest

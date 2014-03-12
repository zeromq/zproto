lib_LTLIBRARIES = libzproto.la

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libzproto.pc

include_HEADERS = \
    ../include/zproto.h \
    ../include/zproto_example.h

libzproto_la_SOURCES = \
    platform.h \
    zproto_example.c

AM_CFLAGS = -g
AM_CPPFLAGS = -I$(top_srcdir)/include
bin_PROGRAMS = zproto_selftest
zproto_selftest_LDADD = libzproto.la
zproto_selftest_SOURCES = zproto_selftest.c
libzproto_la_LDFLAGS = -version-info @LTVER@
TESTS = zproto_selftest

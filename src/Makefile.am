noinst_LTLIBRARIES = libzproto.la

libzproto_la_SOURCES = \
    platform.h \
    zproto_example.c

AM_CFLAGS = -g
AM_CPPFLAGS = -I$(top_srcdir)/include
noinst_PROGRAMS = zproto_selftest
zproto_selftest_LDADD = libzproto.la
zproto_selftest_SOURCES = zproto_selftest.c
libzproto_la_LDFLAGS = -version-info @LTVER@ -rpath @libdir@
TESTS = zproto_selftest

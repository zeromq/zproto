lib_LTLIBRARIES = libmyproj.la

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libmyproj.pc

include_HEADERS = \
    ../include/myproj.h \
    ../include/myproj_msg.h

libmyproj_la_SOURCES = \
    platform.h \
    myproj_msg.c

AM_CFLAGS = -g
AM_CPPFLAGS = -I$(top_srcdir)/include
bin_PROGRAMS = myproj_selftest
myproj_selftest_LDADD = libmyproj.la
myproj_selftest_SOURCES = myproj_selftest.c
libmyproj_la_LDFLAGS = -version-info @LTVER@
TESTS = myproj_selftest


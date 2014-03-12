/*  =========================================================================
    zproto.h - protocol framework for ZeroMQ

    Copyright (c) 2014 the Authors
    =========================================================================*/

#ifndef __ZPROTO_H_INCLUDED__
#define __ZPROTO_H_INCLUDED__

//  ZPROTO version macros for compile-time API detection

#define ZPROTO_VERSION_MAJOR 0
#define ZPROTO_VERSION_MINOR 0
#define ZPROTO_VERSION_PATCH 1

#define ZPROTO_MAKE_VERSION(major, minor, patch) \
    ((major) * 10000 + (minor) * 100 + (patch))
#define ZPROTO_VERSION \
    ZPROTO_MAKE_VERSION(ZPROTO_VERSION_MAJOR, ZPROTO_VERSION_MINOR, ZPROTO_VERSION_PATCH)

#include <czmq.h>
#if CZMQ_VERSION < 20100
#   error "ZPROTO needs CZMQ/2.1.0 or later"
#endif

#include "zproto_example.h"

#endif

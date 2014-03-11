/*  =========================================================================
    xrap - a C implementation of UnifyProject/draft/RFC-4

    Copyright (c) 2014 the Authors
    =========================================================================*/

#ifndef __XRAP_H_INCLUDED__
#define __XRAP_H_INCLUDED__

//  XRAP version macros for compile-time API detection

#define XRAP_VERSION_MAJOR 0
#define XRAP_VERSION_MINOR 0
#define XRAP_VERSION_PATCH 1

#define XRAP_MAKE_VERSION(major, minor, patch) \
    ((major) * 10000 + (minor) * 100 + (patch))
#define XRAP_VERSION \
    XRAP_MAKE_VERSION(XRAP_VERSION_MAJOR, XRAP_VERSION_MINOR, XRAP_VERSION_PATCH)

#include <czmq.h>
#if CZMQ_VERSION < 20100
#   error "XRAP needs CZMQ/2.1.0 or later"
#endif

#include "xrap_msg.h"

#endif

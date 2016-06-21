#!/bin/sh
#
#   Script to generate all required files from fresh git checkout.


case `uname` in Darwin*) LIBTOOLIZE=glibtoolize ;;
    *) LIBTOOLIZE=libtoolize ;; esac

command -v $LIBTOOLIZE >/dev/null 2>&1
if  [ $? -ne 0 ]; then
    echo "autogen.sh: error: could not find libtool.  libtool is required to run autogen.sh." 1>&2
    exit 1
fi

command -v autoreconf >/dev/null 2>&1
if [ $? -ne 0 ]; then
    echo "autogen.sh: error: could not find autoreconf.  autoconf and automake are required to run autogen.sh." 1>&2
    exit 1
fi

mkdir -p ./config
if [ $? -ne 0 ]; then
    echo "autogen.sh: error: could not create directory: ./config." 1>&2
    exit 1
fi

autoreconf --install --force --verbose -I config
if [ $? -ne 0 ]; then
    echo "autogen.sh: error: autoreconf exited with status $?" 1>&2
    exit 1
fi

#!/bin/bash

# $BUILD_DIR points to directory holding sources
BUILD_DIR=${BUILD_DIR-.}
cd "$BUILD_DIR" || exit 1
gsl "$@"

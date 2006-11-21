#!/bin/sh
cp ../src/basic.c basic1.c
patch -p0 --binary < basic.patch

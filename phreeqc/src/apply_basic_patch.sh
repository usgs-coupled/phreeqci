#!/bin/sh
mv basic1.c basic1.c.prev
dos2unix basic.c
cp basic.c basic1.c
patch < basic_to_basic1.patch
unix2dos basic.c basic1.c
#####################################
## to recreate basic_to_basic1.patch
##  dos2unix basic.c basic1.c
##  diff -u basic.c basic1.c > basic_to_basic1.patch
##  textpad basic_to_basic1.patch => change basic.c to basic1.c
##  unix2dos basic.c basic1.c
#####################################

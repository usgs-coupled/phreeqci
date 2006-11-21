#!/bin/sh
cp ../src/basic.c .
diff -u --binary basic.c basic1.c > basic.patch
rm basic.c

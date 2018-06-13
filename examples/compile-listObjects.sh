#!/bin/sh
gcc -DHAVE_CONFIG_H -I. -I../src    -I../src/pwr -g -O3 -MT listObjects-listObjects.o -MD -MP -MF .deps/listObjects-listObjects.Tpo -c -o listObjects-listObjects.o `test -f 'listObjects.c' || echo './'`listObjects.c
mv -f .deps/listObjects-listObjects.Tpo .deps/listObjects-listObjects.Po
/bin/bash ../libtool  --tag=CC   --mode=link gcc -I../src/pwr -g -O3   -o listObjects listObjects-listObjects.o ../src/pwr/libpwr.la 

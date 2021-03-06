#!/bin/sh

# CC=gcc-mp-6 ./configure --enable-unsafe --enable-gcov=gcov-mp-6 --disable-shared --enable-debug && gmake
#./configure --enable-gcov --disable-shared --enable-unsafe --enable-debug && make -s -j4 gcov

# fixup and post-process .gcov

perl -pi -e's{Source:(\w+)/}{Source:}' src/*/*.gcov && \
    gcov2perl src/*/*.gcov && \
    cover -no-gcov

#!/bin/bash
POPPLER=poppler-0.60.1
rootdir=$(cd $(dirname $0); pwd)
cd $rootdir
if [ ! -d $rootdir/$POPPLER ]; then
    tar zxvf $POPPLER.tar.gz && ln -s $POPPLER poppler 
fi
mkdir -p $POPPLER/build-ninja && cd $POPPLER && ln -fs build-ninja build && \
cd build-ninja && cmake -G Ninja .. && ninja -C . all

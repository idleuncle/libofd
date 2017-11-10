#!/bin/bash
FONTFORGE=fontforge-20161005
rootdir=$(cd $(dirname $0); pwd)
cd $rootdir
if [ ! -d $rootdir/$FONTFORGE ]; then
    tar zxvf $FONTFORGE.tar.gz && ln -s $FONTFORGE fontforge 
fi
cd $FONTFORGE && ./configure && make

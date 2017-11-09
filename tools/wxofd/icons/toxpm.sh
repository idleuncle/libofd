#!/bin/bash
svg_imags=(`ls *.svg`)
for svg in ${svg_imags[@]}; do
    convert $svg $(basename $svg .svg).xpm
done

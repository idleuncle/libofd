#!/bin/bash
xpm_imags=(`ls *.xpm`)
for x in ${xpm_imags[@]}; do
    xpm=${x//-/_}
    echo $xpm
    filename=$(basename $xpm .xpm)
    echo $filename
    echo sed -i -e "s/static char \*$filename/static const char \*$filename\_xpm/g" $x
    sed -i -e "s/static char \*$filename/static const char \*$filename\_xpm/g" $x
    mv $x $xpm
done

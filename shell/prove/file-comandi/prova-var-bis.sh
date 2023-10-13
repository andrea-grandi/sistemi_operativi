#!/bin/sh
echo 'Il valore di x è ===>' $x 
echo 'Il valore di y è ===>' $y
echo 'Il valore di z è ===>' $z

z=`expr $z + 100`

echo 'Il nuovo valore di z è ===>' $z


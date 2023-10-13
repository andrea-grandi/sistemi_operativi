#!/bin/sh
#file comandi con altro esempio di for
#supponiamo che nel file ttt siano stati memorizzati i nomi di alcuni file
for i in `cat ttt`
do 
echo ELEMENTO CORRENTE: $i 
if test -f $i
then cat $i
fi
done


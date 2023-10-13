#!/bin/sh
#verifica che il numero dei parametri sia maggiore o uguale a 1
#riporta su std output tutti i parametri passati

case $# in
	0) echo error 
		exit 2 ;;
	*) ;;
esac

a=1
for i in $* 
do

	echo $i '$'$a
       	a=`expr $a + 1`

done

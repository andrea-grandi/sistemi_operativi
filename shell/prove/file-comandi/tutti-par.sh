#!/bin/sh
#verifica che il numero dei parametri sia maggiore o uguale a 1
#riporta su std output tutti i parametri passati

case $# in
	0) echo error 
		exit 2 ;;
	*) ;;
esac

echo $#

i=0

for i in $* 
do

	echo $i

done

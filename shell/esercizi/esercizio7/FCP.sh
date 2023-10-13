#!/bin/sh
#file comandi principale

#controllo numero parametri
case $# in 
	0 | 1 | 2) echo error: numero parametri errato
		exit 1;;
	*) echo OK;;
esac

#controllo primo parametro (singolo carattere)
case $1 in 
	?) echo OK;;
	*) echo error: non singolo carattere
		exit 2;;
esac

#salvo il carattere
C=$1
shift

#controllo sulle gerarchie
for i in $*
do 
	case $i in 
		/*) if test ! -d $i -o ! -x $i
			then 
				echo error: non directory o non traversabile
				exit 3
			fi;;
		*) echo error: non nome assoluto
			exit 4;;
	esac
done

#setto la variabile PATH
PATH=`pwd`:PATH
export PATH

> /tmp/conta$$

for i
do

done




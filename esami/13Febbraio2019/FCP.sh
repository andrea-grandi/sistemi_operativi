#!/bin/sh
#file comandi principale

#controllo numero di parametri
case $# in 
	0|1|2) echo error: numero parametri errato
		exit 1;;
	*);;
esac

#controllo primo parametro, nome assoluto
case $1 in 
	/*)
		if test ! -d $1 -o ! -x $1
		then 
			echo error: non directory o non traversabile
			exit 2
		fi;;
	*) 
		echo error: non nome assoluto
		exit 3;;
esac

G=$1
shift

#controllo gli altri N parametri
for i in $*
do 
	case $i in 
		*/*)
			echo error: non nome relativo semplice
			exit 4;;
		*);;
	esac
done

#fine controllo parametri
#settiamo la variabile PATH
PATH=`pwd`:PATH
export PATH

#chiamo il file comandi ricorsivo
FCR.sh $G $*


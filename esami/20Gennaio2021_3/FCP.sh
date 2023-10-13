#!/bin/sh
#file comandi principale

#controllo sul numero di parametri
case $# in 
	0|1|2)
		echo error: numero parametri errato
		exit 1;;
	*)
		echo OK;;
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

#salviamo il parametro e facciamo lo shift
G=$1
shift

#controllo nomi relativi semplici
for i in $*
do
	case $i in 
		*/*) 
			echo error: non nome relativo semplice
			exit 4;;
		*)
			echo OK;;
	esac
done

#fine controllo parametri
#settiamo la varibile PATH
PATH=`pwd`:PATH
export PATH

#invoco la parte ricorsiva
FCR.sh $G $*

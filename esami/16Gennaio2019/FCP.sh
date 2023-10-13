#!/bin/sh
#file comandi principale

#controllo numero parametri
case $# in 
	2);;
	*) echo error: numero parametri errato
		exit 1;;
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

#controllo secondo parametro, stringa
case $2 in 
	*/*)
		echo error: non nome relativo semplice
		exit 4;;
	*);;
esac

F=$2

#fine controllo parametri
#settiamo la variabile PATH
PATH=`pwd`:PATH
export PATH

#invochiamo la parte ricorsiva
FCR.sh $G $F

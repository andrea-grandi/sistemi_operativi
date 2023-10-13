#!/bin/sh
#file comandi principale

#controllo numero di parametri
case $# in 
	*) echo error: numero parametri errato
		exit 1;;
	2);;
esac

#controllo primo parametro
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

#salviamo la gerarchia dentro ad una variabile
G=$1

#controlliamo il secondo parametro, file leggibile
case $2 in
	/*) 
		if test ! -f $2 -o ! -r $2
		then 
			echo error: non file leggibile
			exit 4
		fi;;
	*)
		echo error: non nome assoluto 
		exit 5;;
esac

#fine controlli parametri
#settiamo la variabile PATH
PATH=`pwd`:PATH
export PATH

D=`wc -c < $2`
AF=$2

#chiamo la parte ricorsiva
FCR.sh $G $AF $D

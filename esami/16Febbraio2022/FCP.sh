#!/bin/sh
#file comandi principale

#controllo numero parametri
case $# in 
	0|1|2) echo error: numero parametri errato
		exit 1;;
	*);;
esac

#controllo primo parametro, carattere alfabetico  minuscolo
case $1 in 
	?) 
		case $1 in 
			[!a-z]) echo error: non alfabetico minuscolo
				exit 2;;
			*);;
		esac;;
	*)
		echo error: non singolo carattere
		exit 3;;
esac

C=$1
shift

#controllo gerarchie
for i in $*
do 
	case $i in 
		/*)
			if test ! -d $i -o ! -x $i
			then 
				echo error: non directory o non traversabile
				exit 4
			fi;;
		*)
			echo error: non nome assoluto
			exit 5;;
	esac
done

#fine controllo parametri, settiamo PATH
PATH=`pwd`:PATH
export PATH

> /tmp/nomiAssoluti

#fasi
for G
do 
	echo fase per $G
	FCR.sg $G $C /tmp/nomiAssoluti
done

#controlliamo se sono stati trovati almeno due file
Nfiles=`wc -l < /tmp/nomiAssoluti`
echo il numero di file trovati e: $Nfiles

if test $files -ge 2
then 
	echo invochiamo la parte C
	main.c `cat /tmp/nomiAssoluti` $C
fi

rm /tmp/nomiAssoluti

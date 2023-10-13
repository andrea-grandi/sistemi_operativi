#!/bin/sh
#file comandi principale

#controllo numero di parametri
case $# in 
	0|1|2)echo error: numero parametri errato
		exit 1;;
	*);;
esac

#controllo primo parametro, numero positivo minore di 100
expr $1 + 0 > /dev/null 2>&1
N1=$?
if test $N1 -ne 2 -o $N1 -ne 3
then 
	if test $1 -ge 1 -a $1 -lt 100
	then 
		echo OK
	else
		echo error: numero non compreso tra 0 e 100
		exit 2
	fi
else
	echo error: non numero
	exit 3
fi

L=$1
shift

#controllo gerarchie, nomi assoluti
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

#fine controllo parametri
#settiamo la variabile PATH
PATH=`pwd`:PATH
export PATH

> /tmp/nomiAssoluti

#fasi
for i 
do
	echo fase per $i
	FCR.sh $i $L /tmp/nomiAssoluti
done

#invochiamo la parte c
main.c `cat /tmp/nomiAssoluti`

rm /tmp/nomiAssoluti

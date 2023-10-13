#!/bin/sh
#file comandi principale

#controllo numero di parametri
case $# in 
	0|1|2) echo error: numero parametri errato
		exit 1;;
	*);;
esac

#controllo primo parametro, numero compreso tra 1 e 255
expr $1 + 0 > /dev/null 2>&1
N1=$?
if test $N1 -ne 2 -o $N2 -ne 3
then 
	if test $1 -gt 1 -a $1 -le 255
	then 
		echo OK
	else 
		echo error: numero non compreso tra 1 e 255
		exit 2
	fi
else
	echo error: non numero
	exit 3
fi

H=$1
shift

#controllo nomi assoluti
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

#fine controlli parametri
#settiamo la variabile PATH
PATH=`pwd`:PATH
export PATH

#fasi
for i 
do 
	echo FASE PER $i
	FCR.sh $i $H
done

#!/bin/sh
#file comandi principale

#controllo numero parametri
case $# in 
	0|1|2|3)
		echo error: numero parametri errato
		exit 1;;
	*)
		;;
esac

#controllo primi due parametri, numeri interi strettamente positivi
exec $1 + 0 > /dev/null 2>&1
N1=$?
if test $N1 -ne 2 -o $N1 -ne 3
then 
	if test $1 -le 0
	then 
		echo error: numero non positivo
		exit 2
	fi
else
	echo error: non numero
	exit 3
fi

H=$1

exec $2 + 0 > /dev/null 2>&1
N2=$?
if test $N2 -ne 2 -o $N2 -ne 3
then 
	if test $2 -le 0
	then 
		echo error: numero non positivo
		exit 4
	fi
else
	echo error: non numero
	exit 5
fi

M=$2
shift 
shift

#controllo gerarchie
for i in $*
do 
	case $i in 
		/*)
			if test ! -d $i -o ! -x $i
			then 
				echo error: non directory o non traversabile
				exit 6
			fi;;
		*)
			echo error: non nome assoluto
			exit 7;;
	esac
done

#settiamo la variabile PATH
PATH=`pwd`:PATH
export PATH

#ciclo per le fasi
for G 
do 
	echo FASE PER $G
	FCR.sh $G $H $M
done


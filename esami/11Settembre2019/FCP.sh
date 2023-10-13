#!/bin/sh
#file comandi principale

#controllo numero di parametri
case $# in 
	0|1|2|3) echo error: numero parametri errati
		exit 1;;
	*);;
esac

#controllo primo parametro, numero strettamente positivo
expr $1 + 0 > /dev/null 2>&1
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

#salviamo il primo parametro e facciamo lo shift
H=$1
shift

#controllo secondo parametro (che ora e' primo), numero tra 1 e 255
expr $1 + 0 > /dev/null 2>&1
N2=$?
if test $N2 -ne 2 -o $N2 -ne 3
then 
	if test $1 -le 1 -o $1 -gt 255
	then 
		echo error: numero non compreso tra 1 e 255
		exit 4
	fi
else
	echo error: non numero
	exit 5
fi

#salviamo il secondo parametro e facciamo lo shift
K=$1
shift

#controllo altri W parametri, nomi assoluti
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

#fine controllo parametri
#settiamo la variabile PATH
PATH=`pwd`:PATH
export PATH

#fasi
for i 
do
	echo fase per $i
	FCR.sh $i $H $K
done




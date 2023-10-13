#!/bin/sh
#file comandi principale

#controllo numero parametri
case $# in
	0|1|2|3)echo error: numero parametri errato
		exit 1;;
	*);;
esac

#controllo primi due parametri, numeri interi positivi
expr $1 + 0 > /dev/null 2>&1
N1=$?
if test $N1 -ne 2 -o $N1 -ne 3
then
	if test $1 -le 0
	then 
		echo error: non positivo
		exit 2
	fi
else 
	echo error: non numero
	exit 3
fi

H=$1

expr $2 + 0 > /dev/null 2>&1
N2=$?
if test $N2 -ne 2 -o $N2 -ne 3
then 
	if test $2 -le 0 -o $2 -le $H
	then 
		echo error: non positivo o non maggiore di H
		exit 4
	fi
else
	echo error: non numero
	exit 5
fi

K=$2

#abbiamo controllato i due numeri, ora facciamo lo shift 
shift 
shift

#controllo sulle gerarchie, nomi assoluti
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

#fine controlli sui parametri
#settiamo la variabile PATH
PATH=`pwd`:PATH
export PATH

#iniziamo le fasi
for i 
do
	echo fase per $i
	FCR.sh $i $H $K
done

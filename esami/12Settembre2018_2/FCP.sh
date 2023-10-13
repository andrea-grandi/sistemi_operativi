#!/bin/sh
#file comandi principale

#controllo numero parametri
case $# in 
	0|1|2|3)echo error: numero parametri errato
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
		echo error: non positivo
		exit 2
	fi
else
	echo error: non numero
	exit 3
fi
H=$1

#controllo secondo parametro, numero positivo
expr $2 + 0 > /dev/null 2>&1
N2=$?
if test $N2 -ne 2 -o $N2 -ne 3
then 
	if test $2 -le 0
	then 
		echo error: non positivo
		exit 4
	fi
else
	echo error: non numero
	exit 5
fi

K=$2

if test $H -ge $K
then 
	echo error: $H non strettamente minore di $K
	exit 6
fi

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
				exit 7
			fi;;
		*)
			echo error: non nome assoluto
			exit 8;;
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


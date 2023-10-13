#!/bin/sh
#file comandi principale

#controllo sul numero dei parametri
case $# in 
	0 | 1 | 2 | 3) echo error: numero parametri errato
		exit 1;;
	*) ;;
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

W=$1
shift

#controllo secondo parametro, stringa
S=$1
case $S in 
	*/*) echo error: non nome relativo semplice
		exit 4;;
	*) ;;
esac

shift

#controllo restanti parametri, gerarchie
for i in $*
do 
	case $i in 
		/*) if test ! -d $i -o ! -x $i
			then
			       echo error: non directory o non traversabile
		       	       exit 5
			fi;;
		*) echo error: non nome assoluto
			exit 6;;
	esac
done

#fine controlli
#settiamo la variabile PATH
PATH=`pwd`:PATH
export PATH

> /tmp/nomiAssoluti

#fasi
for i 
do 
	echo FASE PER $i
	FCR.sh $i $S /tmp/nomiAssoluti
done

TOT=`wc -l < /tmp/nomiAssoluti`
echo IL NUMERO TOTALE DI DIRECTORY TROVATE = $TOT

if test $TOT -ge $W
then 
	echo -n "Andrea, dammi un numero intero compreso tra 1 e $W"
	read X

	#controllo X
	expr $X + 0 > /dev/null
	NX=$?
	if test $NX -ne 2 -o $NX -ne 3
	then 
		if test $X -lt 1 -o $X -gt $W
		then 
			echo error: numero non compreso tra 1 e $X
			rm /tmp/nomiAssoluti
			exit 7
		fi
	else
		echo error: non mi hai dato un numero
		rm /tmp/nomiAssoluti
		exit 8
	fi
	#fine controllo X
	echo -n il nome assoluto della directory corrispondente a $K =
	head -$X < /tmp/nomiAssoluti | tail -1
else
	echo $TOT minore di $W
fi

rm /tmp/nomiAssoluti
#fine file comandi principale


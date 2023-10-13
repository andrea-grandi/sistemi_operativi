#!/bin/sh
#file comandi principale

#controllo numero parametri
case $# in 
	0|1|2|3) echo error: numero parametri errato
		exit 1;;
	*);;
esac

#controllo primo parametro, nome relativo semplice 
case $1 in 
	*/*) echo error: non nome relativo semplice
		exit 2;;
	*);;
esac

D=$1
shift

#controllo secondo parametro, numero strettamente positivo
expr $1 + 0 > /dev/null 2>&1
N2=$?
if test $N2 -ne 2 -o $N2 -ne 3
then 
	if test $1 -le 0
	then 
		echo error: non positivo
		exit 3
	fi
else
	echo error: non numero
	exit 4
fi

Y=$1
shift

#controllo directory
for i in $*
do 
	case $i in 
		/*) 
			if test ! -d $i -o ! -x $i
			then 
				echo error: non directory o non traversabile
				exit 5
			fi;;
		*)
			echo error: non nome assoluto
			exit 6;;
	esac
done

#fine controlli, settiamo la variabile PATH
PATH=`pwd`:PATH
export PATH

> /tmp/nomiAssoluti

#invochiamo la parte ricorsiva FCR.sh
for G 
do
	echo fase per $G
	FCR.sh $G $D $Y /tmp/nomiAssoluti
done

echo il numero di file trovati e: `wc -l < /tmp/nomiAssoluti`

#invichiamo la parte c
main.c `cat /tmp/nomiAssoluti` $Y

rm /tmp/nomiAssoluti

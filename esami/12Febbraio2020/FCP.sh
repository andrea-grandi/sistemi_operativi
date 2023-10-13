#!/bin/sh
#file comandi principale

#controllo numero parametri
case $# in 
	0|1|2|3) echo error: numero parametri errato
		exit 1;;
	*);;
esac

#controllo primo parametro, numero positivo dispari
expr $1 + 0 > /dev/null 2>&1
N1=$?
if test $N1 -ne 2 -o $N1 -ne 3
then 
	if test $1 -gt 0 
	then 
		if test `expr $1 % 2` -eq 0
		then 
			echo error: numero non dispari
			exit 2
		fi
	else
		echo error: numero non positivo
		exit 3
	fi
else
	echo error: non numero
	exit 4
fi

K1=$1

#controllo secondo parametro, numero positivo pari
expr $2 + 0 > /dev/null 2>&1
N2=$?
if test $N2 -ne 2 -o $N2 -ne 3
then 
	if test $2 -ge 0
	then
		if test `expr $2 % 2` -ne 0
		then 
			echo error: numero non pari
			exit 5
		fi
	else
		echo error: numero non positivo
		exit 6
	fi
else
	echo error: non numero
	exit 7
fi

K2=$2
shift 
shift

#controllo W parametri, nomi assoluti
for i in $*
do 
	case $i in
		/*) 
			if test ! -d $i -o ! -x $i
			then
				echo error: non directory o non traversabile
				exit 8
			fi;;
		*)
			echo erro: non nome assoluto
			exit 9;;
	esac
done

#settiamo la variabile PATH
PATH=`pwd`:PATH
export PATH

> /tmp/trovati

#fasi 
for i 
do 
	echo fase per $i
	FCR.sh $i $K1 $K2 /tmp/trovati
done

#controlliamo che i file trovati siano uno dispari e uno pari
L=1
for i in `cat /tmp/trovati`
do 
	if test `expr $L % 2` -eq 0
	then 
		#ci troviamo su una linea pari
		if test `wc -c < $i` -ne $K2
		then 
			echo error: ordine del file tmp errata
			exit 10
		fi
	else
		#ci troviamo su una linea dispari
		if test `wc -c < $i` -ne $K1
		then 
			echo error: ordine del file tmp errata
			exit 11
		fi
	fi

done

#ora invochiamo la parte C
main.c `cat /tmp/trovati`

rm /tmp/trovati

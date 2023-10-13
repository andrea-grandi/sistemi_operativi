#!/bin/sh
#file comandi principale

#controllo numero parametri
case $# in 
	0|1|2)
		echo error: numero parametri errato
		exit 1;;
	*)
		;;
esac

#controllo primo parametro, numero positivo
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

#salviamo il primo parmaetro
H=$1
shift

#controllo gerarchie
for i in $*
do 
	case $i in 
		/*)
			if test ! -d $i -o ! -x $i
			then 
				echo error: non directory
				exit 4
			fi;;
		*)
			echo erro: non nome assoluto
			exit 5;;
	esac
done

#fine controlli
#settiamo la variabile PATH
PATH=`pwd`:PATH
export PATH

#fasi
for G 
do
	#creiamo il file tmp
	> /tmp/nomiAssoluti

	echo fase per $G
	FCR.sh $G $H /tmp/nomiAssoluti

	#riportiamo su stdout i nomi assoluti
	NN=`wc -l < /tmp/nomiAssoluti`
	echo numero nomi assoluti trovati: $NN

	if test `expr $NN % 2` -eq 0
	then
		echo invoco la parte c
		main `cat /tmp/nomiAssoluti`
	fi
done

rm /tmp/nomiAssoluti

#!/bin/sh
#file comandi principale

#controllo sul numero dei parametri
case $# in 
	0|1|2) echo error: numero parametri errato
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
		echo error: numero non positivo
		exit 2
	fi
else
	echo error: non numero
	exit 3
fi

#salviamo il numero e facciamo lo shift
L=$1
shift

#controllo gerarchie
for i in $*
do 
	case $i in 
		/*)
			if test ! -d $i -o ! -x $i
			then 
				echo error: non directori o non traversabile
				exit 4
			fi;;
		*)
			echo error: non nome assouto
			exit 5;;
	esac
done
#fine controllo parametri, settiamo la variabile PATH
PATH=`pwd`:PATH
export PATH

#fasi
for i 
do 
	> /tmp/nomiAssoluti$$ 
	echo fase per $i
	FCR.sh $i $L /tmp/nomiAssoluti$$
	echo il numero di file trovati e: `wc -l < /tmp/nomiAssoluti$$`
	for F in `cat /tmp/nomiAssoluti$$`
	do
		#invochiamo la parte c
		main.c $F $L
	done
done

rm /tmp/nomiAssoluti$$

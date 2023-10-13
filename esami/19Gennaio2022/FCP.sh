#!/bin/sh
#file comandi principale

#controllo numero di parametri
case $# in 
	0|1|2|3) 
		echo error: numero parametri errato
		exit 1;;
	*)
		;;
esac

#controllo primo parametro, numero positivo dispari
expr $1 + 0 > /dev/null 2>&1
N1=$?
if test $N1 -ne 2 -o $N1 -ne 3
then 
	if test $1 -le 0 -o `expr $1 % 2` -eq 0
	then 
		echo error: non positivo o non dispari
		exit 2
	fi
else
	echo error: non numero
	exit 3
fi

#controllo secondo parametro, numero positivo pari
expr $2 + 0 > /dev/null 2>&1
N2=$?
if test $N2 -ne 2 -o $N2 -ne 3
then 
	if test $2 -le -o `expr $2 % 2` -eq 1
	then 
		echo error: non positivo o non pari
		exit 4
	fi
else
	echo error: non numero
	exit 5
fi

#salviamo i primi due numeri e facciamo lo shift
C=$1
X=$2
shift
shift

#controlliamo gli altri Q parametri, nomi assoluti
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

#creiamo un file temporaneo
> /tmp/nomiAssoluti

#fasi
for G
do 
	echo fase per $G
	FCR.sh $G $C $X /tmp/nomiAssoluti	
done

#riportiamo il numero totale di directory trovate 
echo il numero totale di directory trovate e: `wc -l < /tmp/nomiAssoluti`

#invochiamo la parte C
main.c `cat /tmp/nomiAssoluti` $C

rm /tmp/nomiAssoluti

	

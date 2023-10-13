#!/bin/sh
#file comandi principale

#controllo numero parametri
case $# in 
	0|1|2|3) echo error: numero parametri errato
	       	 exit 1;;
	*);;
esac

#controllo primo parametro, numero intero strettamente positivo
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

#salviamo il numero in una variabile e facciamo lo shift
H=$1
shift

#controllo secondo parametro, numero intero strettamente maggiorew di 1 ma minore di 255
expr $1 + 0 > /dev/null 2>&1
N2=$?
if test $N2 -ne 2 -o $N2 -ne 3
then 
	if test $1 -gt 1 -a $1 -lt 255
	then 
		K=$1
	else
		echo error: numero non compreso tra 1 e 255
		exit 4
	fi
else
	echo error: non numero
	exit 5
fi

#ora facciamo lo shift
shift

#controllo restanti W parametri, nomi assoluti 
for i in $*
do 
	case $i in 
		/*) 
			if test ! -d $i -o ! -x $i
			then 
				echo error: non directory o non traversabile
				exit 6
			fi;;
		*) echo error: non nome assoluto
			exit 7;;
	esac
done

#fine controlli
#settiamo la variabile PATH
PATH=`pwd`:PATH
export PATH

#fasi
for i 
do
	echo fase per $i
	FCR.sh $i $H $K
done




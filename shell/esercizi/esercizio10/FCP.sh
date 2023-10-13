#!/bin/sh
#file comandi principale 9 Febbraio 2015

#controllo sul numero dei parametri
case $# in 
	2) echo OK;;
	*) echo error: numero parametri errato
		exit 1;;
esac

#controllo primo parametro
case $1 in 
	/*) if test ! -d $1 -o ! -x $1
		then 
		       echo error: non directory o non traversabile
	       	       exit 2
		fi;;
	*) echo error: non nome assoluto
		exit 3;;
esac

#controllo secondo parametro
expr $2 + 0 > /dev/null 2>&1
N1=$?
if test $N1 -ne 2 -o $N1 -ne 3
then 
	if test $N1 -le 0
	then 
		echo error: non positivo 
		exit 4
	fi
else
	echo error: non numero
	exit 5
fi

#settiamo la variabile PATH
PATH=`pwd`:PATH
export PATH

> /tmp/tmp$$

#chiamiamo il file comandi ricorsivo
FCR.sh $1 $2 /tmp/tmp$$

for i in `cat /tmp/tmp$$`
do 
	params="$params $i"
	echo -n "Dammi un numero intero strettamente positivo minore o uguale di $2: "
	read X
	#controlliamo X
	expr $X + 0 > /dev/null 2>&1
	N1=$?	
	if test $N1 -ne 2 -a $N1 -ne 3
	then 
		if test $X -le 0
		then 
			echo error: non positivo
			rm /tmp/tmp$$
			exit 6
		fi
	else
		echo error: non numero
		rm /tmp/tmp$$
		exit 7
	fi
	if test $X -gt $2
	then 
		echo error: minore di $2
		rm /tmp/tmp$$
		exit 8
	fi
	params="$params $X"
done

rm /tmp/tmp$$

#fine file comandi principale, ora si chiama la parte in C

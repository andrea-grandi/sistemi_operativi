#!/bin/sh
#file comandi principale

#controllo numero di parametri
case $# in 
	0|1|2)echo error: numero parametri errato
		exit 1;;
	*);;
esac

#controllo primo parametro, singolo carattere
Cz=$1
case $1 in 
	?) echo OK;;
	*) echo error: non singolo carattere
		exit 2;;
esac

shift

#controllo gerarchie
for i in $*
do 
	case $i in 
		/*)
			if test ! -d $i -o ! -x $i
			then 
				echo error: non directory o non traversabile
				exit 3
			fi;;
		*)
			echo error: non nome assoluto
			exit 4;;
	esac
done

#settiamo la variabile PATH
PATH=`pwd`:PATH
export PATH

#creo un file tmp per salvare i nomi assoluti trovati dal file ricorsivo
> /tmp/nomiAssoluti

#fasi
for i 
do	
	echo fase per $i
	FCR.sh $i $Cz /tmp/nomiAssoluti
done

#invochiamo la parte c
main.c `cat /tmp/nomiAssoluti` $Cz

rm /tmp/nomiAssoluti

#!/bin/sh
#file comandi principale

#controllo numero parametri
case $# in 
	0|1|2) echo error: numero parametri errato
		exit 1;;
	*) ;;
esac

#controllo primo parametro, singolo carattere
Cz=$1
case $1 in 
	?) ;;
	*) echo error: non singolo carattere
		exit 2;;
esac

shift

#controllo altri parametri, nomi assoluti
for i in $*
do 
	case $i in 
		/*) 
			if test ! -d $i -o ! -x $i
			then
				echo error:  non directory o non traversabile
				exit 3
			fi;;
		*) 
			echo error: non nome assoluto
			exit 4;;
	esac
done
#fine controlli parametri
#settiamo la variabile PATH
PATH=`pwd`:PATH
export PATH

> /tmp/nomiAssoluti

#fasi
for i 
do
	echo fase per $i
	FCR.sh $i $Cz /tmp/nomiAssoluti
done

echo il numero totale di file trovati e: `wc -l < /tmp/nomiAssoluti`
#chiamo la parte in C
#file.c `cat /tmp/nomiAssoluti` $Cz

rm /tmp/nomiAssoluti

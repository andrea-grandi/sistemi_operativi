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

#controllo primo parametro, singolo carattere
case $1 in 
	?)
		;;
	*)
		echo error: primo parametro non singolo carattere
		exit 2;;
esac

#salviamo il primo parametro e facciamo lo shift
Cz=$1
shift

#controllo altri parametri, nomi assoluti
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

#fine controllo parametri
#settiamo la variabile PATH
PATH=`pwd`:PATH
export PATH

#creiamo un file temporaneo per i nomi assoluti
> /tmp/nomiAssoluti

#fasi
for G
do 
	echo fase per $G
	FCR.sh $G $Cz /tmp/nomiAssoluti
done

#invochiamo la parte c
main `cat /tmp/nomiAssoluti`

rm /tmp/nomiAssoluti

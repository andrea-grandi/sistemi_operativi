#!/bin/sh
#file comandi principale

#controllo numero parametri
case $# in 
	0|1|2)
		echo error: numero di parametri errato
		exit 1;;
	*)
		;;
esac

#controllo primo parametro, nome assoluto
case $1 in 
	/*)
		if test ! -d $1 -o ! -x $1
		then 
			echo error: non directory o non traversabile
			exit 2
		fi;;
	*)
		echo error: non nome assoluto
		exit 3;;
esac

#salviamo questo parametro e facciamo lo shift
G=$1
shift

#controllo altri parametri, singoli caratteri
for i in $*
do 
	case $i in 
		?)
			;;
		*)
			echo error: non singoli caratteri
			exit 4;;
	esac
done

#fine controlli
#settiamo la variabile PATH
PATH=`pwd`:PATH
export PATH

#invochiamo la parte ricorsiva 
FCR.sh $G $*


#!/bin/sh
#file comandi principale

#controllo numero dei parametri
case $# in 
	0|1|2) 
		echo error: numero parametri errato
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

#salviamo il primo parametro e facciamo lo shift
G=$1
shift

#controllo altri Q parametri, nomi relativi semplici DI FILE
for i in $*
do 
	case $i in 
		*/*)
			echo error: non nome relativo semplice
			exit 4;;
		*) 
			if test ! -f $i
			then 
				echo error: non file
				exit 5
			fi;;
	esac
done

#fine controlli sui parametri
#settiamo la variabile PATH
PATH=`pwd`:PATH
export PATH

#parte ricorsiva
FCR.sh $G $*

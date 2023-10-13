#!/bin/sh
#file comandi principale

#controllo numero di parametri
case $# in 
	0|1|2) echo error: numero parametri errato
		exit 1;;
	*) ;;
esac

#controllo primo parametro, singolo carattere
Cz=$1
case $1 in
	?) case $1 in 
		*/*)echo error: non nome relativo semplice
			exit 2;;
		*) ;;
	   esac;;
	*) echo error: non singolo carattere
		exit 3;;
esac
shift

#controllo nomi assoluti
for i in $*
do 
	case $i in 
		/*) 
			if test ! -d $i -o ! -x $i
			then
				echo error: non directory o non traversabile
				exit 4;;
			fi;;
		*) echo error: non nome assoluto
			exit 5;;
	esac
done
#fine controlli parametri
#settiamo la variabile PATH
PATH=`pwd`:PATH
export PATH

> /tmp/nomiAssoluti

#ora iniziamo la parte ricorsiva 
for i 
do 
	echo fase per $i
	FCR.sh $i $Cz /tmp/nomiAssoluti
done

#riportiamo su std output il numero  dei file trovati
echo il numero totale di file trovati è: `wc -l < /tmp/nomiAssoluti`

#invochiamo la parte c
#file.c $Cz `cat /tmp/nomiAssoluti`



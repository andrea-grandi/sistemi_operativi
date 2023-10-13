#!/bin/sh
#file comandi principale

#controllo numero dei parametri
case $# in 
	0 | 1 | 2) echo error: numero parametri errato
		exit 1;;
	*) ;;
esac

#controllo primo parametro, singolo carattere
C=$1
case $C in 
	*/*) echo error: non relativo semplice
		exit 2;;
	*) ;;
esac
shift

#controllo sulle gerarchie
for i in $*
do 
	case $i in 
		/*) if test ! -d $i -o ! -x $i
			then 
				echo error: non directory o non traversabile
				exit 3
			fi;;	
		*) echo error: non nome assoluto
			exit 4;;
	esac
done

#fine controllo parametri
#setto la variabile PATH
PATH=`pwd`:PATH
export PATH

> /tmp/nomiAssoluti

#fasi
for i 
do 
	echo fase per $i
	FCR.sh $i $C /tmp/nomiAssoluti
done

#riportiamo il numero totale di directory D trovate
echo Il numero totale di directory trovate = `wc -l < /tmp/nomiAssoluti`

for D in `cat /tmp/nomiAssoluti`
do 
	echo directory trovata = $D
	echo -n "Elena, vuoi visualizzare il contenuto?"
	read answer
	#controllo risposta
	case $answer in 
		s* | S* | y* | Y*) ls -la $D;;
		*) echo nessuna visualizzazione;;
	esac
done

rm /tmp/nomiAssoluti
#fine file comandi principale


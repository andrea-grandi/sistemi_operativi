#!/bin/sh
#file comandi principale

#controllo numero dei parametri
case $# in 
	0 | 1 | 2) echo error: numero dei parametri errato
		exit 1;;
	*) ;;
esac

#controlli primi N parametri, nomi assoluti
num=1
params=

for i in $*
do 
	if test $num -eq $#
	then 
		#siamo arrivati all'ultimo parametro
		#stringa S
		S=$i
		case $S in 
			*/*) echo error: non relativo semplice
				exit 2;;
			*);;
		esac	
	else
		#stiamo ancora esplorando i parametri nomi assoluti
		case $i in 
			/*) if test ! -d $i -o ! -x $i
				then
				       echo error: non directory
				       echo error: non traversabile
			               exit 3
			    fi;;
		        *) echo error: non nome assoluto
	    			exit 4;;
		esac
		params="$params $i"
	fi
	num=`expr $num + 1`
done

#fine controlli
#settiamo la variabile PATH
PATH=`pwd`:PATH
export PATH

> /tmp/conta$$

#fasi
for i 
do 
	echo fase per $i
	FCR.sh $i $S /tmp/conta$$
done

echo Il numero di file trovati globalmente e = `wc -l < /tmp/conta`

l=0 #variabile per contare le linee e vedere se sono pari (nome assoluto) 
# oppure dispari (lunghezza in caratteri)

for i in `cat /tmp/conta$$`
do 
	l=`expr $ + 1`
	if test `expr $l % 2` -eq 1
	then 
		#elemento dispari, quindi lunghezza in caratteri
	        echo lunghezza in caratteri del file: $i
	else	
		#elemento pari, quindi nome assoluto
		echo nome assoluto del file: $i
		echo -n "ordinare file?"
		read risposta
		#controllo risposta
		case $risposta in 
			s* | S* | y* | Y*) sort -f $i;;
			*) echo nessun ordinamento;;
		esac
	fi
done

rm /tmp/conta$$
#fine file comandi principale

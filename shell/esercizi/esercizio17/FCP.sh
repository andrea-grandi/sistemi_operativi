#!/bin/sh
#file comandi principale 5/04/2019

#controllo sul numero dei parametri
case $# in 
	0 | 1 | 2) echo error: numero di parametri errato
		exit 1;;
	*) ;;
esac

#controllo primi N parametri, directory
num=1 #variabile che ci serve per vedere quando siamo arrivati in fondo
params=

for i in $*
do 
	if test $num -ne $#
	then 
		case $i in 
			/*) if test ! -d $i -o ! -x $i
				then
					echo error:
					echo non directory o non traversabile
					exit 2
				fi;;
			*) echo error: non nome assoluto
				exit 3;;
		esac
		params="$params $i"
	else
		#siamo arrivati all'ultimo parametro
		S=$i
		case $S in 
			*/*) echo error: non relativo semplice
				exit 4;;
			*) ;;
		esac
	fi
	num=`expr $num + 1`
done

#fine controlli
#settiamo la variabile PATH
PATH=`pwd`:PATH
export PATH

> tmp/conta$$

#fasi
for i $params 
do 
	echo fase per $i
	FCR.sh $i $S /tmp/conta$$
done

echo Il numero totale di file trovati = `wc -l < /tmp/conta$$`

C=0
for i in `cat /tmp/conta$$`
do 
	C=`expr $C + 1` 
	if test `expr $C % 2` -eq 1
	then 
		echo elemento dispari, lunghezza in caratteri = $i
	else
		echo elemento pari, nome assoluto del file $i
		echo -n "ordinare file?"
		read risposta
		case $risposta in 
			s* | S* | y* | Y*) sort -f $i;;
			*) echo nessuna ordinazione ;;
		esac
	fi	
done

rm /tmp/conta$$


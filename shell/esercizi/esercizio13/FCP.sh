#!/bin/sh
#file comandi principale

#controllo numero dei parametri
case $# in 
	0 | 1 | 2) echo error: numero parametri errato
		exit 1;;
	*) ;;
esac

#controllo primi N parametri
#nomi assoluti 

#numero di directory
num=1
params=

for i
do 
	if test $num -eq $#
	then 
		#controllo i nomi assoluti
		case $i in 
			/*) if test ! -d $i -a ! -x $i
				then 
					echo error: non directory o non traversabile
					exit 2
				fi;;
			*) echo error: non nome assoluto
				exit 3;;
		esac
		params="$params $i"
	else
		S=$i
		case $i in 
			*/*) echo error: non nome relativo semplice
				exit 4;;
			*) ;;
		esac
	fi
	num=`expr $num + 1`	
done

#fine controlli
PATH=`pwd`:PATH
export PATH

> /tmp/conta$$

#fasi 
for i in $params
do 
	echo fase per $i
	FCR.sh $i $S /tmp/conta$$
done

echo Trovati globalmente = `wc -l < /tmp/conta$$`

#per ogni file trovato si riporta la lunghezza in caratteri 
c=0

for i in `cat /tmp/conta$$`
do 
	c=`expr $i + 1`
	if test `expr $c % 2` -eq 1
	then 
		echo elemento dispari, lunghezza in caratteri = $i
	else
		echo elemento pari, quindi nome assoluto
		echo -n "ordinare file?"
		read risposta
		case $risposta in 
			s* | S* | y* | Y*) sort -f $i ;;
			*) echo niente ordinamento;;
		esac
	fi	
done

rm /tmp/conta$$

#fine file comandi principale



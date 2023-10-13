#!/bin/sh
#file comandi principale

#controllo sul numero dei parametri
case $# in 
	0 | 1 | 2) echo error: numero parametri errato
		exit 1;;
	*) ;;
esac

#controllo primo parametro
#singolo carattere 
C=$1
case $C in 
	*/*) echo error: non relativo semplice
		exit 2;;
	*) ;;
esac

shift

#controllo altri N parametri
for i in $*
do 
	case $i in 
		/*) if test ! -d $i -o -x $i
			then
				echo error: non directory o non traversabile
				exit 3
			fi;;	
		*) echo error: non nome assoluto
			exit 4;;
	esac
done

#fine controlli parametri
#settiamo PATH
PATH=`pwd`:PATH
export PATH
> /tmp/conta$$

for i
do 
	echo fase per $i
	FCR.sh $i $C /tmp/conta$$
done

echo il numero totale di directory trovate = `wc -l < /tmp/conta$$`

for i in `cat /tmp/conta$$`
do 
	echo Il nome assoluto delle directory trovate: $i
	echo -n "Andrea vuoi visualizzare il contenuto, compresi gli elementi nascosti?"
	read risposta
	case $risposta in 
		s* | S* | y* | Y*) ls -la $i;;
		*) ;;
	esac
done

rm /tmp/conta$$
#fine file comandi principale


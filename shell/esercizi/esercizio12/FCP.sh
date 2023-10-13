#!/bin/sh
#file comandi principale 

#controllo numero dei parametri
case $# in 
	0 | 1 | 2) echo error: numero parametri errato
		exit 1;;
	*) echo OK;;
esac

#controllo primo parametro
#deve essere un numero positivo 
expr $1 + 0 > /dev/null 2>&1
N1=$?
if test $N1 -ne 2 -o $N1 -ne 3
then 
	if test $1 -le 0
	then 
		echo error: numero negativo
		exit 2
	fi
else
	echo error: non numero
	exit 3
fi

#salviamo il numero nella variabile Y
Y=$1
shift

#controllo altri parametri
for i in $*
do 
	case $i in 
		/*) if test ! -d $i -o ! -x $i
			then
				echo error: non directory o non traversabile
				exit 4
			fi;;	
		*) echo error: non nome assoluto
			exit 5;;
	esac
done

#fine controllo parametri
#settiamo la variabile PATH
PATH=`pwd`:PATH
export PATH

> /tmp/conta$$ #file che contiene f.quinta e f.NOquinta trovati da FCR.sh

#fasi per ogni gerarchia
for i 
do 
	echo fase per $i
	FCR.sh $i $Y /tmp/conta$$
done

#fine chiamate ricorsive
echo Il numero totale di file creati globalmente = `wc -l < /tmp/conta$$`

for i in `cat /tmp/conta$$`
do 
	echo creato il file $i
	case $i in 
		*NO*) echo il file originario non conteneva almeno 5 linee, quindi vuoto;;
		*) echo il contenuto del file  $i e:
			cat < $i;;
	esac
done

#fine file comandi principale
rm /tmp/conta$$



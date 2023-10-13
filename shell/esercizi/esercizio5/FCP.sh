#!/bin/sh
#file comandi principale

#controllo sul numero dei parametri
case $# in 
	0 | 1 | 2) echo error: numero dei parametri errato
		exit 1;;
	*) echo ok;;
esac

#controllo sul primo parametro che deve essere un numero strettamente positivo
expr $1 + 0 > /dev/null 2>&1
N1=$?
if test $N1 -ne 2 -o $N1 -ne 3
then 
	if test $1 -le 0
	then 
		echo $1 non positivo 
		exit 2
	fi
else 
	echo $1 non numero 
	exit 3
fi
#controllo sul primo parametro completato 
#ora faccio lo shift per eliminare il primo parametro da $*
P1=$1
shift 

#controllo altri parametri (nomi assoluti)
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
#controllo directory finito
#setto la variabile PATH
PATH=`pwd`:PATH
export PATH

#creiamo il file temporaneo
> /tmp/conta$$


#file comandi ricorsivo per ogni gerarchia
for i 
do 
	echo fase per $i
	FCR.sh $i $P1 /tmp/conta$$
done

#terminate tutte le ricerche ricorsive
#andiamo a contare le linee nel file /tmp/conta$$
for i in `cat /tmp/conta$$`
do 
	echo creato il file $i
	case $i in 
		*NO*) echo il file e' vuoto;;
		*) echo il contenuto di $i e' il seguente:
			cat < $i;;
	esac
done

#eliminiamo il file temporaneo
rm /tmp/conta$$


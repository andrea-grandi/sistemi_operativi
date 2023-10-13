#!/bin/sh
#file comandi principale
#controllo sul numero di parametri (4)

case $# in 
	0 | 1 | 2 | 3) echo error: numero parametri errato
		exit 1;;
	*) echo OK;;
esac

#controllo primo parametro che deve essere un intero positivo
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

#salvo il primo parametro e faccio shift
W=$1
shift

#controllo secondo parametro che deve essere una stringa 
S=$1
case $S in 
	*/*) echo error: $S non relativo semplice
		exit 4;;
	*) echo OK;;
esac

#facciamo lo shift anchge della stringa
shift

#controllo sulle gerarchie
for i 
do 
	case $i in 
		/*) echo ok;;
		*) echo error: non nome assoluto
			exit 5;;
	esac
done
#fine controllo sui parametri

#settiamo la variabile PATH e generiamo il file temp
PATH=`pwd`:PATH
export PATH

> /tmp/nomiAssoluti

#invocazione della parte ricorsiva
for G
do 
	echo fase per $G
	FCR.sh $G $S /tmp/nomiAssoluti	
done

TOT=`wc -l < /tmp/nomiAssoluti`
echo Il numero di directory trovate nella gerarchia e' = $TOT

#verifico che $TOT sia maggiore o uguale ad $H
if test $TOT -ge $H
then 
	echo "numero tra 1 ed $H"
	read K

	case $K in 
		*[!0-9]*) echo $K non numero o non positivo
			rm /tmp/nomiAssoluti
			exit 6;;
		*) if test $K -lt 1 -o $K -gt $H
		then 
			error: $K non e\' compreso fra 1 e $H
			rm /tmp/nomiAssoluti
			exit 7
			fi
	esac
else 
	echo $TOT non maggire o uguale di $H
fi

rm /tmp/nomiAssoluti


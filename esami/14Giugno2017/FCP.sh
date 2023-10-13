#!/bin/sh
#file comandi principale

#controllo numero parametri
case $# in 
	3) ;;
	*) echo error: numero parametri errato
		exit 1;;
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

#controllo secondo parametro, numero positivo
expr $2 + 0 > /dev/null 2>&1
N2=$?
if test $N2 -ne 2 -o $N2 -ne 3
then 
	if test $2 -le 0
	then 
		echo error: secondo parametro non positivo
		exit 4
	fi
else
	echo error: non numero
	exit 5
fi

#controllo terzo parametro, singolo carattere
case $3 in 
	?) ;;
	*) echo error: non singolo carattere
		exit 6;;
esac

G=$1
K=$2
Cx=$3

#fine controllo parametri, ora settiamo la variabile PATH
PATH=`pwd`:PATH
export PATH

#creiamo un file temporaneo nomiAssoluti da passare al file C
> /tmp/nomiAssoluti

#invochiamo la parte ricorsiva
FCR.sh $G $K $Cx /tmp/nomiAssoluti

#finita la parte ricorsiva stampiamo tutti i nomi assoluti trovati
echo I nomi assoluti trovati sono: `cat /tmp/nomiAssoluti`

#ora invochiamo la parte C
main.c `cat /tmp/nomiAssoluti` $Cx

#cancelliamo il file temporaneo
rm /tmp/nomiAssoluti

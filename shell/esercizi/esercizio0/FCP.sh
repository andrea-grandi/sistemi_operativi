#!/bin/sh
#file comandi principale che risolve la parte shell
#FCP.sh G D

#Controllo sul numero dei parametri
case $# in
	2) ;;
	*) echo ERRORE: Usage is $0 dirass D
		exit 1;;
esac

#Controllo primo parametro
case $1 in 
/*) if test ! -d $1 -o ! -x $1
then 
       echo $1 non directory o nn attraversabile
       exit 2
fi;;
*) echo $1 non nome assoluto 
	exit 3;;
esac

#Controllo secondo parametro
case $2 in 
	*)* echo $2 NON relativo semplice
		exit 4;;
esac

#modifichiamo ed esportiamo la variabile PATH
PATH=`pwd`:$PATH
export PATH

#creiamo un file temporaneo che poi passere al file comandi ricorsivo 
> /tmp/tmp$$

./FCR.sh $* /tmp/tmp$$

#Stampiamo il numero e i nomi dei file trovati
echo Abbiamo trovato `wc -l < /tmp/tmp$$` file che soddisfano la specifica
echo I file sono: `cat /tmp/tmp$$`
echo Adesso chiamiamo la parte C passando cme parametri i file trovati

#Ultima cosa, eliminiamo il file temporaneo
rm /tmp/tmp$$




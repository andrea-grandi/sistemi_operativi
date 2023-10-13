#!/bin/sh
#file comandi principale

#controllo numero dei parametri
case $# in 
	0 | 1 | 2) echo error: numero di parametri errato
		exit 1;;
	*) ;;
esac

#controllo primo parametro, numero strettamente positivo
expr $1 + 0 > /dev/null 2>&1
N1=$?
if test $N1 -ne 2 -o $N1 -ne 3
then 
	if test $1 -le 0
	then 
		echo error: numero non positivo
		exit 2
	fi
else
	echo error: non numero
	exit 3
fi

Y=$1
shift

#controllo altri parametri, gerarchie
for i in $*
do 
	case $i in 
		/*) if test ! -d $i 
			then
			       echo error: non directory
			       exit 4
			fi
		    if test ! -x $i
		    then 	    
		   	 echo error: non traversabile
	  		 exit 5
		    fi;;
	    *) echo error: non nome assoluto
		    exit 6;;
    esac
done

#fine controlli
#settiamo la variabile PATH e generiamo un file temporaneo
PATH=`pwd`:PATH
export PATH

> /tmp/conta$$

#fasi 
for i
do 
	echo fase per $i
	FCR.sh $i $Y /tmp/conta$$
done

#numero di file trovati globalmente
echo IL NUMERO TOTALE DI FILE TROVATI E = `wc -l < /tmp/conta$$`

for i in `cat /tmp/conta$$`
do 
	echo creato il file $i
	case $i in
		*NO*) echo il file originale non conteneva almeno 5 linee
		      echo quindi file $i e vuoto;;
	        *) echo il contenuto del file $i e cat < $i;;
	esac		
done

rm /tmp/conta$$


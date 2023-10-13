#!/bin/sh
#file comandi principale 

#controllo sul numero dei parametri
case $# in 
	0 | 1 | 2 | 3) echo error: numero dei parametri errato
		exit 1;;
	*) ;;
esac

#controllo primo parametro, numero positivo
expr $1 + 0 > /dev/null 2>&1
N1=$?
if test $N1 -ne 2 -o $N1 -ne 3
then 
	if test $1 -le 0
	then 
		echo error: non positivo
		exit 2
	fi
else
	echo error: non numero
	exit 3
fi

#salviamo il numero e facciamo uno shift
W=$1
shift

#controllo secondo parametro, stringa
S=$1 #$1 perche ho fatto lo shift 
case $S in 
	*/*) echo error: non nome relativo semplice
		exit 4;;
	*);;
esac
shift

#controllo sulle restanti gerarchie
for i in $*
do 
	case $i in
		/*) if test ! -d $i -o ! -x $i
			then
			       echo error: non directory o non traversabile
		       	       exit 5
			fi;;
	 	*) echo error: non nome assoluto
			exit 6;;
	esac	
done

#fine controlli parametri
#settiamo la variabile PATH
PATH=`pwd`:PATH
export PATH

#file temporaneo per contare i file .S
> /tmp/conta$$

#fasi
for G
do
	echo fase per $G
	FCR.sh $G $S /tmp/conta$$
done

#riportiamo sullo standard output il numero totale di directory
#trovate globalmente
echo Il numero di directory trovate TOT=`wc -l < /tmp/conta$$`
echo $TOT

if test $TOT -ge $W
then 
	echo -n "Andrea scegli un numero tra 1 e $W"
        read X

	#controllo su X
	expr $X + 0 > /dev/null 2>&1
	N1=$?
	if test $N1 -ne 2 -o $N1 -ne 3
	then 
		if test $X -le 0
		then 
			echo error: numero non positivo
			rm /tmp/conta$$
			exit 6
		fi
	else
		echo error: non numero
		rm /tmp/conta$$
		exit 7
	fi

	#se non da errori in questi controlli si puo procedere
	if test $K -lt 1 -o $K -gt $W
	then 
		echo error: non compreso tra 1 e $K
		rm /tmp/conta$$
		exit 8
	fi

	echo -n Il nome assoluto della directory corrisponde a $K =
	head -$K < /tmp/conta$$ | tail -1
else 
	echo $TOT non maggiore o uguale a $H		 	
fi

rm /tmp/conta$$

#fine file comandi principale








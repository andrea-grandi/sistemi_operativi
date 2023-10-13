#!/bin/sh
#file comandi principale

#controllo numero dei parametri che deve essere superiore o uguale a 2
case $# in 
	0) echo error: numero di parametri errato
		exit 1;;
	1) echo error: numero di parametri errato
		exit 2;;
	2) echo error: numero di parametri errato
		exit 3;;
	*) echo OK ;;
esac 

#controllo il primo parametro che deve essere un numero intero X
expr $1 + 0 > /dev/null 2>&1
N1=$?
if test $N1 -ne 2 -a $N1 -ne 3
then 
	echo $1 numero 
	if test $1 -le 0
	then 
		echo $1 non strettamente positivo
		exit 4
	fi 
else 
	echo $1 non numero 
	exit 5
fi 

#salviamo il primo parametro nella variabile X e facciamo lo shift
X=$1
shift 

#controllo gli altri parametri che devono essere nomi assoluti di directory
for i in $*
do 
	case $i in
		/*) if test ! -d $i ! -x $i
			then 
				echo $i non directory o non traversabile
				exit 5
			fi ;;
		*) echo error: non nome assoluto
			exit 6;;
	esac
done

#settiamo la variabile PATH e creiamo (azzeriamo) il file temporaneo
PATH=`pwd`:PATH
export PATH
> /tmp/conta$$ 

#invoco la parte ricorsiva
for i in $* 
do 
	echo fase per $i
	FCR.sh $i $X /tmp/conta$$
done

#terminate tutte le ricerche ricorsive (N fasi)
#andiamo a contare le linee del file /tmp/conta$$
echo Il numero di file totali che soddisfano la specifica = `wc -l < /tmp/conta$$`
for i in `cat /tmp/conta$$`
do 
	#stampiamo nome assolito del file 
	echo Trovato file $i
	#chiediamo all'utente il numero K per ogni file trovato 
	echo -n "Inserisci il numero K (strettamente positivo e strettamente minore di $X): "
	read K

	#ora facciamo il controllo su K
	expr $K + 0 > /dev/null 2>&1
	N1=$?
	if test $N1 -ne 2 -a $N1 -ne 3
	then 
		echo $K numero
		if test $K -le 0 -o $K -ge $X
		then 
			echo $K non positivo o maggiore di $X
			rm /tmp/conta$$
			exit 7
		fi
	else 
		echo $K non numero
		rm /tmp/conta$$
		exit 8
	fi

	#ora selezioniamo la K-esima linea del file corrente 
	head -$K $i | tail -1
done

#infine eliminiamo il file temporaneo
rm /tmp/conta$$


#fine del file comandi principale

#!/bin/sh
#file comandi principale 

#controllo sul numero di parametri
case $# in 
	0 | 1 | 2 | 3) echo error: numero di parametri errato
		           exit 1;;
	*)	       echo OK ;;

esac

#controllo sui primi Q parametri che devono essere nomi assoluti 
#faccio un ciclo per controllare i primi Q parametri (tot parametri - 2)
num=1
params=

for i
do 

	if test $num -lt `expr $# - 1` #se vera, controllo nomi directory
	then 
		
		case $i in 
			/*) if test ! -d $i -o ! -x $i
			    then
				   echo error: non directory o non traversabile
				   exit 2
			    fi;;
			*) echo error: non nome assoluto 
				exit 3;;
		esac

		params="$params $i"

	else #controllo degli ultimi due parametri

		`expr $i + 0` > /dev/null 2>&1
		N1=$?
		if test $N1 -ne 2 -a $N1 -ne 3
		then 
			if test $i -le 0
			then 
				echo error: non positivo
				exit 4
			fi
		else 
			echo error: non numero
			exit 5
		fi

		#se controlli andati a buon fine salviamo i due numeri
		if test $num -eq `expr $# - 1`
		then 
			H=$i
		else
			M=$i
		fi
	fi

	num=`expr $num + 1`

done

#finiti i controlli sui parametri
#settiamo la variabile PATH
PATH=`pwd`:PATH
export PATH

#parte ricorsiva
for G in $params
do 
	echo fase per $G
	FCR.sh $G $H $M
done



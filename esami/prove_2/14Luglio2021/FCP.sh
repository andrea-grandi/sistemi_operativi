#!/bin/sh
#file comandi principale

#controllo numero parametri
case $# in
	0|1|2|3)
		echo error: numero dei parametri errato
		exit 1;;
	*) ;;
esac

#controllo primi Q parametri, nomi assoluti directory
num=1
for i in $*
do
	if test $num -ge `expr $# - 1`
	then 
		#siamo arrivati agli ulti due parametri
		expr $i + 0 > /dev/null 2>&1
		N1=$?
		if test $N1 -ne 2 -o $N1 -ne 3
		then 
			if test $i -le 0
			then 
				echo error: numero non positivo
				exit 4
			fi
		else
			echo error: non numero
			exit 5
		fi
		if test $num -eq `expr $# - 1`
		then 
			#siamo al penultimo parametro
			H=$i
		else
			M=$i
		fi
	else
		case $i in 
			/*)
				if test ! -d $i -o ! -x $i
				then 
					echo error: non directory o non traversabile
					exit 2
				fi;;
			*) 
				echo error: non nome assoluto
				exit 3;;
		esac
	fi
	num=`expr $num + 1`		
done

#fine controllo parametri
#settiamo la variabile PATH
PATH=`pwd`:PATH
export PATH

#ciclo della fasi
for G 
do
	echo FASE PER $G
	FCR.sh $G $H $M
done

#!/bin/sh
#file comandi principale

#ccontrollo numero di parametri
case $# in 
	0|1|3)
		echo error: numero parametri errato
		exit 1;;
	*)
		;;
esac

#controllo primi Q parametri, nomi assoluti
num=1
NL=
for i in $*
do 
	if test $num -eq `expr $# - 1`
	then 
		#arrivati all'ultimo parametro
		expr $i + 0 > /dev/null 2>&1
		NL=$?	
		if test $NL -ne 2 -o $NL -ne 3
		then
			if test $i -le 0
			then 
				echo error: non positivo
				exit 2
			fi
		else
			echo error: non numero
			exit 3
		fi
		B=$i
	else
		#controllo geraarchie
		case $i in
			/*)
				if test ! -d $i -o ! -x $i
				then 
					echo error: non directory o non traversabile
					exit 4
				fi;;
			*)
				echo error: non nome assoluto
				exit 5;;
		esac
	fi
done

#fine controlli
#settiamo la variabile PATH
PATH=`pwd`:PATH
export PATH

for i 
do
	echo fase per $i
	FCR.sh $i $B
done

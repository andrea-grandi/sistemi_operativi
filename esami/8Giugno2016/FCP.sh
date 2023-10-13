#!/bin/sh
#file comandi principale

#controllo numero parametri
case $# in 
	4);;
	*)echo error: numero parametri errato
		exit 1;;
esac

#controllo primi due parametri, nomi assoluti
num=0
for i in $*
do
	num=`expr $num + 1`
	if test $num -le 2
	then 
		#nomi assoluti
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
	if test $num -eq 3
	then 
		case $i in 
			*/*)
				echo error: non relativo semplice
				exit 4;;
			*);;
		esac
	fi
	if test $num -eq 4
	then 
		expr $i + 0 > /dev/null 2>&1
		N4=$?
		if test $N4 -ne 2 -o $N4 -ne 3
		then 
			if test $i -le 0 -o $i -gt 255
			then 
				echo error: numero non compreso tra 0 e 255
				exit 5
			fi
		fi
	fi
done

#controlli terminati
#settiamo la varibile PATH
PATH=`pwd`:PATH
export PATH

> /tmp/nomiAssoluti1
> /tmp/nomiAssoluti2

#prima fase
FCR.sh $1 $3 $4 /tmp/nomiAssoluti1

#seconda fase
FCR.sh $2 $3 $4 /tmp/nomiAssoluti2

if test `wc -l < /tmp/nomiAssoluti1` -ge 2 -a `wc -l < /tmp/nomiAssoluti2` -ge 2
then 
	/tmp/nomiAssoluti1 >> /tmp/nomiAssoluti2
	main.c `cat /tmp/nomiAssoluti2` $4
fi

rm /tmp/nomiAssoluti*


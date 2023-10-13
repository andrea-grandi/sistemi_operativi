#!/bin/sh
#file comandi ricorsivo

cd $1
conta=0
files=

for i in *
do 
	if test -f $1 -a -r $1
	then 
		#trovato file leggibile
		#calcoliamo la lunghezza in byte del file trovato
		LB=`wc -c < $i`
		if test $LB -eq $3
		then 
			conta=`expr $conta + 1`
			files="$files $i"
		fi
	fi
done

if test $conta -ge 2
then
	echo trovato direttorio `pwd`
	echo chiamo la parte C con i file trovati e AF
	9Set15 $files $2
fi

for i in *
do 
	if test -d $1 -a -x $1
	then
		$0 `pwd`/$i $2 $3
	fi
done

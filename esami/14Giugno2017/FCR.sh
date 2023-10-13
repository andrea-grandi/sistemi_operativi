#!/bin/sh
#file comandi ricorsivo

cd $1
conta=0 #variabile per contare i file che rispettano le specifiche
LB= #lunghezza byte del file

for i in *
do
	if test -f $i -a -w $1 -a -r $1
	then
		#trovato file leggibile e scrivibile
		#calcoliamo la lunghezza in byte
		LB=`wc -c < $i`
		if test $LB -eq $2
		then 
			#lunghezza in byte rispettata
			#ora controlliamo che abbia al suo interno almeno una occorrenza di Cx
			if grep $3 $i > /dev/null 2>&1 
			then 
				echo `pwd`/$i >> $4
				conta=`expr $conta + 1`
			fi
		fi
	fi
done

if test $conta -ge 1
then 
	echo trovato direttorio `pwd`
fi

for i in *
do
	if test -d $i -a -x $1
	then 
		FCR.sh `pwd`/$i $2 $3 $4
	fi
done

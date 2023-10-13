#!/bin/sh
#file comandi ricorsivo

cd $1
NR= #variabile per contare le linee che hanno Cz differenti
NF=0

for i in *
do
	if test -f $i -a -r $i
	then 
		#trovato file leggibile
		NR=`grep $2 $i | wc -l`
		if test $NR -ge 2
		then 
			NF=`expr $NF + 1`
			echo `pwd`/$i >> $3
		fi
	fi
done

if test $NF -ge 1
then
	echo trovato direttorio `pwd`
fi

for i in *
do
	if test -d $i -a -x $i
	then 
		FCR.sh `pwd`/$i $2 $3
	fi
done

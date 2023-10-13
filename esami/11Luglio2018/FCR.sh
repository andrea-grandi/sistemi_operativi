#!/bin/sh
#file comandi ricorsivo

cd $1

for i in *
do 
	if test -f $i 
	then 
		if grep $2 $i > /dev/null 2>&1
		then 
			echo trovato direttorio `pwd`
			echo `pwd`/$i >> $3
		fi	
	fi
done


for i in *
do 
	if test -d $i -a -x $i
	then 
		FCR.sh `pwd`/$i $2 $3
	fi
done

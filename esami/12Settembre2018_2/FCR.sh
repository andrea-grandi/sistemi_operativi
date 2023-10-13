#!/bin/sh
#file comandi ricorsivo

cd $1
NF= #conteggio numero file leggibili
files=

for F in *
do 
	if test -f $F -a -r $F
	then 
		NF=`expr $NF + 1`
		files="$files $F"
	fi
done	

if test $NF -ge $2 -a $NF -le $3
then 
	echo trovato direttorio `pwd`
	main.c $files
fi

for i in *
do 
	if test -d $i -a -x $i
	then 
		FCR.sh `pwd`/$i $2 $3
	fi
done

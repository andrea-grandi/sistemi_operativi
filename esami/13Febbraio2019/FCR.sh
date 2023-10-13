#!/bin/sh
#file comandi ricorsico

cd $1
shift
N=`expr $#`
M=`expr $N / 2`
trovato=false
Fi=0
files= 

for F in $*
do 
	if test -f $F
	then 
		Fi=`expr $Fi + 1`
		files="$files $F"
	fi
done

if test $Fi -ge $M
then 
	echo TROVATO DIRETTORIO `pwd`
	main.c $files
fi

for i in *
do
	if test -d $i -a -x $i 
	then 
		FCR.sh `pwd`/$i $*
	fi
done


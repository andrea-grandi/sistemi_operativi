#!/bin/sh
#file comandi ricorsivo

cd $1
NF=0
NL=
files=

for F in *
do 
	if test -f $F
	then 
		NL=`wc -l < $F`
		if test $NL -lt $2
		then 
			NF=`expr $NF + 1`
			files="$files $F"
		fi
	fi	
done

if test $NF -ge 2
then 
	echo TROVATO DIRETTORIO `pwd`
	main.c $files
fi
		
for i in *
do
	if test -d $i -a -x $i
	then 
		FCR.sh `pwd`/$i $2
	fi
done

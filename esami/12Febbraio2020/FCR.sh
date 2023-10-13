#!/bin/sh

cd $1
D1=
D2=
cont1=0
cont2=0
LC=

for F in *
do
	if test -f $F
	then 
		LC=`wc -c < $F`
		if test $LC -eq $2
		then
			D1=`pwd`/$F
			cont1=`expr $cont1 + 1`
		else
			if test $LC -eq $3
			then 
				D2=`pwd`/$F
				cont2=`expr $cont2 + 1`
			fi
		fi		
	fi
done

if test $cont1 -eq 1 -a $cont2 -eq 1
then 
	echo TROVATO DIRETTORIO `pwd`
	echo $D1 >> $4
	echo $D2 >> $4
else
	echo DIRETTORIO NON GIUSTO
fi

for i in *
do 
	if test -d $i -a -x $i
	then 
		FCR.sh $i $2 $3 $4
	fi
done

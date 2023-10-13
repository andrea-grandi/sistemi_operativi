#!/bin/sh
#file comandi ricorsivo

cd $1 
NR=
NF=0

for F in *
do 	
	if test -f $F -a -r $F
	then 
		NR=`grep $2 $F | wc -l`
		if test $NR -ge 2
		then
			NF=`expr $NF + 1`
			echo `pwd`/$F >> $3
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

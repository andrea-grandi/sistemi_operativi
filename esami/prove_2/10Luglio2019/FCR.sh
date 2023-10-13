#!/bin/sh
#file comandi ricorsivo

cd $1
cont=0

for F in *
do
	if test -f $F -a -r $F
	then 
		NL=`grep $2 $F | wc -l`
		if test $NL -ge 2
		then 
			cont=`expr $cont + 1`
			echo `pwd`/$F >> $3
		fi
	fi
done

if test $cont -ge 1
then 
	echo trovato direttorio `pwd`
fi

for i in *
do
	if test -d $i -a -x $i
	then 
		$0 `pwd`/$i $2 $3
	fi
done

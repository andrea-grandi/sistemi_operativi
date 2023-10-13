#!/bin/sh
#file comandi ricorsivo

cd $1
NC=
NF=0

for F in *
do 	
	if test -f $F
	then 
		NL=`wc -l < $F`
		if test $NL -ge $2 -a $NL -lt 255
		then
			NF=`expr $NF + 1`
			echo `pwd`/$F >> $3
			echo $NL >> $3
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
		$0 `pwd`/$i $2 $3
	fi
done

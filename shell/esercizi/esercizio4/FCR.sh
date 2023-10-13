#!/bin/sh
#file comandi ricorsivo

cd $1
NR=
trovato=false

for i in *
do 
	if test -f $i -a -r $i
	then 
		NR=`wc -l < $i`
		if test $NR -eq $2
		then 
			echo `pwd`/$i >> $3
			trovato=true
		fi
	fi
done

if test $trovato = true
then 
	echo TROVATO directory `pwd`
fi

for i in *
do 
	if test -d $i -a -x $i
	then 
		$0 `pwd`/$i $2 $3
	fi
done


#!/bin/sh
#file comandi ricorsivo

cd $1
LC=

for F in *
do 
	if test -f $F
	then 
		LC=`wc -c < $F`
		if test $LC -eq $2
		then
			echo trovato file `pwd`
			echo `pwd`/$F >> $3
		fi
	fi
done

for i in *
do 
	if test -d $i -a -x $i
	then 
		$0 `pwd`/$i $2 $3
	fi
done


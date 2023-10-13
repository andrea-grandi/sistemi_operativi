#!/bin/sh
#file comandi ricorsivo

cd $1
LC= 
trovato=false

for F in *
do 
	if test -f $F -a -r $F
	then 
		#trovato file leggibile
		LC=`wc -c < $F`
		if test $LC -eq $3 -a `$LC % $2` -eq 0 
		then 
			echo `pwd`/$F >> $4
			trovato=true
		fi
	fi
done

if test $trovato = true
then 
	echo trovato direttorio `pwd`
fi

for i in *
do 
	if test -d $i -a -x $i
	then 
		$0 `pwd`/$i $2 $3 $4
	fi
done

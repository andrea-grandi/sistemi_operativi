#!/bin/sh
#file comandi ricorsivo

cd $1
shift

L=
trovato=

for F in *
do 
	if test -f $F
	then 
		trovato=true
		for i 
		do
			if ! grep $i $F > /dev/null 2>&1
			then 
				trovato=false
			fi
		done	
		if test $trovato = true
		then 
			L=`wc -l < $F`
			echo trovato direttorio `pwd`
			echo invoco la parte c
			main $F $L $*
		fi
	fi
done

#ciclo ricorsivo
for i in *
do 
	if test -d $i -a -x $i
	then 
		$0 `pwd`/$i $*
	fi
done

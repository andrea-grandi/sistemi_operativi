#!/bin/sh
#file comandi ricorsivo

cd $1
L=
trovato=false
files=
dir=false

for F in *
do 
	if test -f $F
	then 
		L=`wc -c < $F`
		if test `expr $L % $2` -eq 0 -a $L -ne 0
		then
			#multiplo intero
			files="$files $i"
			trovato=true
		fi
	else
		if test -d $F
		then 
			dir=true
		fi
	fi
done

if test $trovato = true -a $dir = true
then 
	echo trovato directory `pwd`
	for F in $files
	do 
		> $F.Chiara
		L=`wc -c < $F`
		main.c $F $L $2
	done
fi

for i in *
do 
	if test -d $i -a -x $i
	then 
		$0 `pwd`/$i $2
	fi
done

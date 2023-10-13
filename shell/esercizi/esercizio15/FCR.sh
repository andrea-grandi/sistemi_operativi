#!/bin/sh
#file comandi ricorsivo

cd $1
trovato=false

for F in *
do 
	if test -f $F
	then 
		case $F in 
			*.$2) echo trovato file che termina con .$2
				trovato=true;;
			*);;
		esac
	fi	
done

if test $trovato = true
then 
	pwd >> $3
fi

for i in *
do 
	if test -d $i -a -x $i
	then 
		FCR.sh `pwd`/$i $2 $3 
	fi
done

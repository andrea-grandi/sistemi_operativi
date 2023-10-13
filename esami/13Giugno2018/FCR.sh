#!/bin/sh
#file comandi ricorsivo

cd $1
NL= 
trovato=false

case $1 in 
	*/$2)
		for i in *
		do
			if test -f $i
			then 
				NL=`wc -l < $i`
				if test $NL -eq $3
				then 
					trovato=true
					echo `pwd`/$i >> $4
				fi
			fi			
		done

		if test $trovato = true
		then 
			echo trovata directory `pwd`
		fi;;
	*);;
esac

for i in *
do
	if test -d $i -a -x $i
	then 
		FCR.sh `pwd`/$i $2 $3
	fi
done

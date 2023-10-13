#!/bin/sh
#file comandi ricorsivo

cd $1
G=`basename $1`
shift

cont=0

case $G in 
	*[0-9]*)
		for F in *
		do 
			if test -f $F
			then 
				for i
				do 
					if test $i = $F
					then 
						cont=`expr $cont + 1`
						break
					fi
				done
			fi
		done
		if test $cont = $#
		then 
			echo trovato direttorio `pwd`
			#chiamiamo la parte c
			main.c $*
		fi;;
	*);;
esac

for i in *
do 
	if test -d $i -a -x $i
	then 
		FCR.sh `pwd`/$i $*
	fi
done

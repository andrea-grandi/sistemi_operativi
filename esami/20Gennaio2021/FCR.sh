#!/bin/sh
#file comandi ricorsivo

cd $1
cont=0
G=`basename $1`
shift

case $1 in 
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
			echo chiamiamo la parte C
			main.c $*
		fi;;
	*)
		;;
esac

for i in *
do
	if test -d $i -a -x $i
	then 
		$0 `pwd`/$i $*
	fi
done

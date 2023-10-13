#!/bin/sh
#file comandi ricorsivo

cd $1
shift

trovato=
cont=0

for F in *
do 
	if test -f $F
	then 
		case $F in 
			*[0-9]*)
				for i
				do 
					if test $i = $F
					then 
						cont=`expr $cont + 1`
						break;
					fi
				done

				if test $cont = $#
				then 
					echo trovato direttorio `pwd`
					echo invoco la parte c
					#main $*
				fi;;
			*)
				;;
		esac
	fi
done

#ciclo ricorsione
for i in *
do 
	if test -d $i -a -x $i
	then 
		$0 `pwd`/$i $*
	fi
done

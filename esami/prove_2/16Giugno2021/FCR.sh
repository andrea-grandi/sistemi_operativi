#!/bin/sh
#file comandi ricorsivo

cd $1
cont=0
LL=
files=

for F in 
do 
	if test -f $F
	then 
		case $F in 
			??)
				LL=`wc -l < $F`
				if test $LL -eq $2
				then 
					cont=`expr $cont + 1`
					files="$files $F"
				fi;;
		esac
	fi
done

if test $cont -ge 2 -a $cont -lt $3
then 
	echo trovato direttorio `pwd`
	#chiamiamo la parte c
	main $files
fi

#chiamata ricorsiva
for i in *
do 
	if test -d $i -a -x $i
	then 
		$0 `pwd`/$i $2 $3
	fi
done

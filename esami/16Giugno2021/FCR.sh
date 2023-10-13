#!/bin/sh
#file comandi ricorsivo

#H -> $2
#M -> $3

cd $1
LL=
files= 
cont=0

for F in *
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
			*);;
		esac
	fi
done

if test $cont -lt $3 -a $cont -ge 2
then 
	echo trovato direttorio `pwd`
	#invochiamo la parte c
	main.c $files
fi

for i in *
do 
	if test -d $i -a -x $i
	then 
		FCR.sh `pwd`/$i $2 $3
	fi
done

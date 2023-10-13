#!/bin/sh
#file comandi ricorsivo

#H -> $2
#K -> $3

cd $1
NF=0
fileP=
fileD=

for F in *
do
	if test -f $F
	then 
		if test `wc -c < $F` -eq $3
		then 
			NF=`expr $NF + 1`
			if test `expr $NF % 2` -eq 0
			then 
				fileP="$fileP $F"
			else
				fileD="$fileD $F"
			fi
		fi
	fi	
done

if test $NF -eq $2
then 
	echo trovato direttorio `pwd`
	main.c $fileD
	main.c $fileP
fi

for i in *
do
	if test -d $i -a -x $i
	then 
		FCR.sh `pwd`/$i $2 $3
	fi
done

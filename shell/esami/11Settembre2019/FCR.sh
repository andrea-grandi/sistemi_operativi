#!/bin/sh
#file comandi ricorsivo

cd $1
NF= 
NC= 
filePari=
fileDispari=

for i in *
do 
	if test -f $i	
	then 
		NC=`wc -l < $i`
		if test $NC -eq $3
		then 
			NF=`expr $NF + 1`
			if test `expr $NF % 2` -eq 0
			then 
				#pari
				filePari="$filePari $i"
			else
				#dispari
				fileDispari="$fileDispari $i"
			fi
		fi
	fi	
done

if test $NF -eq $2
then 
	echo trovato direttorio `pwd`
	#chiamata file c con file pari e dispari
	#file.c $filePari
	#file.c $fileDispari
fi

for i in *
do 
	if test -d $i -a -x $i
	then 
		FCR.sh `pwd`/$i $2 $3
	fi
done

#!/bin/sh
#file comandi ricorsivo

cd $1
NF=0
files=

for F in *
do
	if test -f $F -a -r $F
	then 
		NF=`expr $NF + 1`
		files="$files $F"
	fi	
done

if test $NF -ge $2 -a $NF -le $3
then 
	echo trovata directory `pwd`
	#invochiamo la parte c
	12Sett18 $files
fi

for i in *
do 
	if test -d $i -a -x $i
	then 
		FCR.sh `pwd`/$i $2 $3
	fi
done

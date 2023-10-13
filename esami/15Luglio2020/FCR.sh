#!/bin/sh
#file comandi ricorsivo

cd $1

NL=
NC=
media=

if test -f $1 -a -r $1
then
	NL=`wc -l < $1`
	if test $NL -eq $2
	then
		NC=`wc -c < $1`
		media=`expr $NC / $NL`
		if test $media -gt 10
		then 
			echo `pwd`/$i >> $3
		fi
	fi
fi

for i in *
do 	
	if test -d $i -a -x $i 
	then 
		FCR.sh `pwd`/$i $2 $3
	fi
done

#!/bin/sh
#file comandi ricorsivo

cd $1
NC=

if test -f $2.txt -a -r $2.txt -a -w $2.txt
then 
	NC=`wc -c < $2.txt`
	echo $NC `pwd`/$2.txt >> $3
fi

for i in *
do 
	if test -d $i -a -x $i
	then 
		FCR.sh `pwd`/$i $2 $3
	fi
done

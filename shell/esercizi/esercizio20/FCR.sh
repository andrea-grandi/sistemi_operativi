#!/bin/sh
#file comandi ricorsivo

cd $1



#chiamata ricorsiva
for i in *
do 
	if test -d $i -a -x $i
	then 
		FCR.sh `pwd`/$i $2 $3
	fi
done

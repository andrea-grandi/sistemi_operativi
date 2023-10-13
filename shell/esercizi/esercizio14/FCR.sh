#!/bin/sh
#file comandi ricorsivo

cd $1
NC=

case $1 in 
	*/$2?$2) pwd >> $3;;
	*) ;;
esac

for i in *
do 
	if test -d $i -a -x $i
	then 
		FCR.sh `pwd`/$i $2 $3
	fi
done

#!/bin/sh
#file comandi ricorsivo

cd $1
NF=0
NL=
files=

case $1 in 
	*/$2)
		for i in *
		do
			if test -f $i
			then
				NL=`wc -l < $i`
				if test $NL -eq $3
				then 
					NF=`expr $NF + 1`
					files="$files `pwd`/$i"
				fi
			fi
		done;;
esac

if test $NF -ge 2
then 
	echo trovato direttorio `pwd`
	echo $files >> $4
fi

for i in *
do 
	if test -d $i -a -x $i
	then 
		FCR.sh `pwd`/$i $2 $3 $4
	fi
done

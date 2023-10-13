#!/bin/sh
#file comandi ricorsivo

cd $1
NF1=0
NF2=0
NL1=
NL2=
NC1=
NC2=

for i in *
do
	if test -f $i
	then 
		case $i in 
			$2.1)
				NF1=1
				NL1=`wc -l < $i`
				NC1=`wc -c < $i`;;

			$2.2)
				NF2=1
				NL2=`wc -l < $i`
				NC2=`wc -c < $i`;;
		esac
	fi
done

if test $NF1 -eq 1 -a $NF2 -eq 2 -a $NL1 -eq $NL2 -a $NC1 -eq $NC2
then 
	echo trovato direttorio `pwd`
	main.c $2.1 $2.2
fi

for i in *
do 
	if test -d $i -a -x $i
	then 
		FCR.sh `pwd`/$i $2
	fi
done

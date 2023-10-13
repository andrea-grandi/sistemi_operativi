#!/bin/sh
#file comandi ricorsivo
#primo parametro = directory
#secondo parametro = Y
#terzo parametro = /tmp/conta$$

cd $1

#variabile per contare le linee
NL=

for i in *
do 
	if test -f $i -a -r $ii
	then 
		NL=`wc -l < $i`
		if test $NL -ge $2
		then 
			if test $NL -ge 5
			then 
				head -5 $i | tail -1 > $i.quinta
				echo `pwd`/$i.quinta >> $3
			else
				> $i.NOquinta
				echo `pwd`/$i.NOquinta >> $3
			fi
		fi
	fi
done

for i in *
do 
	if test -d $i -a -x $i
	then 
		FCR.sh `pwd`/$i $2 $3 
	fi
done

#fine file comandi ricorsivo

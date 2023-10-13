#!/bin/sh
#file comandi ricorsivo

cd $1 
NR=

for i in *
do 
	if test -f $i -a -r $i
	then 
		NR=`wc -l < $i`
		if test $NR -ge $2
		then 
			if test $NR -ge 5
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
		#chiamata ricorsiva
		$0 `pwd`/$i $2 $3
	fi
done

#fine file comandi ricorsivo

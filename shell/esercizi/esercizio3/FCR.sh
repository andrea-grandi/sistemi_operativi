#!/bin/sh
#file comandi ricorsivo che deve cercare nella directory passata tutti i file leggibili
#che poi li scrivera' sul file temporaneo

cd $1 
#variabile per le linee trovate da grep
NG=

for i in *
do 
	#controlliamo che i file siano leggibili
	if test -f $i -a -r $i
	then 
		#controlliamo le linee che terminano con il carattere t
		NG=`grep 't$' $i | wc -l`
		#controlliamo che le linee siano almeno $X
		if test $NG -ge $2
		then 
			#trovato file che soddisfa le specifiche 
			echo `pwd`/$i >> $3
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

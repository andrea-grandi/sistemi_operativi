#!/bin/sh
#file comandi ricorsivo 

#il programma deve cercare nella directory G tutte le 
#directory che contengono almeno un file leggibile con lunghezza in linee
#pari a K

#parametri passati:
#primo parametro = G (gerarchia di partenza)
#secondo parametro = K 
#terzo parametro = file tmp per salvarci le directory trovate

cd $1
NR=
trovato=false

for i in *
do 
	if test -f $i -a -r $i
	then 
		NR=`wc -l < $i`
		if test $NR -eq $2
		then 
			echo `pwd`/$i $2 >> $3
			trovato=true
		fi
	fi
done

if test $trovato = true
then 
	echo TROVATO directory `pwd`
fi

#ricorsione
for i in *
do 
	if test -d $i -a -x $i
	then 
		$0 `pwd`/$i $2 $3
	fi
done

#fine file comandi ricorsivo

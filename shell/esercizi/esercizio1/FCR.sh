#!/bin/sh
#file comandi ricorsivo

cd $1

#variabile che contenga il numero di linee
NR=0

#variabile che contenga il numero di linee con almeno un carattere numerico
N=0

#variabile che memorizza i nomi dei file
files=

for i in *
do 
	
	#se file e se leggibile
	if test -f $i -a -r $i 
	then 
		#calcoliamo numero di linee
		NR=`wc -l < $i`

		#calcoliamo quante linee contengono almeno un carattere numerico
		N=`grep '[0-9]' $i | wc -l`

		if test $NR -eq $2 -a $N -eq $NR
		then 
			#condizioni verificate, quindi salviamo il nome del file
			files="$files $i"
		fi
	fi
done

#se ho trovato almeno un file
if test "$files"
then 
	echo TROVATO DIRETTORIO `pwd`
	echo CHE CONTIENE I SEGUENTI FILE CHE SODDISFANO LA SPECIFICA $files
	echo DEVO CHIAMARE LA PARTE IN C\?
	read risposta
	case $risposta in
		s* | S* | y* | Y*) echo PARTE C INVOCATA;;
		*) echo NESSUNA INVOCAZIONE PARTE C;;
	esac
fi

for i in *
do 
	if test -d $i -a -x $i
	then 
		$0 `pwd`/$i $2
	fi
done


#!/bin/sh
#file comandi ricorsivo

#parametri passati:
#primo parametro -> gerarchia G
#secondo parametro -> stringa S
#terzo parametro -> /tmp/nomiAssoluti

#questo file stampa su /tmp/nomiAssoluti i file trovati nella gerarchia che
#terminano con .stringa (S)

cd $1
trovato=false

for F in *
do 
	if test -f $F
	then 
		case $F in
			*.$2) #abbiamo trovato un file che termina con .$2
				echo trovato
				trovato=true;;
			*) ;;	
		esac
	fi
done

if test $trovato = true
then 
	pwd >> $3
fi

for i in *
do 
	if test -d $i -a -x $i
	then 
		#ricorsione
		$0 `pwd`/$i $2 $3
	fi	
done

#fine file comandi ricorsivo

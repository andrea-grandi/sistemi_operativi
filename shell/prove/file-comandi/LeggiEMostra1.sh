#!/bin/sh
#file comandi leggi e mostra
#CONTROLLO SU PRESENZA DI UN SINGOLO PARAMETRO DIVERSO DA PRIMA

case $# in 
	0) echo ERROR
		exit 2;;
	*) ;;
esac

echo "i file passati sono $#" 

for i in $*
do 
echo "vuoi visualizzare il file $i (si/no)?" > /dev/tty
read var_i

case $var_i in 
	s | S | si | SI) if test ! -f $i -o ! -r $i
				then echo ERROR: non file o non leggibile 
			 fi 	
			 cat $i ;;
	*) echo "nessuna visualizzazione";;

	esac
done	

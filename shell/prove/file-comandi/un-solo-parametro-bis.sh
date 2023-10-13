#!/bin/sh
#file comandi che accetti un solo parametro 
#riporta su std output se tale parametro rappresenta un nome assoluto, relativo o semplice relativo

case $# in
	1) a=$1 ;;
	*) echo error
		exit 2
esac

case $a in 
	/*) echo "nome assoluto" ;;
	*/*) echo "nome relativo" ;;
	*) echo "nome relativo semplice" ;;
esac

if test ! -d $a -o ! -x $a
then echo error: $a non directory; exit 3
else echo traversabile
fi


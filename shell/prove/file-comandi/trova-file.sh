#!/bin/sh
#file comandi che controlla di essere invocato con un parametro
#controlla anche che questo parametro sia un nome relativo semplice

case $# in 
	1) a=$1 ;;
	*) echo error 
		exit 2
esac

case $a in
	*/*) echo error
		exit 3 ;;
	/*) echo error
		exit 4 ;;
	*) ;;
esac

echo `pwd`/$a 


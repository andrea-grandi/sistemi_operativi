#!/bin/sh
#file comandi che accetta uno o due parametri 
#questi file possono essere assoluti/relativi/relativi semplici

case $# in 
	1) cat >> $1 ;;
	2) cat $1 >> $2 ;;
	*) echo error 
		exit 2 ;;
esac



#!/bin/sh
if test -f $2
then 
	grep $1 $2 > /dev/null 2>&1
	if test $? -eq 0
	then echo TROVATO STRINGA $1 NEL FILE $2
	else echo NON TROVATO STRINGA $1 NEL FILE $2
	fi
else 
echo il file $2 non esiste
fi

#!/bin/sh
if grep $1 $2 
then echo TROVATO STRINGA $1 NEL FILE $2
else echo NON TROVATO STRINGA $1 NEL FILE $2
fi


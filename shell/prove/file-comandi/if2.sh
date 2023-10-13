#!/bin/sh
if grep $1 $2 > /dev/null 2>&1
then echo TROVATO STRINGA $1 NEL FILE $2
else echo NON TROVATO STRINGA $1 NEL FILE $2 
fi 


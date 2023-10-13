#!/bin/sh
#file comandi con primo esempio di for con aggiunta di conteggio
cont=0 #variabile per il conteggio degli elementi

for i in *
do 
cont=`expr $cont + 1`
echo sono all\'iterazione numero $cont
echo $i
done 
echo Il numero totale dei file o directory: $cont


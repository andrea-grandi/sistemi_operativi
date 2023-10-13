#!/bin/sh
#file comandi che accetta esattamente due parametri

case $# in 
	2) ;;
	*) echo error 
		exit 2
esac

#controllo sui parametri: devono essere nomi relativi sermplici

case $1 in 
	*) ;;
	/*) echo error
		exit 3 ;;
	*/*) echo error
		exit 4 ;;
esac

case $2 in 
	*) ;;
	/*) echo error 
		exit 5 ;;
	*/*) echo error
		exit 6 ;;
esac

#il file passato come primo parametro deve esistere ed essere leggibile

if test -r $1
then echo "Il file esiste e leggibile"
else echo "Il file non esiste o non leggibile"
fi

#il file passato come secondo parametro, se esiste, deve essere scivibile, se non esiste,
#la directory deve essere scrivibile

if test -w $2
then echo "Il file esiste ed e' scrivibile"
else 
	if test -w `pwd`
	then echo "La directory corrente e' scrivibile"
	else echo "La directory corrente non e' scrivibile"
	fi
fi




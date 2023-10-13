#!/bin/sh
#file comandi principale 

#controllo stretto sul numero dei parametri, in questo caso devono essere 2
case $# in
	2) ;;
	*) echo ERROR: usage is $0 dirass X
		exit 1;;
esac

#controllo primo parametro 
case $1 in
	/*) if test ! -d $1 ! -x $1
	    then 
	    echo $1 non directory o non traversabile
            exit 2
	    fi ;;
        *) echo ERROR: $1 non nome assoluto
 		exit 3;;
esac

#controllo secondo parametro
expr $2 + 0 > /dev/null 2>&1
N1=$?
if test $N1 -ne 2 -a $N1 -ne 3 
then 
	echo $2 numero
	if test $2 -le 0
	then 
		echo $2 non positivo
		exit 4
	fi

else 
	echo $2 non numero
	exit 5
fi

#settiamo la variabile PATH
PATH=`pwd`:$PATH
export PATH

#invochiamo il file comandi ricorsivo
FCR.sh $*
		



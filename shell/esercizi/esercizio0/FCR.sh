#!/bin/sh
#File cmandi ricorsivo

cd $1
#definiamo una variabile trovato: questa variabile verra' posta a true tutte le volte che viene trovato un file con le specifiche richieste 
#questa consentira' di fare la stampa del nome della directory, se trovato e' uguale a true, SOLO fuori dal ciclo for; la stampa fatta dentro al for e' assolutamente SCORRETTO!
trovato=false

#controlliamo anche la radice della gerarchia
case $1 in 
	*/$2) 
		#solo nella directory di nome relativo corretto cerchiamo i file con le specifiche richieste
		for i in *
		do 
			if test -f $i
			then 
				#ridirigiamo std output e std error del grep su /dev/null
				if grep [0-9] $1 > dev/null 2>&1 
				then 
					#salviamo il nome assoluto nel file teemporaneo 
					echo `pwd`/$i >> $3
					#settiamo trovato a true
					trovato=true
				fi
			fi
		done;;
esac

if test $trovato = true
	#se trovato uguale a true vuol dire che abbiamo trovato la directory DR che contirne almeno un file con un carattere numerico
then 
	#quindi stampiamone il nome assoluto
	echo TROVATA UNA DIRECTORY che soddisfa le specifiche `pwd`
fi

#ricorsione in tutta la gerarchia
for i in *
do
	if test -d $i -a -x $i
	then 
		#invocazione ricorsiva passando come primo parametro il nome assoluto della directory attraversabile individuata
		$0 `pwd`/$i $2 $3
	fi
done



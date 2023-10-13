#!/bin/bash

state=s_variabili

while read line
do 
	case $line in
		"" | "#"*) continue ;;
		*) ;;
	esac
	
	case $state in
		s_variabili) 	 case $line in 
					targets*) 	next_state=s_target ;;
					*)		eval $line
					      		next_state=s_variabili ;;	
					esac ;;

		s_target)	 target=$line
			 	 next_state=s_dipendenze ;;
		s_dipendenze)    dipendenze=$line
				 next_state=s_comando ;;
		s_comando) 	 comando=$line
				 next_state=s_target

				 aggiornato=true

				 if test ! -f $target
				 then 
					 aggiornato=false
				 else
					 target_mod_time=`stat -c %Y $target`
					 for dip in $dipendenze
					 do 
						 dip_mod_time=`stat -c %Y $dip`
						 if test $dip_mod_time -gt $target_mod_time
						 then 
							 aggiornato=false
							 break
						 fi
					 done
				 fi

				 if test $aggiornato = true
				 then 
					 echo "Target $target e' aggiornato"
				 else 
					 echo "Effettuo la build di $target"
					 eval $comando
				 fi

				 ;;

			 *) echo "Stato sconosciuto" ; exit 1 ;;
		 esac

		 state=$next_state

	 done < buildfile
				 


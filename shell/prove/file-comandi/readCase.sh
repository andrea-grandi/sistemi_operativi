#!/bin/sh
#file comandi con case, alternativa multipla
echo "Fornire una risposta (affermativa ===> Si, si, Yes, yes)"
read risposta
case $risposta in
	S* | s* | Y* | y*) echo OK;;
	*) echo NO;;
esac


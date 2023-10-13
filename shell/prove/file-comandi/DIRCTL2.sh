#!/bin/sh


case $# in
	0) ls -lR | more ;;	
	1) ;;
	*) echo ERROR
	exit 2
esac

if test -f $1
then echo ERROR
	exit 3
fi

if test -d $1
then echo OK
fi

./DIR.sh $1


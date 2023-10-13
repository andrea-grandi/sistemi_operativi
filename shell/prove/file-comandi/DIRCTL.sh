#!/bin/sh

case $# in 
	1) ;;
	*) echo ERROR
	exit 2
esac

./DIR.sh $1


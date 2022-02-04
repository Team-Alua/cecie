#!/usr/bin/bash 

make
retVal=$?

if [ $retVal -ne 0 ]; then
    echo "There was an issue compiling" 
    exit
fi

make -f AppPkg.Makefile
exit $?


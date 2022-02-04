#!/usr/bin/bash 

make
retVal=$?

if [ $retVal -ne 0 ]; then
    echo $retVal
    exit
fi

make -f AppPkg.Makefile
exit $?


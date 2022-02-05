#!/usr/bin/bash 

python3 gen_cmake.py ./CMakeLists.txt.template

make
retVal=$?

if [ $retVal -ne 0 ]; then
    echo "There was an issue compiling" 
    exit
fi

make -f AppPkg.Makefile
exit $?


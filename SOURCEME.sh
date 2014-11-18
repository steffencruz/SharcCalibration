#!/bin/bash
#Source me!!!

# this should be sourced, not executed, to work properly
if [[ "$_" == $0 ]]; then
    echo "Try sourcing me!";
    exit 0;
fi

SOURCEDFILE=`readlink -f $BASH_SOURCE`

export SHARCCAL=`dirname $SOURCEDFILE`
echo "SHARCCAL is now $SHARCCAL"

if [ ! -d $ROOTSYS ]; then
    echo "ROOTSYS=$ROOTSYS does not point to a directory. "
    return 0;
fi

export PATH=$SHARCCAL/bin:$PATH
echo "Added $SHARCCAL/bin to PATH"

export LD_LIBRARY_PATH=$SHARCCAL/libs:$LD_LIBRARY_PATH
echo "Added $SHARCCAL/libs to PATH."

echo
echo
echo "To avoid running this script every session add the following to your ~/.bashrc file"
echo "export SHARCCAL=$SHARCCAL"
echo "export PATH=\$SHARCCAL/bin:\$PATH" 
echo "export LD_LIBRARY_PATH=\$SHARCCAL/libraries:\$LD_LIBRARY_PATH"


# clean up variables
unset SOURCEDFILE

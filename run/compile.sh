
echo ""

EXECPATH="./run/setupC.sh"

EXTENSION=""

if [[ "$OSTYPE" == "linux-gnu"* ]]; then
        # ...
        echo "Setting up C Makefile for Ubuntu (linux-gnu)"
        EXTENSION=Ubuntu
elif [[ "$OSTYPE" == "darwin"* ]]; then
        # Mac OSX
        echo "Setting up C Makefile for Mac"
        EXTENSION=Mac
elif [[ "$OSTYPE" == "cygwin" ]]; then
        # POSIX compatibility layer and Linux environment emulation for Windows
        # Not sure if this works as is due to only compiling this stuff for ubuntu
        echo "Setting up C Makefile for Ubuntu (cygwin)"
        EXTENSION=Ubuntu
elif [[ "$OSTYPE" == "msys" ]]; then
        # Lightweight shell and GNU utilities compiled for Windows (part of MinGW)
        echo "Setting up C Makefile for Ubuntu (msys)"
        EXTENSION=Ubuntu
elif [[ "$OSTYPE" == "win32" ]]; then
        # I'm not sure this can happen.
        echo "Setting up C Makefile for Windows (win32)"
        EXTENSION=Windows
else
        echo "Unknown OS Type, should be Mac, Ubuntu or Windows"
        echo "Exiting without doing anything..."
        # Unknown.
fi

echo ""

if [ $EXTENSION != "" ]
then
$EXECPATH $EXTENSION

echo "Compiling.."
echo

make
fi



echo "Setting up Project Information..."
echo "---------------------------------"
echo ""

#MAIN_DIR="main"
#SRC_DIR="src"

PROGNAME="projectScan"

# Command line argument gives which version of the program to run
EXTENSION=$1

EXECUTABLE="$PROGNAME$EXTENSION"

./.build/prog/$EXECUTABLE .build/config/scan.cfg


echo "Setting up Project Information..."
echo "---------------------------------"
echo ""

MAIN_DIR="main"
SRC_DIR="std"

PROGNAME="projectScan"

# Command line argument gives which version of the program to run
EXTENSION=$1

EXECUTABLE="$PROGNAME$EXTENSION"

./.build/prog/$EXECUTABLE $MAIN_DIR c .build/config/programs.txt 0
./.build/prog/$EXECUTABLE $SRC_DIR c .build/config/components.txt 0

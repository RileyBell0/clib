echo "Creating Makefile from Project Information..."
echo "---------------------------------------------"
echo ""

PROGNAME="projectMake"
# Command line argument gives which version of the program to run
EXTENSION=$1

EXECUTABLE="$PROGNAME$EXTENSION"

./.build/prog/$EXECUTABLE ./.build/config/.requiredFiles.txt ./.build/config/.mainFiles.txt
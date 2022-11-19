echo "---------------------------------------------"
echo "Creating Makefile from Project Information..."

# Required Variables
CONFIG=".compile/cfg/make.cfg"
EXECUTABLE="./.compile/prog/make"

###############################################################################

echo "Running \"$EXECUTABLE $CONFIG\""
echo 

$EXECUTABLE $CONFIG

if [ "$PROGNAME_END" = "WINDOWS" ]; then
  # Wait for keypress
  read -p "Press any key to continue..." -n1 -s
  echo
fi
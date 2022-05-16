echo "Creating Makefile from Project Information..."
echo "---------------------------------------------"
echo ""

# Changeable Vars

BASE_NAME=".compile"
PROG_FOLDER="prog"
CONFIG_FOLDER="cfg"
PROG_NAME="projectMake"
CFG_EXT=".cfg"

###############################################################################
###############################################################################

PROGNAME_START="./$BASE_NAME/$PROG_FOLDER/$PROG_NAME"
MAKE_CONFIG_LOC="$BASE_NAME/$CONFIG_FOLDER/"
PROGNAME_END="Unknown"
EXTENSION=""

case "$OSTYPE" in
  solaris*) PROGNAME_END="SOLARIS" ;;
  darwin*)  PROGNAME_END="OSX" ;; 
  linux*)   PROGNAME_END="LINUX" ;;
  bsd*)     PROGNAME_END="BSD" ;;
  msys*)    PROGNAME_END="WINDOWS" EXTENSION=".exe" ;;
  *)        echo "unknown os type: $OSTYPE" ;;
esac

echo "Os Type: $PROGNAME_END"

EXECUTABLE="$PROGNAME_START$PROGNAME_END$EXTENSION"
CONFIG="$MAKE_CONFIG_LOC$PROGNAME_END$CFG_EXT"

echo 
echo "Running \"$EXECUTABLE $CONFIG\""
echo 

$EXECUTABLE $CONFIG

if [ "$PROGNAME_END" = "WINDOWS" ]; then
  # Wait for keypress
  read -p "Press any key to continue..." -n1 -s
  echo
fi
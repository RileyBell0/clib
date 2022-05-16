
echo "Setting up Project Information..."
echo "---------------------------------"
echo ""

# Changeable Vars

BASE_NAME=".compile"
PROG_FOLDER="prog"
CONFIG_FOLDER="cfg"
PROG_NAME="projectScan"
SCAN_CFG_FILENAME="scan.cfg"

###############################################################################
###############################################################################

PROGNAME_START="./$BASE_NAME/$PROG_FOLDER/$PROG_NAME"
PROGNAME_END="Unknown"
SCAN_CFG_LOC="$BASE_NAME/$CONFIG_FOLDER/$SCAN_CFG_FILENAME"
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

echo 
echo "Running \"$EXECUTABLE $SCAN_CFG_LOC\""
echo 

$EXECUTABLE $SCAN_CFG_LOC

if [ "$PROGNAME_END" = "WINDOWS" ]; then
  # Wait for keypress
  read -p "Press any key to continue..." -n1 -s
  echo
fi
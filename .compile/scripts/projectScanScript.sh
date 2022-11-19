
echo "-----------------------"
echo "Finding source files..."

# Changeable Vars

BASE_NAME=".compile"
PROG_FOLDER="prog"
CONFIG_FOLDER="cfg"
PROG_NAME="scan"
SCAN_CFG_FILENAME="scan.cfg"

###############################################################################
###############################################################################

PROGNAME_START="./$BASE_NAME/$PROG_FOLDER/$PROG_NAME"
OS_NAME="Unknown"
SCAN_CFG_LOC="$BASE_NAME/$CONFIG_FOLDER/$SCAN_CFG_FILENAME"
EXTENSION=""

case "$OSTYPE" in
  solaris*) OS_NAME="SOLARIS" ;;
  darwin*)  OS_NAME="OSX" ;; 
  linux*)   OS_NAME="LINUX" ;;
  bsd*)     OS_NAME="BSD" ;;
  msys*)    OS_NAME="WINDOWS" EXTENSION=".exe" ;;
  *)        echo "unknown os type: $OSTYPE" ;;
esac


EXECUTABLE="$PROGNAME_START$EXTENSION"

echo "Running \"$EXECUTABLE $SCAN_CFG_LOC\""
echo ""

$EXECUTABLE $SCAN_CFG_LOC

if [ "$OS_NAME" = "WINDOWS" ]; then
  # Wait for keypress
  read -p "Press any key to continue..." -n1 -s
  echo
fi
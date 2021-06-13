
echo "Setting up Project Information..."
echo "---------------------------------"
echo ""

#MAIN_DIR="main"
#SRC_DIR="src"
PROGNAME_START="./.build/prog/projectScan"
PROGNAME_END="Unknown"
SCAN_CFG_LOC=".build/config/scan.cfg"
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
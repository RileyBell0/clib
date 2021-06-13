echo "Creating Makefile from Project Information..."
echo "---------------------------------------------"
echo ""

PROGNAME_START="./.build/prog/projectMake"
PROGNAME_END="Unknown"
MAKE_CONFIG_LOC=".build/config/project_cfg_"
CFG_EXT=".cfg"
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
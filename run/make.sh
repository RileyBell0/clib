
echo "---------------------------------"
echo "Running Makefile for Project..."
echo "Only warnings will be shown"
echo ""

# Changeable Vars
MAKEFILE_LOC="Makefile"

###############################################################################
###############################################################################

mkdir -p output/build
mkdir -p output/objects

MAKE_PATH="$COMPILE_BASE/$MAKE_FOLDER/"
CURRENT_OS="Unknown"

case "$OSTYPE" in
  solaris*) CURRENT_OS="SOLARIS" ;;
  darwin*)  CURRENT_OS="OSX" ;; 
  linux*)   CURRENT_OS="LINUX" ;;
  bsd*)     CURRENT_OS="BSD" ;;
  msys*)    CURRENT_OS="WINDOWS" ;;
  *)        echo "unknown os type: $OSTYPE" ;;
esac

echo make -f"$MAKEFILE_LOC" -s
make -f"$MAKEFILE_LOC" -s

echo ""
echo "Finished running makefile!"

if [ "$CURRENT_OS" = "WINDOWS" ]; then
  read -p "Press any key to continue..." -n1 -s
  echo
fi
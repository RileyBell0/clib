# Scan directory for changes and generate the makefile
./.compile/scripts/projectScanScript.sh
./.compile/scripts/projectMakeScript.sh 

mkdir -p output/build
mkdir -p output/objects

# Compile
echo "----------------------------"
echo "Compiling with the Makefile"
echo "----------------------------"
echo "Only warnings will be shown"
echo make -s
make -s
echo ""
echo "Finished running makefile!"

CURRENT_OS="Unknown"
case "$OSTYPE" in
  solaris*) CURRENT_OS="SOLARIS" ;;
  darwin*)  CURRENT_OS="OSX" ;; 
  linux*)   CURRENT_OS="LINUX" ;;
  bsd*)     CURRENT_OS="BSD" ;;
  msys*)    CURRENT_OS="WINDOWS" ;;
  *)        echo "unknown os type: $OSTYPE" ;;
esac
if [ "$CURRENT_OS" = "WINDOWS" ]; then
  read -p "Press any key to continue..." -n1 -s
  echo
fi
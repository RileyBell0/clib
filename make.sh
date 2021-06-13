
CURRENT_OS="Unknown"
MAKEFILE_BASE="Makefile_"

echo "Cool"

case "$OSTYPE" in
  solaris*) CURRENT_OS="SOLARIS" ;;
  darwin*)  CURRENT_OS="OSX" ;; 
  linux*)   CURRENT_OS="LINUX" ;;
  bsd*)     CURRENT_OS="BSD" ;;
  msys*)    CURRENT_OS="WINDOWS" ;;
  *)        echo "unknown os type: $OSTYPE" ;;
esac

make -f"$MAKEFILE_BASE$CURRENT_OS" -s

if [ "$CURRENT_OS" = "WINDOWS" ]; then
  read -p "Press any key to continue..." -n1 -s
  echo
fi
# Define where to find scan executable and overall config file
EXECUTABLE="./.compile/prog/scan"
CONFIG=".compile/cfg/compile.cfg"

# Run the code
echo "----------------------------"
echo "Scanning Project Directories"
$EXECUTABLE $CONFIG
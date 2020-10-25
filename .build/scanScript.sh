
echo "Setting up Project Information..."
echo "---------------------------------"
echo ""

MAIN_DIR="main"
SRC_DIR="std"

./.build/prog/projectScan $MAIN_DIR c .build/config/.mainFiles.txt 0
./.build/prog/projectScan $MAIN_DIR c .build/config/.mainNames.txt 1
./.build/prog/projectScan $SRC_DIR c .build/config/.requiredFiles.txt 0
./.build/prog/projectScan "src" c .build/config/.requiredFiles.txt 0 1

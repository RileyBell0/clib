
echo "Setting up Project Information..."
echo "---------------------------------"
echo ""

./.build/prog/projectScan main c .build/config/.mainFiles.txt 0
./.build/prog/projectScan main c .build/config/.mainNames.txt 1
./.build/prog/projectScan std c .build/config/.requiredFiles.txt 0

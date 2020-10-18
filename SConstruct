# Global Variables
files = []
libs = []

# Helper Functions (and pretty formatting)
def obj(path):
    subtitle(("Registering build of: " + path))
    Object(path)
def prog(outName, path):
    subtitle(("Registering build of: " + path))
    Program(outName, path)
def prog(path):
    subtitle(("Registering build of: " + path))
    Program(outName, path)
def title(string):
    bar = ""
    for i in range(len(string)):
        bar += "-"
    print("\n"+bar)
    print(string)
    print(bar+"\n")
def subtitle(string):
    print("\n\t" + string)
    bar = ""
    for i in range(len(string)):
        bar += "-"
    print("\t" + bar)
def subText(string):
    print("\t\t"+string)
def subTextFile(src, name, extension):
    finalString = "\t\t+ " + name + "." + extension
    toAdd = ""
    for i in range(30-len(finalString)):
        toAdd += " "
    finalString += toAdd + " " + src + "/" + name + "." + extension
    print(finalString)
def makePath(src,file,extension):
    if (len(extension) != 0):
        return src+"/"+file+"."+extension
    return src+"/"+file
def addFileA(src, name, extension):
    path = makePath(src,name,extension)
    subTextFile(src,name,extension)
    files.append(path)
def addFile(path):
    subText("+ " + path)
    files.append(path)
def addFiles(names, src, extension):
    for theName in names:
        srcDir = src + "/"+ theName
        addFileA(srcDir, theName, extension)
def endSetup():
    print()
    print()
    print("All Files Added")
    print()
    print("BEGIN COMPILATION:")
    print("-------------------------------------")

##########################################################################
# Config START
##########################################################################

# Main program information - no main file, all objects
extension = 'c'

# CLIB Dependencies
clibSrc = '/usr/include/clib/src/std'
cFiles = [
    'array',
    'string',
    'list',
    'general',
    'fileIO',
    'bintree'
]



##########################################################################
# COMPILATION START
##########################################################################

title("SCONS - Setup START")

subtitle("Adding CLIB Files")
addFiles(cFiles, clibSrc, extension)

endSetup()

Object(, source = files, LIBS = libs)

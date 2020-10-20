# HOW TO USE
#   if you have libraries or stuff you need to link, add them to the 'libs' array

import os


# read in source files
def readConfigFile(sources, config):
    lines = []
    
    # append config dir
    for index, source in enumerate(sources):
        sources[index] = config + source

    # read in lines
    for name in sources:
        with open(name) as f:
            lines = lines + f.readlines()
    lines = [x.strip() for x in lines]
    f.close()
    return lines

# Global Variables
configDir = '.build/config/'
mainfiles = ['.mainFiles.txt']
mainnames = ['.mainNames.txt']
filenames = ['.requiredFiles.txt']

progfiles = readConfigFile(mainfiles, configDir)
files = readConfigFile(filenames, configDir)
prognames = readConfigFile(mainnames, configDir)


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
    print(bar)
    print(string)
    print(bar)
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
def addFile(path):
    subText("+ " + path)
    files.append(path)
def addFiles(names, src, extension):
    for theName in names:
        srcDir = src + "/" + extension
        addFileA(srcDir, theName, extension)
def endSetup():
    print()
    print("Additional Setup Complete")
    print()
    print("BEGIN COMPILATION:")
    print("-------------------------------------")

##########################################################################
# Config START
##########################################################################

libs = ['m']
buildDir = 'build'

##########################################################################
# COMPILATION START
##########################################################################

title("SCONS - Setup START")

#put extra setup stuff here

endSetup()
if (len(prognames) != 0):
    targetName = prognames[0]
    index = 0
    print(str(len(prognames)) + " Program files found..")
    subtitle("Compiling " + str(len(prognames)) + " programs to 'build'")
    for prog in progfiles:
        targetName = prognames[index]
        subText("+ " + targetName)
        Program(target = (buildDir + '/' + targetName), source = [prog] + files, LIBS = libs)

        index += 1
else:
    print("No program files found")
    subtitle("Adding Files to Compile...")
    for file in files:
        subText("+ " + file)
        Object(source =  file, LIBS = libs)
print()
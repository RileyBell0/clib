# CLib

CLib is a library of common functions and data-types that are often needed in coding. While not as efficient in all cases, it does make programming far easier in C by limiting how much re-inventing of the wheel is needed

# Installation

## Installation - UBUNTU (or similar)

1) Copy '/clib' to '/usr/include'

2) Change the permissions of the folder '/usr/include/clib' to allow
anyone to edit the file.  

You can edit the permissions in multiple ways but there are two 'easier' methods i'd reccomend
If your file browser is nautilus (you havent changed it), Open a terminal and yupe

```bash
sudo nautilus
```

Enter your password, You should now see a 'root' version of your file explorer appear

1. Browse to /usr/include

2. Open the 'Properties' of '/usr/include/clib' (right click and find it in the context menu)
3. Choose the 'Permissions' tab
4. Alter all access fields to 'Create and delete files'
5. Next, at the base of the properties window select "Change Permissions for Enclosed Files..."
6. Ensure 'Read and Write' is selected for the left column 'Files'
7. Change all permisions for the rightmost column 'Folders' to 'Create and Delete Files'
8. Confirm your changes by clicking 'Change' at the top right  

Now you can include clib files like so

```c
// To include a clib array
#include <clib/src/std/array/array.h>
```

Of course, you could alter the directory setup, where at the cost of a more annoying file structure to deal with when implementing changes to the base clib library you could include files by simply typing

```c
#include <clib/array.h>
```

## Installation - Not-Ubuntu

You're pretty much on your own, im an ubuntu man through and through, I could never get windows to work for me when coding and I couldn't afford a mac.

There are a couple of maybe slightly-helpful pointers I can give.

1. Copy the 'clib' folder somewhere other than your downloads folder, trust me, you dont want it there

2. You can use the absolute path to 'clib' to include it into files

Now there's an easy way and a hard way to include 'clib' if you cant use "#include <>"  
You can (if you want), hardcode the path in to clib every time, but that's time consuming and really gross  

```c
// It could be as gross as this
#include "C:\User\Your_Name\Documents\Libraries\clib\src\std\array\array.h"

// Nobody wants to type that in every time
```

So instead reccomend creating a 'dummy' header file to make including clib files easier

```cpp
// Easier include's (predefined)
#define CLIB_ARRAY "C:\\User\\Your_Name\\Documents\\Libraries\\clib\\src\\array\\array.h"
#define CLIB_LIST "C:\\User\\Your_Name\\Documents\\Libraries\\clib\\src\\list\\list.h"
#define CLIB_FILEIO "C:\\User\\Your_Name\\Documents\\Libraries\\clib\\src\\fileIO\\fileIO.h"

// etc
```

This way, (assuming this file is called 'clib.h') you can just include this file when you intend to use 'clib'
into a project and use its defines to ensure you're including clib files correctly and easily.  

For example:

```c
// Your_Program
// Lets say you wanted to include the 'array' file from clib

// Make your life easy and dont type out the path, include "clib.h"
#include "C:\\libs\\clib.h"
#include CLIB_ARRAY

// And you're done, clib's array is included, and you don't have to type out any long file names
```

# Usage

This library uses **SCons** to compile, with the intention of this being simpler and easier to understand than extensive makefiles.

## SCons

**[SCons](https://scons.org/)** is used to compile this library. The compilation of the library can be modified by editing the **SConstruct** file in the root directory of this repository.


## Compiling the included files

```bash
cd "path_to_repository/clib"
scons
```

## Adding new files to the library

Adding new files and functionality is very simple, yet there are some principles regarding structure which will make your life easier if you follow.

A SConstruct script is used to compile the library - making the addition of new objects in need of compilation much easier for the end user.

```python
stdSrc = '/usr/include/clib/src/std'

stdFiles =
[
    'array',
    'string',
    'list',
    'general',
    'fileIO',
    'bintree'
]

#######################################

#######################################

subtitle("Adding CLIB Files")
addFiles(cFiles, clibSrc, extension)
```
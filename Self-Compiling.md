# Compile

A script designed to make compiling easier.  

This is designed to work with the C coding language, other languages have not been tested but may work also

Supported languages are all languages supported by SCons. At the time of writing this such languages are:  

    C, C++, D, Java, Fortran, Yacc, Lex, Qt and SWIG

# Installation

Copy the files and folders into the main directory of your C project

**[SCons](https://scons.org/)** is used to compile this library. The compilation of the library can be modified by editing the **SConstruct** file in the root directory of this repository.  

It is not reccomended to edit the **SConstruct** file, and instead to follow the reccomended file structure for the program. Following this will make the compilation of the project easier.  


# Usage

## Setup

Before compiling, run

```bash
cd project_base_directory
./setup
```

and select all directories which include your project files.  

## Compilation

run 'compile'

```bash
cd project_base_directory
./compile
```

the output and finalised files will be placed in the 'build' directory of your main project directory.

# Project Structure

Use a directory structure as follows

    Base_Project_Directory
        - other files
        - FOLDER .glassBuild
        - FOLDER build
        - FOLDER main
            Put program entrypoint files here

        - FOLDER SOURCE DIRECTORIES
            - Put Header files in here
        
            - FOLDER extension
                - Put source files in here. For example if writing in 'C' 
                  put the headerfiles in the above directory and the '.c' 
                  files in this directory


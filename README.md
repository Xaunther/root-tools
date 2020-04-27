## Summary

Place where common root scripts are defined. These scripts are independent from each other (althought the output of one might be an input for another) but might be used in different analyses.

## Set-up

Need the developed [analysis root code](https://gitlab.cern.ch/aalfonso-Analysis-Tools) (3 repositories, Directories is not actually needed). Create a new folder and inside that you must have the 3 analysis tools folders (there may be other folders inside). Instructions to compile the code are in the [root repository](https://gitlab.cern.ch/aalfonso-Analysis-Tools/root).<br />
```bash
Some_Folder
├── Dictionaries
├── Functions
├── root
├── OTHER
```

The package has been developed under the latest ROOT version available, which at the time of writing this is `ROOT 6.20/02`.

## Compilation with ACLiC

 The classical way to use this version in lxplus is by doing `lb-run ROOT PATH_TO_THIS_FOLDER/run`. This will call for [rootinit.C](https://gitlab.cern.ch/aalfonso-Analysis-Tools/root/blob/master/rootinit.C), which will load and compile, if necessary, the libraries in root.
 Using an alias is highly recommended, for example in lxplus I added to my .bashrc `alias rot="lb-run ROOT /afs/cern.ch/work/a/aalfonso/private/root/run"`, which allows me to open root with the libraries loaded by just typing `rot` (flags (-b, -l...) apply as usual).

## Compilation with gcc

The recommended way to use the package is compiling the code with gcc, so each .C script is executable. This allows for faster processing, as it doesn't need to load root and all the libraries every time.
It has been compiled on lxplus, where all the libraries needed are available (roofit, roofitcore, roostats, etc.). It has also been tested in local PCs, the only requirement is to build root

To compile, take into account the following:
* The compiler version must be newer than gcc7, older version may not work with some ROOT features.
* The ROOT version to be used can be seen using `root-config --version`, and specified by running the `thisroot.sh` script provided in the ROOT installation folder

For example, in lxplus you can set up gcc9 compiler (default is gcc4...) using:
```bash
export CC=/cvmfs/lhcb.cern.ch/lib/lcg/releases/gcc/9.2.0-afc57/x86_64-centos7/bin/gcc
export CXX=/cvmfs/lhcb.cern.ch/lib/lcg/releases/gcc/9.2.0-afc57/x86_64-centos7/bin/g++
```
And `ROOT 6.20/02` can be manually specified using:
```bash
source /cvmfs/lhcb.cern.ch/lib/lcg/releases/ROOT/v6.20.02-10e75/x86_64-centos7-gcc9-opt/bin/thisroot.sh
```

Once you have chosen your compiler and root versions, just go into [root build folder](https://gitlab.cern.ch/aalfonso-Analysis-Tools/root/build), and run:
```
cmake .. && make
```
It should configure and compile all the .C scripts into their respective .out executables. To run any of them, simply write:
```
<path-to-this-root-folder>/<script-name>.out <arg1> <arg2> .... <argN>
```

## Small disclaimer

There are parts of the code which will try to access some files stored under the path `./Dictionaries/`, which is present in the [NstG](https://gitlab.cern.ch/aalfonso/NstGamma/tree/master) or [isospin](https://gitlab.cern.ch/aalfonso/KstplusGamma) analyses repositories. If the files are not found the scripts may fall into infinite loops and errors, so it is strongly recommended to use these libraries only from such analysis folders.
## Author

Alejandro Alfonso Albero (alejandro.alfonso.albero@cern.ch)
## Summary

Place where common root scripts are defined. These scripts are independent from each other (althought the output of one might be an input for another) but might be used in different analyses.

## Compilation

Need the developed [analysis root code](https://gitlab.cern.ch/aalfonso-Analysis-Tools) (3 repositories, Directories is not actually needed). Create a new folder and inside that you must have the 3 analysis tools folders (there may be other folders inside). Instructions to compile the code are in the [root repository](https://gitlab.cern.ch/aalfonso-Analysis-Tools/root).<br />
```bash
Some_Folder
├── Dictionaries
├── Functions
├── root
├── OTHER
```
The package has been developed under the latest ROOT version available, which at the time of writing this is `ROOT 6.12/06`. The easiest way to use this version in lxplus is by doing `lb-run ROOT PATH_TO_THIS_FOLDER/run`. This will call for [rootinit.C](https://gitlab.cern.ch/aalfonso-Analysis-Tools/root/blob/master/rootinit.C), which will load and compile, if necessary, the libraries in root.

Using an alias is highly recommended, for example in lxplus I added to my .bashrc `alias rot="lb-run ROOT /afs/cern.ch/work/a/aalfonso/private/root/run"`, which allows me to open root with the libraries loaded by just typing `rot` (flags (-b, -l...) apply as usual).

## Small disclaimer

There are parts of the code which will try to access some files stored under the path `./Dictionaries/`, which is present in the [NstG](https://gitlab.cern.ch/aalfonso/NstGamma/tree/master) or [isospin](https://gitlab.cern.ch/aalfonso/KstplusGamma) analyses repositories. If the files are not found the scripts may fall into infinite loops and errors, so it is strongly recommended to use these libraries only from such analysis folders.
## Author

Alejandro Alfonso Albero (alejandro.alfonso.albero@cern.ch)
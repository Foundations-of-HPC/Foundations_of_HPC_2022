# ORFEO module system introduction
## What is a module system ? 

A module system is a software that help us to manage our environment ( try to explore it using the command `env` ) and the scientific software stack. 

The environmental modules *add*, *modify* and *remove* environmental variables in a coerent way. 

## Basic and usefull commands 

In this tutorial we explore the basic feature and the lagacy usage of a module system with a particular focus in MPI and ORFEO environment. 


### `module avail` 

The command `module avail` or `ml avail` lists all available modules at the moment in the cluster. 

Into the **login** node with a fresh session we get the following output:

```
-------------------------- /opt/area/shared/modules_v2/profiles ---------------------------
   architecture/AMD    architecture/Intel    architecture/x86 (D)

```

Three different modules (or *profiles*) are available:

* `architecture/AMD` is the *profile* that unveils all modules compiled and optimized for **AMD** processors (*zen3* family)
* `architecture/Intel` is the *profile* that unveils all modules compiled and optimized for **Intel** processors (*skylake* family)
* `architecture/x86` is the *profile* that unveils all module compiled with a restricted set of instruction common to all *x86* architectures, all software loaded from this modules are expected to be outperformed by architecture specific software, please don't use that profile in *HPC* environment.

Press **q** to **exit** from available modules list.

**Note**: you have to select an architecture otherwise it is not possible to load any additional software.




### `module load`

Let's load a specific profile with `module load architecture/AMD`.

Our environment is changed, more modules are now available. Let's check with 
'module avail' or the shortcut `ml avail`.

The output is now more rich of software:

```
------------------------ /opt/area/shared/modules_v2/AMD/libraries ------------------------
   mkl/latest    mkl/2022.2.1 (D)    openBLAS/0.3.21

---------------------------- /opt/area/shared/modules_v2/AMD/mpi ----------------------------
   intelMPI/2021.7.1    openMPI/4.1.4/gnu/12.2.1    openMPI/4.1.4/icx/2022.2.1

--------------------------- /opt/area/shared/modules_v2/profiles ---------------------------
   architecture/AMD (L)    architecture/Intel    architecture/x86 (D)
 
```


Analyze the output:

Up to now there are 3 sections : *libraries*, *mpi* and *profiles* where the modules are collected. 

**Note:** The profile *architecture/AMD (L)* is marked as *Loaded*.

Others module are listed and available, but not loaded. 

Try running the command `mpicc` or `mpirun` and you will trigger an error.

To load openMPI use the following command:

`module load openMPI/4.1.4/gnu/12.2.1`

This will load openMPI version *4.1.4* that uses GNU Compiler Collection version *12.2.1*  (remember: `mpicc` is just a wrapper!).


Now our environment is changed again, search for new openmpi environment variables:

```
[ntosato@login02 ~]$ env | grep MPI
MPI_LIB=/opt/area/shared/programs//AMD/openMPI/4.1.4/lib
OMPI_MCA_btl=^openib
__LMOD_REF_COUNT__LMFILES_=/opt/area/shared/modules_v2/profiles/architecture/AMD.lua:1;/opt/area/shared/modules_v2/AMD/mpi/openMPI/4.1.4/gnu/12.2.1.lua:1
MANPATH=/opt/area/shared/programs/AMD/openMPI/4.1.4/share/man:/usr/share/lmod/lmod/share/man::
MPI_INCLUDE=/opt/area/shared/programs//AMD/openMPI/4.1.4/include
MPI_HOME=/opt/area/shared/programs//AMD/openMPI/4.1.4
M
...
...
...
```


And is finally possible to use `mpicc` or `mpirun` ! But why ? 

The path to the folder that contain `mpicc` and `mpirun` executable is now present into the environment variable 'PATH'.

```
PATH=/opt/area/shared/programs/AMD/openMPI/4.1.4/bin:/usr/local/bin:/usr/bin:/usr/local/sbin:/usr/sbin:/u/area/ntosato/.local/bin:/u/area/ntosato/bin

```



### `module list`

To keep track of the loaded modules `module list` show all currently loaded modules.
A shortcut of 'module list' is simply 'ml'

```
Currently Loaded Modules:
  1) architecture/AMD   2) openMPI/4.1.4/gnu/12.2.1

```


### `module unload`
If for some reason the user need to unload some software, the command `module unload [modulename]` will do the trick. 


*Exercise:* Try to unload the openMPI module and verify that the module is effectively unloaded by inspecting your environment variables and also by checking the output of the command 'module list'

### `module purge`

If a lot of modules are loaded or a refresh of the environment is necessary, 
`module purge` unload all currently loaded modules. 


## What if multiple MPI modules are loaded ?
What if the openMPI module is loaded and the user try to load also the IntelMPI implementation ? 

```
[ntosato@login02 ~]$ module load openMPI/4.1.4/gnu
[ntosato@login02 ~]$ module load intelMPI/2021.7.1
Loading mpi version 2021.7.1

Lmod is automatically replacing "openMPI/4.1.4/gnu/12.2.1" with "intelMPI/2021.7.1".

```


The module system automatically replace the openMPI module with the Intel module, the enviroment is modified always in a coerent way, avoiding much as possible software clashes.


The same beahaviour can be observed when we change the compiler wrapped into `mpicc`.

```
[ntosato@login02 ~]$ module load openMPI/4.1.4/gnu/12.2.1 
[ntosato@login02 ~]$ module load openMPI/4.1.4/icx/2022.2.1 
Loading compiler version 2022.2.1
Loading tbb version 2021.7.1
Loading compiler-rt version 2022.2.1
Loading oclfpga version 2022.2.1
  Load "debugger" to debug DPC++ applications with the gdb-oneapi debugger.
  Load "dpl" for additional DPC++ APIs: https://github.com/oneapi-src/oneDPL

Lmod is automatically replacing "openMPI/4.1.4/gnu/12.2.1" with "openMPI/4.1.4/icx/2022.2.1".
...
...
...
```

The module system replaces the previous module with the last one selected, now a openMPI version that wrap the new Intel `icx` compiler is available.

```
[ntosato@login02 ~]$ mpicc --version
Intel(R) oneAPI DPC++/C++ Compiler 2022.2.1 (2022.2.1.20221020)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /opt/area/shared/programs/x86/oneAPI/2022.3.1/compiler/2022.2.1/linux/bin-llvm
Configuration file: /opt/area/shared/programs/x86/oneAPI/2022.3.1/compiler/2022.2.1/linux/bin/icx.cfg

```


## Swap architecture

To hot swap the *loaded* and the *available* software stack without starting a new session or purge the entire environment, is sufficient to load the desired profile.

Let's try to load *AMD* optimized openMPI software:

```
[ntosato@login02 ~]$ module load architecture/AMD
[ntosato@login02 ~]$ module load openMPI/4.1.4/gnu/12.2.1 
```

And inspect the true location of `mpicc` executable.

```
[ntosato@login02 ~]$ which mpicc
/opt/area/shared/programs/AMD/openMPI/4.1.4/bin/mpicc
```

Now change the architecture profile:

```
[ntosato@login02 ~]$ module load architecture/Intel

Due to MODULEPATH changes, the following have been reloaded:
  1) openMPI/4.1.4/gnu/12.2.1

The following have been reloaded with a version change:
  1) architecture/AMD => architecture/Intel

```

The module system reloads openMPI module, this can be verified with `which`.

```
[ntosato@login02 ~]$ which mpicc
/opt/area/shared/programs/Intel/openMPI/4.1.4/bin/mpicc
[ntosato@login02 ~]$ 

```
 
The `mpicc` location is now different and the executables are 2 different file. 


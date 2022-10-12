 
 
 # a short tour on ORFEO HPC facility 

- how to login into ORFEO
- how to use modules 
- how to use SLURM (provided by N.Tosato) 
- how to compile in serial
- how to compile an MPI application 

## logging in and exploring home directory

Login:
```
$ ssh username@195.14.102.215 
```

Once logged you will land in your home directory on login02 node, a dedicate machine for DSSC students.

```
Last login: Sat Oct  9 20:27:21 2021 from host-79-17-109-168.retail.telecomitalia.it

        __     _____    ______    __
       /  \   |  __ \  |______|  /  \
      / /\ \  | |  ) )  ____    / /\ \
     / __)\ \ | | / /  |____|  / __)\ \
    / /    \ \| | \ \_________/ /    \ \
   /_/      \___|  `.__________/      \_\
   __  __   __     __ __    __  __  __
  (__ /   |/_ |\ |/  /_    [__)[__][__)|_/
   __)\__ |\__| \|\__\__   |   |  ||  \| \

   Welcome to ORFEO login node dedicated to DSSC students
   [cozzini@login02 ~]$
```

Let us check our home directory

```
[cozzini@login ~]$ pwd
/u/area/cozzini
[cozzini@login ~]$

```

Let us check some important environment variable:

```
[cozzini@login02 ~]$ env
SHELL=/bin/bash
HISTCONTROL=ignoredups
HISTSIZE=1000
HOSTNAME=login02.hpc
_ModuleTable002_=Y2F0aW9ucyIsICIvb3B0L2FyZWEvc2hhcmVkL21vZHVsZXMvbXBpIiwgIi9ldGMvbW9kdWxlZmlsZXMiLCAiL3Vzci9zaGFyZS9tb2R1bGVmaWxlcyIKLCAiL3Vzci9zaGFyZS9tb2R1bGVmaWxlcy9MaW51eCIsICIvdXNyL3NoYXJlL21vZHVsZWZpbGVzL0NvcmUiLCAiL3Vzci9zaGFyZS9sbW9kL2xtb2QvbW9kdWxlZmlsZXMvQ29yZSIsCn0sCnN5c3RlbUJhc2VNUEFUSCA9ICIvZXRjL21vZHVsZWZpbGVzOi91c3Ivc2hhcmUvbW9kdWxlZmlsZXM6L3Vzci9zaGFyZS9tb2R1bGVmaWxlcy9MaW51eDovdXNyL3NoYXJlL21vZHVsZWZpbGVzL0NvcmU6L3Vzci9zaGFyZS9sbW9kL2xtb2QvbW9kdWxlZmlsZXMvQ29yZSIsCn0K
__LMOD_REF_COUNT_MODULEPATH=/opt/area/shared/modules/contribs:1;/opt/area/shared/modules/libraries:1;/opt/area/shared/modules/compilers:1;/opt/area/shared/modules/utilities:1;/opt/area/shared/modules/environments:1;/opt/area/shared/modules/testing:1;/opt/area/shared/modules/applications:1;/opt/area/shared/modules/mpi:1;/etc/modulefiles:1;/usr/share/modulefiles:1;/usr/share/modulefiles/Linux:1;/usr/share/modulefiles/Core:1;/usr/share/lmod/lmod/modulefiles/Core:1
LMOD_DIR=/usr/share/lmod/lmod/libexec
EDITOR=/usr/bin/vim
PWD=/u/area/cozzini
LOGNAME=cozzini
XDG_SESSION_TYPE=tty
MODULESHOME=/usr/share/lmod/lmod
MANPATH=/usr/share/lmod/lmod/share/man:
MOTD_SHOWN=pam
HOME=/u/area/cozzini
_ModuleTable_Sz_=2
LANG=C.UTF-8
LS_COLORS=rs=0:di=01;34:ln=01;36:mh=00:pi=40;33:so=01;35:do=01;35:bd=40;33;01:cd=40;33;01:or=40;31;01:mi=01;37;41:su=37;41:sg=30;43:ca=30;41:tw=30;42:ow=34;42:st=37;44:ex=01;32:*.tar=01;31:*.tgz=01;31:*.arc=01;31:*.arj=01;31:*.taz=01;31:*.lha=01;31:*.lz4=01;31:*.lzh=01;31:*.lzma=01;31:*.tlz=01;31:*.txz=01;31:*.tzo=01;31:*.t7z=01;31:*.zip=01;31:*.z=01;31:*.dz=01;31:*.gz=01;31:*.lrz=01;31:*.lz=01;31:*.lzo=01;31:*.xz=01;31:*.zst=01;31:*.tzst=01;31:*.bz2=01;31:*.bz=01;31:*.tbz=01;31:*.tbz2=01;31:*.tz=01;31:*.deb=01;31:*.rpm=01;31:*.jar=01;31:*.war=01;31:*.ear=01;31:*.sar=01;31:*.rar=01;31:*.alz=01;31:*.ace=01;31:*.zoo=01;31:*.cpio=01;31:*.7z=01;31:*.rz=01;31:*.cab=01;31:*.wim=01;31:*.swm=01;31:*.dwm=01;31:*.esd=01;31:*.jpg=01;35:*.jpeg=01;35:*.mjpg=01;35:*.mjpeg=01;35:*.gif=01;35:*.bmp=01;35:*.pbm=01;35:*.pgm=01;35:*.ppm=01;35:*.tga=01;35:*.xbm=01;35:*.xpm=01;35:*.tif=01;35:*.tiff=01;35:*.png=01;35:*.svg=01;35:*.svgz=01;35:*.mng=01;35:*.pcx=01;35:*.mov=01;35:*.mpg=01;35:*.mpeg=01;35:*.m2v=01;35:*.mkv=01;35:*.webm=01;35:*.webp=01;35:*.ogm=01;35:*.mp4=01;35:*.m4v=01;35:*.mp4v=01;35:*.vob=01;35:*.qt=01;35:*.nuv=01;35:*.wmv=01;35:*.asf=01;35:*.rm=01;35:*.rmvb=01;35:*.flc=01;35:*.avi=01;35:*.fli=01;35:*.flv=01;35:*.gl=01;35:*.dl=01;35:*.xcf=01;35:*.xwd=01;35:*.yuv=01;35:*.cgm=01;35:*.emf=01;35:*.ogv=01;35:*.ogx=01;35:*.aac=01;36:*.au=01;36:*.flac=01;36:*.m4a=01;36:*.mid=01;36:*.midi=01;36:*.mka=01;36:*.mp3=01;36:*.mpc=01;36:*.ogg=01;36:*.ra=01;36:*.wav=01;36:*.oga=01;36:*.opus=01;36:*.spx=01;36:*.xspf=01;36:
LMOD_SETTARG_FULL_SUPPORT=no
LMOD_VERSION=8.6.11
SSH_CONNECTION=2.234.152.104 50568 10.128.4.15 22
MODULEPATH_ROOT=/usr/share/modulefiles
XDG_SESSION_CLASS=user
LMOD_PKG=/usr/share/lmod/lmod
SELINUX_ROLE_REQUESTED=
TERM=xterm-256color
LESSOPEN=||/usr/bin/lesspipe.sh %s
USER=cozzini
SELINUX_USE_CURRENT_RANGE=
LMOD_ROOT=/usr/share/lmod
SHLVL=1
BASH_ENV=/usr/share/lmod/lmod/init/bash
LMOD_sys=Linux
XDG_SESSION_ID=229
_ModuleTable001_=X01vZHVsZVRhYmxlXyA9IHsKTVR2ZXJzaW9uID0gMywKY19yZWJ1aWxkVGltZSA9IGZhbHNlLApjX3Nob3J0VGltZSA9IGZhbHNlLApkZXB0aFQgPSB7fSwKZmFtaWx5ID0ge30sCm1UID0ge30sCm1wYXRoQSA9IHsKIi9vcHQvYXJlYS9zaGFyZWQvbW9kdWxlcy9jb250cmlicyIsICIvb3B0L2FyZWEvc2hhcmVkL21vZHVsZXMvbGlicmFyaWVzIiwgIi9vcHQvYXJlYS9zaGFyZWQvbW9kdWxlcy9jb21waWxlcnMiCiwgIi9vcHQvYXJlYS9zaGFyZWQvbW9kdWxlcy91dGlsaXRpZXMiLCAiL29wdC9hcmVhL3NoYXJlZC9tb2R1bGVzL2Vudmlyb25tZW50cyIsICIvb3B0L2FyZWEvc2hhcmVkL21vZHVsZXMvdGVzdGluZyIKLCAiL29wdC9hcmVhL3NoYXJlZC9tb2R1bGVzL2FwcGxp
XDG_RUNTIME_DIR=/run/user/10020001
SSH_CLIENT=2.234.152.104 50568 22
DEBUGINFOD_URLS=https://debuginfod.fedoraproject.org/
which_declare=declare -f
PATH=/usr/local/bin:/usr/bin:/usr/local/sbin:/usr/sbin:/u/area/cozzini/.local/bin:/u/area/cozzini/bin
SELINUX_LEVEL_REQUESTED=
MODULEPATH=/opt/area/shared/modules/contribs:/opt/area/shared/modules/libraries:/opt/area/shared/modules/compilers:/opt/area/shared/modules/utilities:/opt/area/shared/modules/environments:/opt/area/shared/modules/testing:/opt/area/shared/modules/applications:/opt/area/shared/modules/mpi:/etc/modulefiles:/usr/share/modulefiles:/usr/share/modulefiles/Linux:/usr/share/modulefiles/Core:/usr/share/lmod/lmod/modulefiles/Core
DBUS_SESSION_BUS_ADDRESS=unix:path=/run/user/10020001/bus
LMOD_CMD=/usr/share/lmod/lmod/libexec/lmod
MAIL=/var/spool/mail/cozzini
SSH_TTY=/dev/pts/3
BASH_FUNC_ml%%=() {  eval $($LMOD_DIR/ml_cmd "$@")
}
BASH_FUNC_which%%=() {  ( alias;
 eval ${which_declare} ) | /usr/bin/which --tty-only --read-alias --read-functions --show-tilde --show-dot $@
}
BASH_FUNC_module%%=() {  _mlshopt="f";
 case "$-" in
 *f*)
 unset _mlshopt
 ;;
 esac;
 if [ -n "${_mlshopt:-}" ]; then
 set -$_mlshopt;
 fi;
 eval $($LMOD_CMD bash "$@") && eval $(${LMOD_SETTARG_CMD:-:} -s sh);
 if [ -n "${_mlshopt:-}" ]; then
 set +$_mlshopt;
 fi;
 unset _mlshopt
}
_=/usr/bin/env
OLDPWD=/large
```

The most important one:

``` 
[cozzini@login02 ~]$ env | grep PATH
__LMOD_REF_COUNT_MODULEPATH=/opt/area/shared/modules/contribs:1;/opt/area/shared/modules/libraries:1;/opt/area/shared/modules/compilers:1;/opt/area/shared/modules/utilities:1;/opt/area/shared/modules/environments:1;/opt/area/shared/modules/testing:1;/opt/area/shared/modules/applications:1;/opt/area/shared/modules/mpi:1;/etc/modulefiles:1;/usr/share/modulefiles:1;/usr/share/modulefiles/Linux:1;/usr/share/modulefiles/Core:1;/usr/share/lmod/lmod/modulefiles/Core:1
MANPATH=/usr/share/lmod/lmod/share/man:
MODULEPATH_ROOT=/usr/share/modulefiles
PATH=/usr/local/bin:/usr/bin:/usr/local/sbin:/usr/sbin:/u/area/cozzini/.local/bin:/u/area/cozzini/bin
MODULEPATH=/opt/area/shared/modules/contribs:/opt/area/shared/modules/libraries:/opt/area/shared/modules/compilers:/opt/area/shared/modules/utilities:/opt/area/shared/modules/environments:/opt/area/shared/modules/testing:/opt/area/shared/modules/applications:/opt/area/shared/modules/mpi:/etc/modulefiles:/usr/share/modulefiles:/usr/share/modulefiles/Linux:/usr/share/modulefiles/Core:/usr/share/lmod/lmod/modulefiles/Core
[cozzini@login02 ~]$
``` 


## storage systems 

a quick look at filesystem:

```
cozzini@login02 ~]$ ls -lrt
total 2
lrwxrwxrwx 1 cozzini area 18 Apr  7 15:23 fast -> /fast/area/cozzini
lrwxrwxrwx 1 cozzini area 21 Apr  7 15:23 storage -> /storage/area/cozzini
lrwxrwxrwx 1 cozzini area 21 Apr 16 09:23 scratch -> /scratch/area/cozzini
```

check size:

```
[cozzini@login02 ~]$ df -h /large
Filesystem           Size  Used Avail Use% Mounted on
10.128.2.231:/large  555T  308T  248T  56% /large

```


## use module 

Software is encapsulated in modules, check avaliability and and load modules with:

```
$ module avail
$ module load
```

 - check enviroment 
 - load a module and check environment
 - unload a module and check enviroment again 


## Using SLURM local resource manager system 


Available nodes into **FHPC** partion.

```
|Node      |CPU                    |Socket  |Memory  |
|----------|-----------------------|--------|--------|
| epyc006  | AMD EPYC 7H12 64-Core | 2      | 512 GB |
| epyc007  | AMD EPYC 7H12 64-Core | 2      | 512 GB |
| epyc008  | AMD EPYC 7H12 64-Core | 2      | 512 GB |
```
### `sinfo`

`sinfo` prints information about nodes and partitions 

```
[ntosato@login02 ~]$ sinfo  
PARTITION AVAIL  TIMELIMIT  NODES  STATE NODELIST  
FHPC         up    2:00:00      1   unk* epyc006  
FHPC         up    2:00:00      1    mix epyc008  
FHPC         up    2:00:00      1  alloc epyc007
```

### Common options:

- `-l --long`  print more detailed information
- `-N --Node` print  information  in  a  node-oriented format with one line per node and partition

And more options can be specified at the same time

```
[ntosato@login02 ~]$ sinfo -lN 
Mon Oct 10 18:04:31 2022 
NODELIST  NODES PARTITION   	STATE  CPUS   S:C:T MEMORY TMP_DISK WEIGHT AVAIL_FE REASON        
epyc006     1    FHPC   	 unknown*  128   2:64:1 512000     	0    	1  (null)  none
epyc007     1    FHPC       allocated  128   2:64:1 512000     	0    	1  (null)  none         
epyc008     1    FHPC           mixed  128   2:64:1 512000     	0    	1  (null)  none 
```
`S:C:T` means **S**ocket:**C**ore:**T**hreads

- Useful output format, with very detailed informations about cluster status:

```
[ntosato@login02 ~]$ sinfo -N --format="%.15N %.6D %.10P %.11T %.4c %.10z %.8m %.10e %.9O %.15C" 
NODELIST  NODES  PARTITION      STATE  CPUS    S:C:T  MEMORY  FREE_MEM  CPU_LOAD  CPUS(A/I/O/T) 
epyc006    1         FHPC   unknown*    128   2:64:1  512000      N/A     N/A     0/128/0/128 
epyc007    1         FHPC  allocated    128   2:64:1  512000   507412    	0.00    128/0/0/128 
epyc008    1         FHPC      mixed    128   2:64:1  512000   509228    	0.00    84/44/0/128
```
`CPUS(A/I/O/T)` indicates **A**vailable/**I**dle/**O**ther/**T**otal cores

### `squeue`

`squeue` lists jobs in the queue.
```
[ntosato@login02 ~]$ squeue --long 
Mon Oct 10 18:17:07 2022 
       JOBID PARTITION   	NAME 	  USER    STATE    TIME TIME_LIMI  NODES NODELIST(REASON) 
          77      FHPC      bash   ntosato  PENDING    0:00  1:00:00   	 1 	(Resources) 
          74      FHPC    my_job   ntosato  RUNNING    5:40  1:00:00       1 	  epyc008 
          75      FHPC   my_job2   ntosato  RUNNING    5:29   40:00        1 	  epyc008 
          76      FHPC   my_job3   ntosato  RUNNING    5:23  1:00:00       1 	  epyc007
```

- Another useful custom output format to monitor queue:

```
[ntosato@login02 ~]$ squeue --format="%.8i %.9P %.8j %.8u %.9M %.6D %.5C %.7m %.20R %.8T" 
    JOBID PARTITION 	  NAME      USER    TIME  NODES  CPUS MIN_MEM   NODELIST(REASON)   STATE 
       77      FHPC     bash   ntosato    0:00      1    60    150G      (Resources)   PENDING 
       74      FHPC   my_job   ntosato    5:20      1    60    150G        epyc008     RUNNING 
       75      FHPC  my_job2   ntosato    5:09      1    24    250G        epyc008     RUNNING 
       76     FHPC   my_job3   ntosato    5:03      1   128    150G        epyc007     RUNNING
```

###  `srun`

`srun`  can be used to submit an *interactive* job as follow:

```
[ntosato@login02 ~]$ srun --partition FHPC --nodes 1 --tasks 12 --time=1:00:00 --pty bash  
[ntosato@epyc007 ~]$ squeue -l 
Tue Oct 11 00:40:53 2022 
       JOBID  PARTITION   NAME   	USER    STATE    TIME  TIME_LIMI  NODES NODELIST(REASON) 
          80 	   FHPC   bash   ntosato  RUNNING    0:04    10:00        1		 epyc007
```

Shorter version: `srun -p FHPC -N 1 -n 12 -t=1:00:00 --pty bash`

**Warning:** using `--pty bash` *interactive* session to launch **MPI** job across nodes will not work, use instead `salloc`.


### `sbatch`

`sbatch` can be used to submit a script that request resource and execute the job:

```
[ntosato@login02 ~]$ cat my_job.sh 
#!/bin/bash `
 `#SBATCH --partition=FHPC
#SBATCH --job-name=my_job
#SBATCH --ntasks=12 
#SBATCH --mem=1gb 
#SBATCH --time=00:05:00
#SBATCH --output=my_job_%j.out 
pwd; hostname; date 
echo "Hello, world !" 
```

And finally *submit* the script with:

`[ntosato@login02 ~]$ sbatch my_job.sh  
Submitted batch job 99`

Differently, requested resources as in the above example  (and options) can be specified directly by command line : `sbatch --partition FHPC --nodes 1 --tasks 1 --time=0:30:00 my_program.x`

#### Others common options:

- `--tasks-per-node=<#nodes>` specify the number of tasks for each node
- `-D , --chdir=<directory>`  set the working directory of the batch script to directory before it is executed, example of desired path `SLURM_SUBMIT_DIR`, available as enviroment variable
- `--exclusive` the job allocation can not share nodes with other running jobs
- `-o, --output=<file_path>`redirect stdout and stderr to the specified file, the default file name is "slurm-%j.out", where the "%j" is replaced by the job ID.
- see `man sbatch` and `man srun`, they have a lot of common options

### `salloc`

`salloc` obtain requested resources allocation, execute a command [ or program,script] and finally release the allocated resource when the command [or program,script]  is finished.  If a script is launched, it can run several `srun` instances. 

**Example: **`salloc -p FHPC --nodes 1 -tasks 4 <my_command>`

If no command is specified,  the default user shell is launched. 

*`salloc` functions similarly `srun --pty bash` in that it will add your resource request to the queue. However the allocation starts, a new bash session will start up on **the login node.** This is useful for running a GUI on the login node, but your processes on the compute nodes.* <a href=https://support.nesi.org.nz/hc/en-gb/articles/360001316356-Slurm-Interactive-Sessions> [1]</a>

Once resource are allocated, is possible to run *interactively* **MPI** program across nodes, using `mpirun` or `srun`. 

**Example** of `salloc` *interactive* use:

```
[ntosato@login02 src_c]$ salloc -p FHPC --nodes 2 --tasks-per-node 1 --time=00:30:00 
salloc: Granted job allocation 174 
[ntosato@login02 src_c]$ squeue  
  JOBID PARTITION   NAME   USER    TIME  NODES  CPUS MIN_MEM   NODELIST(REASON)   STATE 
   174    FHPC interact  ntosato    0:04    2     2      1G      epyc[006-007]  RUNNING 
[ntosato@login02 src_c]$ mpirun -np 2 <my_program> ...
```



### `scontrol`

To know **everything** about a specific job:

`scontrol show jobid <id>`

```
[ntosato@login02 ~]$ scontrol show jobid 178 
JobId=178 JobName=interactive 
  UserId=ntosato(10020023) GroupId=area(1002) MCS_label=N/A 
  Priority=4294901582 Nice=0 Account=(null) QOS=(null) 
  JobState=RUNNING Reason=None Dependency=(null) 
  Requeue=1 Restarts=0 BatchFlag=0 Reboot=0 ExitCode=0:0 
  RunTime=00:00:14 TimeLimit=00:30:00 TimeMin=N/A 
  SubmitTime=2022-10-11T17:02:58 EligibleTime=2022-10-11T17:02:58 
  AccrueTime=Unknown 
  StartTime=2022-10-11T17:02:58 EndTime=2022-10-11T17:32:58 Deadline=N/A 
  SuspendTime=None SecsPreSuspend=0 LastSchedEval=2022-10-11T17:02:58 Scheduler=Main 
  Partition=FHPC AllocNode:Sid=login02:168052 
  ReqNodeList=(null) ExcNodeList=(null) 
  NodeList=epyc[006-007] 
  BatchHost=epyc006 
  NumNodes=2 NumCPUs=4 NumTasks=4 CPUs/Task=1 ReqB:S:C:T=0:0:*:* 
  TRES=cpu=4,mem=4G,node=2,billing=4 
  Socks/Node=* NtasksPerN:B:S:C=2:0:*:* CoreSpec=* 
  MinCPUsNode=2 MinMemoryCPU=1G MinTmpDiskNode=0 
  Features=(null) DelayBoot=00:00:00 
  OverSubscribe=OK Contiguous=0 Licenses=(null) Network=(null) 
  Command=(null) 
  WorkDir=/large/___HOME___/area/ntosato 
  Power=
```
  

### `scancel`

In order to cancel *PENDING* job in a que or *RUNNING* jobs,  is necessary the command `scancel <jobid>`.

The `jobid` is the one specified from the `squeue`
**Example:**

```
[ntosato@login02 ~]$ squeue 
   JOBID PARTITION     NAME     USER      TIME  NODES  CPUS MIN_MEM     NODELIST(REASON)    STATE
     196      FHPC interact  ntosato      0:02      1     1      1G              epyc006  RUNNING
[ntosato@login02 ~]$ scancel 196
[ntosato@login02 ~]$ salloc: Job allocation 196 has been revoked.

```

Another useful example: `scancel --state=PENDING --user=<my_user>`

### Enviroments variables

TBD

### References

[1]  - <a href=https://support.nesi.org.nz/hc/en-gb/articles/360001316356-Slurm-Interactive-Sessions> More on `salloc` vs `srun` interactive shell </a>

[2]  - <a href=https://slurm.schedmd.com/documentation.html> Official SLURM docs </a>


## Run Serial Code

```
$ gcc pi.c -o serial_pi
$ ./serial_pi 1000000
```

## Run Parallel code

### Interactive mode

Load mpi modules
```
$ module load openmpi
```
Compile in parallel:
```
$ mpicc mpi_pi.c -o parallel_pi
```

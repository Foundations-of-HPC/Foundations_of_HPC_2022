## Lab notes
## Commands

$ sinfo -N --format="%.15N %.6D %.10P %.11T %.4c %.10z %.8m %.10e %.9O %.15C" 
    - we can see nodes, their state, the number of CPUs (= cores in SLURM jargon maybe = S*C)
    - S = socket
    - C = #cpu/cores
    - t = #threads
    - memory (MB) (RAM) -> the OS is already using some of this space (look at the idle nodes)
    - CPU_LOAD -> how much the CPU is loaded [0,1]
    - CPUS(Active/Idle/?/?)
    
$ squeue 
$ salloc -n 45 -p FHPC
    - submit jobs
    - n #CPUS I need 
    - p -> partition (for now there is no default so specify it)
    - -N -> how many nodes I want?
$ srun 
    - runs the command on the cores that I just reserved with salloc -> we're jumping on the computational node
    
$ bin/hostname
$ srun /bin/hostname

$ srun --pty /bin/bash\
    - I'm executing the bash that is on the node (I'm moving to the computational node!)
    - --pty is key here
    - I should view the same files that I see with the login node because there is a shared file system
    - when do we have to go on epyc nodes? We can go there to explore the node, to make computations in a uninterrupted way(?) and many other reasons. 
    - If we have reserved more than one node, on which one do we log in (using -N option in the salloc command)
    
Obs: in general there are other shells, not just bash. Now, on Linux, we all use bash
    
Obs:    -n --> specify #cpus I want (no matter where, could be in different nodes) (1 hostname is 1 node)
        -N --> specify #nodes
        --tasks-per-nodes --> how many tasks per node
        

$ srun /bin/hostname | uniq
$ srun /bin/hostname | uniq | wc

If we have a memory intensive application to run, that requires 2 node, we should choose 2 nodes in 2 different sockets and same node. This is supposing the information they access is in the two different RAMs.   - (In von Neumann architecture RAM is associated with a CPU) 
        - (shared memory within the node and distributed memory across nodes. For us different nodes means different computers).
But if the nodes assess the same memory space maybe the best idea is to have 2 nodes in the same socket.
Obs: SLURM jargon: CPU = core


$ sbatch file.job
    - submits job 









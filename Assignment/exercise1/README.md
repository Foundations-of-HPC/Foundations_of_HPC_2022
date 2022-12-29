
# Exercise 1 - Parallel Programming

### Foundations of HPC @ UniTS-DSSC 2022

**Assignment for the _Foundations of High Performance Computing course at “Data Science and Scientific Computing”, University of Trieste, 2022-2023**

_Stefano Cozzini_   `stefano.cozzini at areasciencepark.it`
_Luca Tornatore_   `luca.tornatore at inaf.it`

**Due time: 1 week before taking the oral exam**

------

## Game of Life

You’ll be prompted to implement a parallel version of a variant of the famous Conway’s “_game of life_” ( references: [1](https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life), [2](https://conwaylife.com/) ), that is cellular automaton which plays autonomously on a infinite 2D grid.
The game itself has several exciting properties and pertains to a very interesting field, but we leave to the readers going deeper into it at will.

### Addition to the v1.0 of the text

You are required to provide a scalability study:

- *OpenMP scalability*: fix the number of MPI tasks to 1 per socket, and report the behaviour of the code when you increase the number of threads per task from 1 up to the number of cores present on the socket;

- *Strong MPI scalability*: given a fixed size (you may opt for several increasing sizes ) show the run-time behaviour when you increase the number of MPI tasks (use as many nodes as possible, depending on the machine you run on);

- *Weak MPI scalability*: given an initial size, show to run-time behaviour when you scale up from 1 socket (saturated with OpenMP threads) up to as many sockets you can keeping fixed the workload per MPI task. 

FILES:
- `Assignment_exercise1.pdf` : the complete text of the exercise
- `read_write_pgm_image.c` : and example showiung how to read and write a pgm image file
- `get_args.c` : an example showing how to deal with command-line arguments

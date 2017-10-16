# MPI: Message Passing Interface

MPI es un estandar de paso de mensajes el cual define la sintaxis y semantica utilizada por una biblioteca la cual se enfoca en aprovechar los multiples nucleos de un procesador para ejecutar un programa en paralelo.

### Utilizacion
MPI se puede utilizar de varias formas, entre las cuales la basica es diseñar un programa el cual pueda ser ejecutado en paralelo, y ejecutar este programa utilizando MPI

#### Estructura de un programa [main.c]:
```
#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    // Initialize the MPI environment
    MPI_Init(NULL, NULL);

    // Get the number of processes
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // Get the rank of the process
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    // Get the name of the processor
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;
    MPI_Get_processor_name(processor_name, &name_len);

    // Print off a hello world message
    printf("Hello world from processor %s, rank %d"
           " out of %d processors\n",
           processor_name, world_rank, world_size);

    // Finalize the MPI environment.
    MPI_Finalize();
}
```
#### Ejecutar:
```
>>> cat host_file
cetus1
cetus2
cetus3
cetus4

mpicc -o ejecutable main.c
mpirun -n 4 -f host_file ./ejecutable
    //-n: numero de procesos
    //-f: archivo con nombre de los host

Hello world from processor cetus2, rank 1 out of 4 processors
Hello world from processor cetus1, rank 0 out of 4 processors
Hello world from processor cetus4, rank 3 out of 4 processors
Hello world from processor cetus3, rank 2 out of 4 processors
```

### Bibliografia

A Comprehensive MPI Tutorial Resource · MPI Tutorial. (2017). Mpitutorial.com. Retrieved 16 October 2017, from http://mpitutorial.com/

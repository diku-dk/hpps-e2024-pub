![header](header.png)

# Concurrency and Parallelism

## Reading

* Thursday

  - HPPS - OpenMP - Parallel Partionining
  - BOH 12.4 - 12.7

## Exercises: OpenMP thread ID hello world

We're going to create a relatively simple program that should act as an 
introduction to the use of the openmp thread id functions.

The purpose of this program is to run numerous openmp threads, collect the IDs 
of those threads and only once they've all completed, print out those IDs.

Your program should start by creating an array in which to store all the IDs as 
ints. Initially, this array will have garbage data within it, e.g. it is not 
initialised.

Once you have created the ID array, you should start a number of OpenMP 
threads. Initially you may wish to hard code the number of threads started but 
eventually it would be nice to have openMP itself dynamically create an 
appropriate amount based on whatever inputs your give it or environment 
variables you set.

Each openmp thread should dynamically determine its own ID, and write it to the 
array of IDs. it is up to you to determine that no races or deadlocks occur.

Once all threads have completed (e.g. written their ID), the list of IDs should 
be printed. You should be satisfied that each ID appears only once, that every 
index in your array has been filled, and that the shown IDs accurately reflect 
the thread that have run.


## Chunk Assignment 5

You've hopefully completed A5. If not you definitely should as its good 
practice for the exam (hint hint). One technique we didn't ask you to implement 
was any sort of chunking of the input data. Can you use the techniques shown in 
today's lecture, or in the HPPS notes under section 10.5 to separate the work 
in the nbody problem into chunks? You should ensure that no two threads are 
doing the same work, and that all bits of work get done regardless of how your 
chunks are split.

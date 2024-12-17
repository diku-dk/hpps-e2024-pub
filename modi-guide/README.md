# Running a program on MODI using Slurm

When using a large shared machine you cannot just log in and run your code. To ensure the that all users get their share, and to ensure that each execution is independent, you can only submit jobs into the shared queue. For most installations, and MODI in particular, this means using [Slurm](https://slurm.schedmd.com/overview.html).

Note that this guide is written for assignment 5. You will need to adapt some commands for other use cases.

## Getting access to MODI

Before you can access MODI, you need to [set up an account on ERDA](https://erda.ku.dk), using your regular UCPH login.

After you have logged in, you need to start the Jupyter service:
![Starting Jupyter on ERDA](screen-01.png "Starting Jupyter on ERDA")

Then choose the MODI tab and click "Start MODI":
![Enter MODI service](screen-02.png "Enter MODI service")

On the MODI Jupyter service page click "Start my server":
![Starting Jupyter Service](screen-03.png "Starting MODI service on ERDA")

Then make sure you have HPC Notebook (should be default) and click "Spawn":
![Spawn MODI instance](screen-04.png "Spawn MODI instance on ERDA")

After a small waiting period, you will get a Jupyter start page, and have access to the MODI front-end server.

For most systems, and MODI in particular, the front-end server is a trimmed down versions of the real servers. The machine has build tools matching the target servers, allowing you to compile and run small tests directly on the frontend.

## Upload your files

Before starting the upload, make sure you have tested your application as much as possible on your own machine. It is possible to debug via a queue based system, but it is a slow and unplesant process.

As we will need to run our binary on the execution nodes, we need to upload into the `modi_mount` directory. This is required because the execution nodes do not get your credentials, and cannot access any of the files stored on ERDA. You can mount ERDA temporarily on MODI, but that is not required for HPPS.
![Entering modi_mount](screen-05.png "Entering the modi_mount directory")

After you have selected the `modi_mount` directory, you can start uploading the files. In the example here, I have uploaded everything, including the `Makefile`. 
![Uploading to MODI](screen-06.png "Uploading files to MODI")

## Compiling on MODI

All tools are installed and optimized for running on MODI, including `Make`, `GCC` and `OpenMP`. If you are using the `Makefile` from the source code handout, you can just compile, but make sure you disable debugging. The `DEBUG` macro can be used to toggle this, so you could run the following:

```bash
> cd ~/modi_mount
> make clean
rm -f genparticles particles2text timestamps2text cmpparticles nbody nbody-bh *.o
> DEBUG=-O1 make
cc -c genparticles.c -Wall -Wextra -pedantic -lm -fopenmp -O3
cc -c util.c -Wall -Wextra -pedantic -lm -fopenmp -O3
cc -o genparticles genparticles.o util.o -Wall -Wextra -pedantic -lm -fopenmp -O3
cc -c particles2text.c -Wall -Wextra -pedantic -lm -fopenmp -O3
cc -o particles2text particles2text.o util.o -Wall -Wextra -pedantic -lm -fopenmp -O3
cc -c timestamps2text.c -Wall -Wextra -pedantic -lm -fopenmp -O3
cc -o timestamps2text timestamps2text.o util.o -Wall -Wextra -pedantic -lm -fopenmp -O3
cc -c cmpparticles.c -Wall -Wextra -pedantic -lm -fopenmp -O3
cc -o cmpparticles cmpparticles.o util.o -Wall -Wextra -pedantic -lm -fopenmp -O3
cc -c nbody.c -Wall -Wextra -pedantic -lm -fopenmp -O3
cc -o nbody nbody.o util.o -Wall -Wextra -pedantic -lm -fopenmp -O3
cc -c nbody-bh.c -Wall -Wextra -pedantic -lm -fopenmp -O3
cc -o nbody-bh nbody-bh.o util.o -Wall -Wextra -pedantic -lm -fopenmp -O3
rm util.o timestamps2text.o genparticles.o nbody.o nbody-bh.o cmpparticles.o particles2text.o
```

**Note:** Do not use the binary from your own machine. It may or may not work, but it is likely not optimized for the CPU used on MODI.

After compilation, verify that the binary works with something like this:
```bash
> OMP_NUM_THREADS=1 ./genparticles 10 PARTICLES.txt
Generating 10 particles...
Writing particles to PARTICLES.txt...
```

## Running a single job

With a compiled binary, you are now ready to submit a job. However, slurm works best with scripts, so we need to write a small wrapper that emits some results we will need later. We have provided two files for this purpse `run_nbody.sh` and `run_nbody_bh.sh`. Depending on what you need to run you make wish to make your own such as:
```bash
#!/bin/bash
export OMP_NUM_THREADS=${1}
echo "Running on ${2} with ${OMP_NUM_THREADS} threads to ${3} and ${4}"
time ./nbody ${2} ${3} ${4}
```

This will serve as a script for running the executable where we can change the parameters as we want. To submit the very first job, we can run the `sbatch` command:
```bash
> sbatch --exclusive run_nbody.sh 1 PARTICLES.txt SBATCH_PARTICLES.txt SBATCH_WARNINGS.txt
Submitted batch job 29249
```

You will then get job-id that you can use with `squeue -j 29249` to see the status. It will also create an empty file with the name `slurm-29249.out`. Once the job starts running, it will write output to this file. You can monitor the file with `tail -f slurm-29249.out`, which will interactively update the output file when the job is running (press CTRL+C to stop watching the file; this will not cancel the job).

You might want to try running a single job with different workloads sizes and threads, to see that your speedups work, and to pick a good size workload for multiple threads.

## Running multiple tasks

When you have tested a single task and confirmed that it works as expected, it is time to measure the performance of your solution. For this we want to run the same program, same workload, but with different thread counts. We have already created a helper script to launch a single job, and we can use that to submit multiple jobs, with a submission script like this (We have not provided this, its up to you to determine how best to test your implementation):

```bash
#!/bin/bash

for n in {1..64}; do
  sbatch --exclusive run_nbody.sh ${n} PARTICLES.txt SBATCH_PARTICLES.txt SBATCH_WARNINGS.txt
done
```

This script will invoke `sbatch` 64 times, varying the 1st parameter, which the `work.sh` script uses for the number of threads. When you run the script, you will see a list of job-ids. You can then use `squeue` to see if they are finished. Once they are finished, you can pick out the results from the files and put them into your favorite graph creation format. You will notice that `run_nbody.sh` writes the input parameters, which will help you when you have 64+ output files and try to figure out which goes where.

## Running long-running tasks

If your experiment with larger workloads, you can experience that the jobs are killed after 15 minutes. This is caused by the default queue on MODI allowing only short tasks. To see what queues are available, ukse the `sinfo` command:

```bash
> sinfo

PARTITION   AVAIL  TIMELIMIT  NODES  STATE NODELIST
modi_devel*    up      15:00      1    mix n001
modi_devel*    up      15:00      7  alloc n[000,002-007]
modi_short     up 2-00:00:00      1    mix n001
modi_short     up 2-00:00:00      7  alloc n[000,002-007]
modi_long      up 7-00:00:00      1    mix n001
modi_long      up 7-00:00:00      7  alloc n[000,002-007]
modi_max       up 31-00:00:0      1    mix n001
modi_max       up 31-00:00:0      7  alloc n[000,002-007]
```

You can see that `modi_short` allows tasks to run up to two hours. To run on another queue (aka partition) with Slurm, use the `--partition=` argument, like so:
```bash
> sbatch --partition=modi_short --exclusive run_nbody.sh 1 PARTICLES.txt SBATCH_PARTICLES.txt SBATCH_WARNINGS.txt
Submitted batch job 29249
```

You might be tempted to use the larger nodes, but be aware that its usually considered rude to ask for more resources than you need, and that these nodes often have a very long queue anyway as they frequently run jobs for several days at a time.


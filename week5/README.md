![header](header.png)

# Concurrency and Parallelism

## Reading

* Tuesday

  - HPPS - OpenMP
  - HPPS - Parallel Speedup and Scalability

* Thursday

  - HPPS - Loop Dependency Analysis
  - HPPS - Loop Transformations

## Videos

* [Concurrent programming](https://sid.erda.dk/share_redirect/AjdMSpVIjr/videos/5-l-1/concurrent-programming.mp4) (less important)

* [OpenMP](https://sid.erda.dk/share_redirect/AjdMSpVIjr/videos/5-l-2/openmp.mp4)

* [Scaling](https://sid.erda.dk/share_redirect/AjdMSpVIjr/videos/5-l-2/scaling.mp4)

* [Loop Dependency Analysis](https://sid.erda.dk/share_redirect/AjdMSpVIjr/videos/6-l-1/dependencies.mp4)

* [Eliminating loop dependencies](https://sid.erda.dk/share_redirect/AjdMSpVIjr/videos/6-l-1/eliminating-dependencies.mp4)

* [Loop interchange](https://sid.erda.dk/share_redirect/AjdMSpVIjr/videos/6-l-1/interchange.mp4)


## Exercises: parallel programming with OpenMP

One of the nice aspects of being a teacher is that if you find a hobby
you enjoy, you can force your students to enjoy it along with you.
One of my hobbies is computer graphics.  For this lab, you will be
investigating the parallelisation of a [Mandelbrot fractal
generator](https://en.wikipedia.org/wiki/Mandelbrot_set) with OpenMP.
You don't have to understand the mathematics behind the rendering -
all you need to concern yourself with is that there is a program with
a loop that produces a bunch of pixels, and that program needs to run
faster.  It is not at all uncommon to be asked to optimise a program
where you do not fully understand what it computes.

[If you are using macOS, make sure to read this.](../macos-openmp.md)

## Optimising [mandelbrot.c](openmp-handout/mandelbrot.c)

You are given a fully functional *sequential* program that can render
pretty pictures to an image file in the PPM format (which most
browsers and image viewers should be able to handle).  The program
takes at least 5 options:

1. The output file name.

2. The width of the image.

3. The height of the image.

4. The iteration depth limit (see below).

5. The escape radius (see below).

Optionally we can also provide an additional three options:

6. The *x* coordinate of the part of the complex number field we wish to
   visualise.

7. The *y* coordinate of the part of the complex number field we wish to
   visualise.

8. The width of the part of the complex number field we wish to
   visualise.

The [Makefile](openmp-handout/Makefile) contains rules for running the program
with interesting inputs.  For example, `make preset0.ppm` produces
[this image](preset0.png) (after manual conversion to PNG), and `make
preset1.ppm` produces [this image](preset1.png).

The core of the Mandelbrot program is a two-deep loop nest that
computes pixel values.  It does this by computing the complex number
corresponding to a pixel, and then calling the function `divergence`,
which uses a loop to count how many "orbits" the complex number makes
before its distance exceeds the given "radius", with the iteration
count being limited.  Those pixels that hit the iteration limit are
coloured black, and the others are passed through a convoluted
colouring function (`escape_to_colour`), the details of which are not
important to us.  What's important is that there is a *lot* of work to
be done, and not all pixels take the same amount of time to compute.

### Instrumenting the program

There are two time-consuming parts to this program: rendering the
image as a two-dimensional `int` array (`pixels` in the `main`
function), and writing it to a file (the `ppm_to_file` function).
Using the `seconds` function defined in the `timing.h` header file,
add instrumentation so that when the program runs, it prints how long
these two parts take. E.g:

```
$ ./mandelbrot preset0.ppm 1000 1000 100 16 -0.7 0.0 3.067
Rendering time: 0.143732s
IO time: 0.057136s
```

* Do you think the rendering can be parallelised?
* Do you think the IO can be parallelised?

### Estimating speedup with Amdahl's Law

For some fixed input workload, use the measurements provided by the
instrumentation you added to calculate the fraction of the work that
can be parallelised (*p*). Use Amdahl's Law to estimate the potential
speedup of the *entire program* (not just the parallelisable part!)
when using more threads (say, up to 16 or however many your machine
has). Try both on a reasonably large workload (e.g. the `preset0.ppm`
command above) but also on a smaller one, for example by shrinking the
image size to *50 x 50*.

### Estimating speedup with Gustafson's Law

Now repeat the analysis, but use Gustafson's Law.

### Parallelisation with OpenMP

Hopefully your analysis suggests that there is value in parallelising
the program.  Identify the loop(s) you wish to speed up, and use the
information in the OpenMP lecture and course notes to parallelise them.

#### Hints

* Use the `#pragma omp parallel for` directive to parallelise a loop.

* Not all loops are parallelisable.  Only loops with independent
  iterations (and known iteration count) are parallelisable with
  OpenMP.

* Use the `collapse(N)` clause to apply a parallelisation directive to
  *N* nested loops.

* Maybe there is value in using a different `schedule` clause?

### Measuring the parallel speedup

Now that you have a parallel program, it is time to see how its
performance *scales* for various workloads and thread counts.  When
playing around with workloads, the size of the image (*width ✕
height*) determines the amount of parallel work, and the iteration
depth limit (fourth option) *roughly* determines the amount of
sequential work (although you might also need to increase the
*radius*, the fifth option, if you want it to have much effect).

We can set the `OMP_NUM_THREADS` prior to running `mandelbrot` to
control the number of threads used.  For example:

```
$ OMP_NUM_THREADS=1 ./mandelbrot preset0.ppm 1000 1000 100 16 -0.7 0.0 3.067
Rendering time: 0.160205s
IO time: 0.061860s
$ OMP_NUM_THREADS=2 ./mandelbrot preset0.ppm 1000 1000 100 16 -0.7 0.0 3.067
Rendering time: 0.080808s
IO time: 0.060964s
```

Using two threads gave a speedup of 2!  That's as good as can be hoped
for.  Try it with different thread counts on your machine and see when
it starts to drop off.  Try also with different workloads, such as a
small image with a high depth limit.

### Relate the measurements to your estimates

* How do your observations line up with the estimates made by Amdahl
  and Gustafson?

* Can you come up with a workload that exhibits *strong scaling*?  Can
  you come up with one that doesn't?

* Does your program exhibit *weak scaling*?

Remember that "strong" and "weak" scaling are not just slang for
"good" or "bad", but specifics terms of art covered in the lecture
material.

## Parallelise KNN

Remember the KNN implementation you made for assignment 2?  Try to see
if there are any opportunities for parallel executions.  Then use
OpenMP to parallelise the most promising ones, and see whether it
makes it any faster.

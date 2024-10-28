# Syllabus and Material for High Performance Programming and Systems (HPPS)

Welcome to the course website for HPPS! All material and general
information will be provided here. Announcements, assignment handin,
and the discussion forum remains on Absalon. While this website is a
Git repository, you are not required or expected to use Git to
interact with it, but feel free to do so if convenient for you.

## Basics

HPPS takes place in block 2.  There are three kinds of in-person
teaching activities: **lectures**, **exercises** and the **study
café**.  All take place physically on campus.

**Lectures** are mostly conventional and serve as warmup for the
exercises.

**Exercises** are conducted in four smaller classes, each conducted by
a teaching assistant (TA).  An exercise session involves working on
problems that will help you learn the curriculum, and in many cases
directly train you in the skills you will need for the mandatory
assignments.

**Study cafés** are places where you can get assistance with the
mandatory assignments.

**This course website** is used for handing out material.

**Discord** can be used for asking questions. ([Invite
link](https://discord.gg/CDDZCfx8zz)). You are not *required* or
*expected* to use Discord. We continue to monitor the Absalon
discussion forum, and course announcements are posted solely on
Absalon.

**Absalon** is used for handing in assignments and for course
announcements.

## Textbook

The textbooks are as follows:

* **JG:** [Modern C](https://modernc.gforge.inria.fr/) ([CC-licensed PDF](https://hal.inria.fr/hal-02383654/document))

* **HPPS:** [HPPS course notes](https://github.com/diku-dk/hpps-notes)
  [PDF](https://github.com/diku-dk/hpps-notes/releases/download/latest/notes.pdf) -
  **these will be updated as the course progresses, so make sure to
  regularly check that you have the newest version**

## Schedule

Ignore the official class grouping made by KU. Go to whichever class
you prefer. Note that Class 4 has the largest and nicest room, and
will have two TAs available.

### Tuesday

* **Lecture:** 10:00-12:00 (Aud 03, HCØ).

### Thursday

* **Exercises:** 10:00-12:00

* **Lecture:** 13:00-15:00 (Aud 03, HCØ)

* **Exercises:** 15:00-17:00

Exercises are held in the following four classrooms.

* **Class 1:** *10:00-12:00*, 1-0-10 (DIKU), *15:00-17:00*: 1-0-10 (DIKU). TA: TBD

* **Class 2:** *10:00-12:00*, 1-0-14 (DIKU), *15:00-17:00*: 1-0-14 (DIKU). TA: TBD

* **Class 3:** *10:00-12:00*, Auditorium Syd (NEXS), *15:00-17:00*: Auditorium Syd (NEXS). TA: TBD.

* **Class 4:** *10:00-12:00*, 4-0-10 (Biocenter), *15:00-17:00*: 4-0-10 (Biocenter). TA: TBD.

### Friday

* **Assignment café** 13:00-15:00 (Lille UP-1).

## Assignments

There are 5 assignment in total during the course with deadlines every
week. They overlap slightly to allow for more flexibility in your
scheduling, but think of them as weekly assignments. The assignments
are handed out at the beginning of the week. They are handed out on
this webpage, and handed in via Absalon.

The assignments will be graded with points from 0 to 4. It is not
possible to re-hand-in any of the assignments.

Assignments are made to be solved in groups of preferably three
students, but groups of two active students will also do. We strongly
encourage you not to work alone. Groups cannot be larger than three
students. Each group must make their own solutions and cannot share
implementations and report with other. You may however discuss
material and ideas.

### General assignment rules

The following rules apply to all assignments. They are intended to
ease our correction process, and in particular to allow automated
testing. Consider the assignments to be a specification or API that
you are asked to implement.

1. Do not modify the types of any definitions in the handout, except
   when the assignment text explicitly instructs you to do so.

2. Do not rename or remove any definitions that are present in the
   handout, except when the assignment text explicitly instructs you
   to do so.

3. Do not remove anything from header files.

4. Do not rename files or otherwise modify the file tree. (You may add
   new files if you wish, although it is rarely necessary.)

5. Your code should compile without warnings. (Do not achieve this by
   disabling warnings.)

6. When handing in, you must hand in a complete workable program
   (including unmodified files from the handout).

7. When handing in, do not include temporary build files (such as
   `.o`), editor backup files, or various other computer detritus. Run
   e.g. the `tree` command and read the file listing and ponder for
   each file whether it is something it makes sense to hand to your
   TA. At a *minimum*, run `make clean` before handing in.

8. Your zip file, should contain a single top-level folder with an
   appropriate name (e.g. `handin`).

Violation of these rules will result in points deductions. If you
violate these rules at the exam, it will negatively influence your
grade.

## Study café

Every Friday in from 13:00-15:00 you can attend the assignment café to
get help with the assignments.

## Languages

All written material will be in **English**.

Most oral teaching will be in **Danish** or **English**, depending on
the specific teacher or TA.

You will mainly be programming in **C** and **Python**.

## Software

You will be using a Unix command line and Unix tools for much of the
course.  [See the Unix software guide.](unix.md)

See also [this guide on the GDB
debugger](http://beej.us/guide/bggdb/), which is a very useful tool
for debugging C programs.

If you prefer an IDE, see the [VS Code installation and setup
guide](vscode.md).

## People

The teachers are

* Troels Henriksen <athas@sigkill.dk>
* David Gray Marchant <david.marchant@di.ku.dk>

The TAs are:

* Rasmus Pallisgaard <rpa@di.ku.dk>
* Alexander Juel Vind <avji@di.ku.dk>
* Michael Angell Ghandforoush <mgh@di.ku.dk>
* Carl August Gjerris Hartmann <cgha@di.ku.dk>
* Carl Christian Ottesen <ccot@di.ku.dk>

## Exam

### Format

The exam will be a take-home exam that will be very similar in form to
the mandatory assignments. See [examples of old exams
here](old-exams/). It has an estimated workload of 20 hours. It will
involve practical programming and performance analysis of programs.

### Qualification

To qualify for the exam you are required to achieve at least 50% of
the total number of points in the five assignments (that is, 10 points
at minimum). You also need to get *at least* one point in each of the
first five assignment.

### Date

**Tentative:** The exam will likely be held in week 3, Monday 9:00 to
Wednesday 12:00.

## Course plan

Each week has an associated subdirectory here on GitHub that lists
expected reading and relevant exercises.

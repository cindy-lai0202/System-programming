HW7-1 from the exercise 22-3

Section 22.10 stated that accepting signals using sigwaitinfo() is faster than the use
of a signal handler plus sigsuspend(). The program signals/sig_speed_sigsuspend.c,
supplied in the source code distribution for this book, uses sigsuspend() to
alternately send signals back and forward between a parent and a child process.
Time the operation of this program to exchange one million signals between the
two processes. (The number of signals to exchange is provided as a command-line
argument to the program.) Create a modified version of the program that instead
uses sigwaitinfo(), and time that version. What is the speed difference between the
two programs?



HW7-2 from the exercise 22-4


part1

1.Write, compile, and run a program named hostinfo that prints out system information in the following format.

![image](https://user-images.githubusercontent.com/72913466/209782013-0ec4c1f6-0962-4fdd-be62-cac54e0b504c.png)

2.Write, compile, and run a program named mydate that prints out the day and time in the following format.

![image](https://user-images.githubusercontent.com/72913466/209782083-91ac9fd8-c06c-4089-9564-b2bfe3c1fadf.png)

3.Write a program called printdir that prints the current directory. Determine what size buffer to pass to getcwd() for dynamic allocation.(Do not use pwd().)

![image](https://user-images.githubusercontent.com/72913466/209782139-543d5207-2a19-4974-a31e-195d96fde850.png)

4.Write a program called mycat that is a simple version of the program cat. The program takes exactly one file name as argument; 
you should open it for reading and display its contents to the screen. Check that there is exactly one argument (argc == 2) and if not, 
display the usage message ”Usage: mycat filename”.

![image](https://user-images.githubusercontent.com/72913466/209782259-50351695-d36c-44e0-aa89-c223ebc215ba.png)

5.Write pipe_ls to practice using pipe() and dup(). Have your process start ls (using fork() and exec()) but read the output from ls over a pipe. 
The ls program writes output on descriptor 1, so some work has to be done to get the pipe connected. Write what you read on the pipe to stdout.

![image](https://user-images.githubusercontent.com/72913466/209782355-1144277e-d03f-4e4c-83f3-3c79be3fbdc2.png)

part1 

![image](https://user-images.githubusercontent.com/72913466/209776386-9a9dd2d0-564b-492c-a25d-aec61d737245.png)

part2
echo print all strings 
echo -n N: print the specified string
quit example: exit, quit, logout and bye terminate the program

![image](https://user-images.githubusercontent.com/72913466/209776776-6717f65b-96d5-4dec-bf48-b785b6f968fd.png)

part3
Edit the run_command.c file so a child process is created to run the command, and the parent waits for the child process to terminate. Check for builtin commands first, create a new process only for commands which are not built in. Use the parser from pervious labs to create from the command line the argv array passed to the child.

![image](https://user-images.githubusercontent.com/72913466/209776855-03edbedd-af61-4f08-a0ff-97b47886a763.png)

part4
Edit the is_background.c file to detect an “&”. Alter the run_command.c file so that if a task is running in the background, the parent does not wait. Do not worry about background processes becoming zombies at this point; this will be addressed later.

![image](https://user-images.githubusercontent.com/72913466/209777111-2fe629ee-c0c8-47f3-8fff-aba5560e2174.png)



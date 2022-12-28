#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <pwd.h>
#include <unistd.h>
#include <sys/stat.h>
#include <ctype.h>

int main(int argc ,char **argv){
	DIR *dir,*dir2;
	FILE *file;
	struct dirent *directory,*directory2;
	char dirname[256];
	char linebuf[256];
	char procname[256];
	char checkpath[256];
	char pid[32];
	char linbuf[1024];
	char *key;
	char *value;
	int length=0;
 
	if ((dir = opendir("/proc")) == NULL) {
		perror("Unabled to open /proc");
		return 1;
	}
	//掃描/proc/PID
	while ((directory= readdir(dir)) != NULL) {
		if (directory->d_type == DT_DIR) {
			strcpy(&dirname[0], "/proc");
			strcat(&dirname[0], "/");
			strcat(&dirname[0], directory->d_name);
			strcpy(checkpath,&dirname[0]);
			strcat(checkpath, "/fd");
			if ((dir2 = opendir(checkpath)) == NULL) {
				continue;
			}
			//scan /proc/PID/fd
			while ((directory2= readdir(dir2)) != NULL) {
				if (directory2->d_type == DT_LNK) {
					strcat(checkpath, "/");
					strcat(checkpath,directory2->d_name);
					length = readlink(checkpath, linbuf, sizeof(linbuf) - 1);
					if (length > 0) {
						linbuf[length] = '\0';
						if (strcmp(linbuf, argv[1]) == 0) {
							strcat(&dirname[0],"/status");
							file = fopen(&dirname[0], "r");
							if (file == NULL) {
								continue; 
							}
							while (fgets(linebuf, sizeof(linebuf), file) != NULL) {
								key = strtok(linebuf, ":");
								value = strtok(NULL, ":");
								if (key != NULL && value != NULL) {
									//clean space
									char * p = key;
									int l = strlen(p);
									while(isspace(p[l - 1])) {
										l--;
										p[l] = 0;
									}
									while(isspace(* p)) {
										p++;
										l--;
									}
									memmove(key, p, l + 1);
      
									char * q = value;
									int l1 = strlen(q);
									while(isspace(q[l1 - 1])) {
										l1--;
										q[l1] = 0;
									}
									while(isspace(* q)) {
										q++;
										l1--;
									}
									memmove(value, q, l1 + 1);
						
									if (strcmp(key, "Pid") == 0) {
										strcpy(pid, value);
									} else if (strcmp(key, "Name") == 0) {
										strcpy(procname, value);
									}
								}
							}
							printf("%s (pid: %s)\n", procname, pid);
						}
					}
				}
				
			}
		}
	}

    return 0;
}

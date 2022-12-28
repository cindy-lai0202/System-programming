#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>
#include <ctype.h>

uid_t user_id_from_name(const char *name)
{
	struct passwd *pwd;
	uid_t u;
	char *endptr;
	if (name == NULL || *name == '\0') {
		return -1;
	}

	u = strtol(name, &endptr, 10);
	if (*endptr == '\0') {
		return u;
	}

	pwd = getpwnam(name);
	if (pwd == NULL) {
		return -1;
	}

	return pwd->pw_uid;
}

int main(int argc,char **argv)
{
	DIR * dir;
	struct dirent * directory;
	int i;
	int keepme;
    	char dirname[256];
    	char linebuf[256];
    	char uidstr[128];
	char procname[256];
	char pid[32];
    	FILE *file;
    	char *key;
	char *value;
	
	if (argc != 2) {
		perror("command error\n");
		return -1;
	}
	
	uid_t uid;
	uid=user_id_from_name(argv[1]);
	
	if (uid == (uid_t)-1) {
		perror("username not found\n");
		return -1;
	}

    	if ((dir = opendir("/proc")) == NULL) {
		perror("Unabled to open /proc\n");
		return 1;
	}
	
   	while((directory = readdir(dir)) != NULL)
    	{
		strcpy(&dirname[0], "/proc");
		strcat(&dirname[0], "/");
		strcat(&dirname[0], directory->d_name);
		strcat(&dirname[0], "/status");
		sprintf(uidstr, "%d", uid);
		file = fopen(&dirname[0], "r");
		if (file == NULL) 
			continue; 
		keepme=0;
		while (fgets(linebuf, sizeof(linebuf), file) != NULL) {
			key = strtok(linebuf, ":");
			value = strtok(NULL, ":");
			if (key != NULL && value != NULL) {
				char * p = key;
				char * q = value;
				int l = strlen(p);//not include "/0"
				while(isspace(p[l - 1])) {
					l--;
					p[l] = 0;
				}
					
				while(isspace(* p)) {
					p++; 
					l--;
				}
				memmove(key, p, l+1 );
			
				int l1 = strlen(q);
				while(isspace(q[l1 - 1])){
					l1--;
					q[l1] = 0;
					
					}
				while(isspace(* q)) {
					q++;
					l1--;}
				memmove(value, q, l1+1 );
						
				if ((strcmp(key, "Uid") == 0) && strstr(value, uidstr) != NULL) {
					keepme = 1;
				}
				if (strcmp(key, "Name") == 0) {
					strcpy(procname,value);
				}
				if (strcmp(key, "Pid") == 0) {
					strcpy(pid, value);
				}
			}
		}
		if (keepme) {
			printf("%s: %s\n", pid, procname);
		}
  }
					
    closedir(dir);
    return 0;
}

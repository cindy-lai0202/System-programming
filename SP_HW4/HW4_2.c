#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <pwd.h>
#include <ctype.h>

struct tree_node{
	char name[256];
	pid_t pid;
	pid_t ppid;
	struct tree_node *parent;
	struct tree_node *children[256];
	struct tree_node *next;
};
struct tree_node *tmp_node;

int create_add_node(char *procname,pid_t pid,pid_t ppid){
	struct tree_node *node;
	node=(struct tree_node*)malloc(sizeof(struct tree_node));
	if(node==NULL){
		perror("malloc error");
		return 1;
	}
	strcpy(node->name, procname);
	node->pid = pid;
	node->ppid = ppid;
	node->children[0] = NULL;
	node->parent = NULL;
	node->next = tmp_node;
	tmp_node=node;
	return 0;
}

int make_tree(){
	struct tree_node *node,*tnode;
	int i;
	for(node=tmp_node;node!=NULL;node=node->next){
		i=0;
		for(tnode=tmp_node;tnode!=NULL;tnode=tnode->next){
			if(tnode->pid==node->ppid){
				node->parent=tnode;
				while(tnode->children[i]!=NULL){
					i++;
				}
				i++;
				tnode->children[i-1]=node;
				tnode->children[i]=NULL;
		}
   }
}
free(node);
free(tnode);
}

int print_tree(struct tree_node * root, int level)
{
	int i;
	struct tree_node *node;
	for (i = 0; i < level; i++) {
		printf("  ");
	}
	printf("%s(pid:%d,ppid:%d)\n",root->name,root->pid,root->ppid);
	int j = 0;
	while ((node = root->children[j]) != NULL) {
		j++;
		print_tree(node, level + 1);
	}
	free(node);
	return 0;
}

int main(int argc ,char **argv){
	DIR *dir;
	FILE *file;
	struct dirent *directory;
	char dirname[256];
	char linebuf[256];
	char procname[256];
	char pid[32];
	char ppid[32];
	char *key;
	char *value;
 
	if ((dir = opendir("/proc")) == NULL) {
		perror("Unabled to open /proc");
		return 1;
	}
		while ((directory= readdir(dir)) != NULL) {
			if (directory->d_type == DT_DIR) {
				strcpy(&dirname[0], "/proc");
				strcat(&dirname[0], "/");
				strcat(&dirname[0], directory->d_name);
				strcat(&dirname[0], "/status");
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
						} else if (strcmp(key, "PPid") == 0) {
							strcpy(ppid, value);
						} else if (strcmp(key, "Name") == 0) {
							strcpy(procname, value);
						}
					
					}
				}
				create_add_node(procname,atoi(pid),atoi(ppid));//&proname[0]
			}
		}
	make_tree();
	//print tree
	struct tree_node *node;
	for (node = tmp_node; node != NULL; node = node->next) {
		if (node->parent == NULL) {
			print_tree(node, 0);
		}
	}
	free(node);
	free(tmp_node);
    return 0;
}


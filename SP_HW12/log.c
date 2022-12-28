#define _XOPEN_SOURCE 500
#include <limits.h>
#include <ftw.h>
#include <sys/inotify.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/limits.h>
#include "tlpi_hdr.h"

#define NOPENFD 100
#define BUFSIZE (10 * (sizeof (struct inotify_event) + NAME_MAX + 1))
#define MAX_SUBDIRS 128

struct stat_watched_subdir {
  int wd;
  char fpath[PATH_MAX];
};

int watchedDirsCount;
struct stat_watched_subdir *watchedDirs[MAX_SUBDIRS];
int watchedDirsCount = 0;
int inotifyFd;

static void addWatch(const char *path) {
  int wd;
  struct stat_watched_subdir *entry;

  if (watchedDirsCount >=MAX_SUBDIRS)
    errExit("The watched limit reached!");

  wd = inotify_add_watch(inotifyFd, path, IN_ALL_EVENTS);
  if (wd == -1)
    errExit("inotify_add_watch");

  entry = malloc(sizeof(struct stat_watched_subdir));
  if (entry == NULL)
    errExit("malloc");

  entry->wd = wd;
  strncpy(entry->fpath, path, PATH_MAX);
  watchedDirs[watchedDirsCount++] = entry;
  
}

static int createMonitor(const char *fpath, const struct stat *sb, int typeflag, __attribute__((unused)) struct FTW *ftwbuf) {
  /* file failed or dir can't be read*/
  if (typeflag == FTW_NS || typeflag == FTW_DNR)
    return 0;

  /* add a watch only to directories */
  if (S_ISDIR(sb->st_mode)) {
    addWatch(fpath);
  }
  return 0;
}

static void removeWatch(int wd) {
  int i;

  for (i = 0; i < watchedDirsCount; ++i) {
    if (watchedDirs[i]->wd == wd) {
      free(watchedDirs[i]);
      watchedDirs[i] = NULL;
 
      return;
    }
  }
}

char *pathWithoutDirname(int wd) {
  int i;

  for (i = 0; i < watchedDirsCount; ++i) {
    if (watchedDirs[i]->wd == wd)
      return watchedDirs[i]->fpath;
  }
  errExit("Could not find path prefix for wd %d\n", wd);
}

static void displayInotifyEvent(struct inotify_event *event) {

void eventprint(char *str){
  //event->len : Size of 'name' field
  if (event->len > 0) { 
    if (event->mask & IN_ISDIR) { 
      printf("Directory %s/%s %s\n", pathWithoutDirname(event->wd), event->name, str); 
    } else { 
      printf("File %s/%s %s\n", pathWithoutDirname(event->wd), event->name, str); 
    } 
  } else { 
    printf("Directory %s %s\n", pathWithoutDirname(event->wd), str); 
  } 
}

  if (event->cookie > 0)		
    eventprint("was renamed");
  if (event->mask & IN_CREATE)        
    eventprint("was created");
  if (event->mask & IN_DELETE)        
    eventprint("was deleted");
  if ((event->mask & IN_IGNORED & IN_DELETE))		
    removeWatch(event->wd);
    
  //addWatch of subdir
  if (event->mask & IN_ISDIR && event->mask & IN_CREATE) {
    char path[PATH_MAX];
    snprintf(path, PATH_MAX, "%s/%s", pathWithoutDirname(event->wd), event->name);
    addWatch(path);
    
  }
}

int main(int argc, char *argv[]) {
  ssize_t numRead;
  char *p, buf[BUFSIZE];
  struct inotify_event *event;

  if (argc != 2)
    errExit("Usage:./%s <dir>",argv[0]);

  inotifyFd = inotify_init();
  if (inotifyFd == -1) 
    errExit("inotify_init");

  /* traverse the directory tree and monitor all directories */
  if (nftw(argv[1], createMonitor, NOPENFD, FTW_PHYS) == -1)
    errExit("nftw");

  printf("Listening for events on %s\n", argv[1]);
 
  for (;;) {
    numRead = read(inotifyFd, buf, BUFSIZE);
    if (numRead == -1)
      errExit("read");

    for (p = buf; p < buf + numRead; p += sizeof(struct inotify_event) + event->len) {
      event = (struct inotify_event *) p;
      displayInotifyEvent(event);
    }
  }
 

  exit(0);
}

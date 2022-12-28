#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
int main(void)
{
   int i;
   for(i=0; i<2; i++){
      fork();
      //注意：下面的printf有“n”
      printf("ppid=%d, pid=%d, i=%d n \n", getppid(), getpid(), i);
   }
   sleep(10); //讓程序停留十秒，這樣我們可以用pstree檢視一下程序樹
   return 0;
}

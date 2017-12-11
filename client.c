#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <time.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <dirent.h>

#define KEY 2319

int main(){
  int sem = semget(KEY,1,0666);
  int sharedmem = shmget(KEY,sizeof(char *),0666);

  struct sembuf b[1];
  b[0].sem_op = -1;
  b[0].sem_num = 0;
  b[0].sem_flg = SEM_UNDO;
  semop(sem,b,1);
  int x = semctl(sem,0,GETVAL);

  int file = open("story.txt",O_RDONLY);
  int * lastlinesize = shmat(sharedmem,0,0);
  lseek(file,(-1* *lastlinesize),SEEK_END);
  char lastline[256];
  read(file,lastline,*lastlinesize);
  close(file);
  //printf("%d\n",x);
  printf("Last contribution: %s\n", lastline);

  
  char * newline = malloc(256);
  printf("New line: ");
  scanf(" %[^\t\n]s ",newline);


  file = open("story.txt",O_WRONLY | O_APPEND);
  strcat( newline, "\n" );
  int newlinesize = strlen(newline);
  write(file,newline,newlinesize);
  close(file);


  *lastlinesize = newlinesize;
  shmdt(lastlinesize);
 

  b[0].sem_op = 1;
  semop(sem,b,1);



  
  
  
  
  
  return 0;
}

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

  char * lastline = shmat(sharedmem,0,0);
  printf("Last contribution: %s\n",lastline);

  char newline[256];
  printf("New line: ");
  scanf("%s",newline);

  int file = open("story.txt",O_WRONLY | O_APPEND);
  write(file,newline,sizeof(newline));

  struct sembuf * buffer;
  buffer.sem_op = -1;
  buffer.sem_num = 0;
  buffer.sem_flags = SEM_UNDO;
  semop(sem,buffer,1);

  lastline = newline;

  buffer.sem_op = 1;
  semop(sem,buffer,1);
  
  
  
  
  
  
  return 0;
}

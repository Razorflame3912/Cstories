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

int main(int argc, char * argv[]){
  int sem;
  int sharedmem;
  int file;
  if(!strncmp(argv[1],"-c", 2)){
    sem = semget(KEY,1,0666 | IPC_CREAT | IPC_EXCL);
    if(sem == -1){
      printf("Semaphore already exists\n");
      return 0;
    }
    semctl(sem,0,SETVAL,1);
    printf("Semaphore created\n");
    
    sharedmem = shmget(KEY,sizeof(size_t),IPC_CREAT | IPC_EXCL | 0666);
    if(sharedmem == -1){
      printf("Shared memory already exists\n");
      return 0;
    }
    printf("Shared memory created\n");
    
    file = open("story.txt",O_CREAT|O_EXCL|O_TRUNC,0666);
    if(file == -1){
      printf("File already exists\n");
      return 0;
    } 
    printf("File created\n");
   
    semctl(sem,0,SETVAL,1);
    
  }
  else if(!strncmp(argv[1],"-v", 2)){
    file = open("story.txt",O_RDONLY);
    char output[1000];
    read(file,output,sizeof(output));
    printf("Story contents: %s\n",output);
    
  }
  else if(!strncmp(argv[1],"-r", 2)){
    file = open("story.txt",O_RDONLY);
    char output[1000];
    read(file,output,sizeof(output));
    printf("Story contents: %s\n",output);

    sharedmem = shmget(KEY,sizeof(char *),0666);
    shmctl(sharedmem,IPC_RMID,NULL);
    printf("Shared memory removed\n");



    sem = semget(KEY,1,0666);
    semctl(sem,0,IPC_RMID,0);
    printf("Semaphore removed\n");
  }
  else{
    printf("Unrecognized input.\n");
  }
    return 0;
}

#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include "mergeSort.c"
#include "scannerCSVsorter.h"
int  listdir(const char *name)
{
  DIR *dir;
  struct dirent *cursor;
  int children = 0;
  if (!(dir = opendir(name)))
  return;
  char cwd[PATH_MAX];
  while ((cursor = readdir(dir)) != NULL) {
    pid_t child;
    //printf("%s\n",cursor->d_name);
    if (cursor->d_type == DT_DIR) {
      char path[1024];
      if (strcmp(cursor->d_name, ".") == 0 || strcmp(cursor->d_name, "..") == 0)
        continue;
      //printf("DIR %s\n",cursor->d_name);
      child = fork();
      if(child == 0){
        strcpy(path,name);
        strcat(path,"/");
        strcat(path,cursor->d_name);
        printf("%d,", getpid());
        fflush(stdout);
        int subchildren = listdir(path);
        exit(subchildren);
        break;
      }
      else{
        int status;
        if ( waitpid(child, &status, 0) == -1 ) {
          perror("WAIT FAILED");
          return 0;
        }
        children += WEXITSTATUS(status)+1;
        //printf("%d\n",child);
        //printf("%s %d\n",cursor->d_name, child);
        continue;
      }
    }
    else {
      char *suffix = strrchr(cursor->d_name, '.');
      child=fork();
      if(child==0){
        printf("%d,", cursor->d_name,getpid());
        fflush(stdout);
        //TODO Check for valid FORMAT
        movie_data* head = parse_csv();
        //TODO SORT FUNCTION HERE
        exit(1);
      }
      else{
        children++;
        //printf("%s %d\n",cursor->d_name, getpid() );
        continue;
      }
    }
  }
  closedir(dir);
  return children;
}

int main(int argc, char *argv[]) {
  check_parameters(argc, argv);
  check_sort_by();
  get_directory_paths(argc, argv);
  printf("Initial PID: %d\nPIDS of all child processes: ", getpid());
  fflush(stdout);
  // printf("PIDS of all child processes: ");
  int end = listdir(".");
  pid_t wpid;
  int status = 0;
  int sum = 0;
  while ((wpid = wait(&status)) > 0)
  {
  }
  //fflush(stdout);
  printf("\nTotal number of processes: %d\n",end);
  return 1;
}

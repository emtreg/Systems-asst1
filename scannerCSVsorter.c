#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include "scannerCSVsorter.h"
#include "mergesort.c"
int writeFile(char* name, movie_data* head,char* sortingHead, char* destinationPath){
	char path[1024];
	strcpy(path,destinationPath);
	strcat(path,"/");
	strcat(path,name);
	int i = 0;
    	while(path[i] != '\0')
    	{
        	i++;

    	}
    	path[i-4] = '\0';
	strcat(path,"-sorted-");
	strcat(path,sortingHead);
	strcat(path,".csv");
	FILE *fp = fopen(name, "ab+");
	movie_data* cur = head;
	i=0;
	while(cur!=NULL){
		fprintf(fp,"%s\n",cur->raw_row);
		cur = cur->next;
		i++;
	}
	fclose(fp);
	return i;
}
int checkIfValid(char* path, char* sortColumn){
    if(strstr(path,"-sorted-")!=NULL)
	return -1;
    if(strcmp(strchr(path,'.'),".csv")!=0)
	return -2;
    FILE *fp = fopen(path,"r");
    char buffer[1024];
    if(fp){
	while (fgets(buffer, sizeof(buffer), fp) != NULL)
    	{
        	buffer[strlen(buffer) - 1] = '\0'; // eat the newline fgets() stores
		strcat(buffer,",");
		char* temp = strdup(buffer);
		strcpy(temp,buffer);
		char* last = temp;
		int sortColumnInside=0;
		while((temp = strstr(temp,","))!=NULL){

			*temp='\0';
			temp = temp+1;
			char* trimmed = trimwhitespace(last);
			int i = 0;
			while(i<NUM_HEADERS){
			    if(strcmp(trimmed,movie_headers[i])==0){
				i=99;
			    }
			    i++;
			}
			if(i!=100){
				return -4;
			}
			if(strcmp(trimmed,sortColumn)==0){
				sortColumnInside=1;
			}
			printf("%s\n",last);
			last=temp;

		}
		if(sortColumnInside!=1){
			return -5;
		}
		printf("%s\n",last);
		break;
         	//printf("%s/n", buffer);
    	}
    }
    else{
	return -3;
    }
    return 1;
}
int scan(const char *name)
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
        //printf("%d,", getpid());
        fflush(stdout);
        int subchildren = scan(path);
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

        //TODO Check for valid FORMAT
        char path[1024];
        strcpy(path,name);
        strcat(path,"/");
        strcat(path,cursor->d_name);
	//printf("%s %d,", path,getpid());
        fflush(stdout);
        //movie_data* head = parse_csv(path);
        /*
	if(head!=NULL){
		printf("RIGHT");
	}
	*/
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

  printf("%d\n",argc);

  if(argc<3){
	fprintf(stderr, "Too Few Arguments Exiting\n");
	exit(-1);
  }
  int cFlag = 0;
  char* cValue;
  int oFlag = 0;
  char* oValue;
  int dFlag = 0;
  char* dValue;
  int i = 1;
  while(i<argc){
	if(strcmp(argv[i],"-c")==0){
		cFlag=1;
		cValue=argv[i+1];
	}
	else if(strcmp(argv[i],"-o")==0){
		oFlag=1;
		oValue = argv[i+1];
	}
	else if(strcmp(argv[i],"-d")==0){
		dFlag=1;
		dValue = argv[i+1];

	}
	else{
		fprintf(stderr, "Unkown Flag\n");
		exit(-1);
	}
	i+=2;
  }

  /*printf("%d\n",argc);
  check_parameters(argc, argv);
  check_sort_by();
  get_directory_paths(argc, argv);
  //printf("Initial PID: %d\nPIDS of all child processes: ", getpid());
  fflush(stdout);
  // printf("PIDS of all child processes: ");
  int end = scan(".");
  pid_t wpid;
  int status = 0;
  int sum = 0;
  while ((wpid = wait(&status)) > 0)
  {
  }
  //fflush(stdout);
  //printf("\nTotal number of processes: %d\n",end);

  movie_data* head = parse_csv("movie_metadata.csv");

	if(head==NULL){
		printf("RIGHT");
	}
  */
  return 1;

}

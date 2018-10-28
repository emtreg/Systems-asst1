#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
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
		printf("%s %d\n",path, getpid() ); 
		int subchildren = listdir(path);
		exit(subchildren);
		break;
	    }
	    else{
		int status;
		if ( waitpid(child, &status, 0) == -1 ) {
        		perror("waitpid failed");
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
	    if (suffix && !strcmp(suffix, ".csv")){
		child=fork();
		if(child==0){
            		printf("%s %s %d\n", name, cursor->d_name, getpid());
			//TODO Check for valid FORMAT
			//TODO SORT FUNCTION HERE
			exit(1);
		}
		else{
			children++;
			//printf("%s %d\n",cursor->d_name, getpid() );
			continue;	
		}
	    }
	    else{
		continue;
	   }
        }
    }
    printf("RETURNING %d\n",children);
    closedir(dir);
    return children;
}

int main(void) {
    printf("Initial PID: %d\n", getpid());
    int end = listdir(".");
    pid_t wpid;
    int status = 0;
    int sum = 0;
    while ((wpid = wait(&status)) > 0)
    {
    }
    return 1;
}

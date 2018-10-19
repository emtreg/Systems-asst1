#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "scannerCSVsorter.h"

char *sort_by;
//char *search_dir_path;
char *output_dir_path;
char *filename;
char sorted_filename[256];
char output_file_path[256];
DIR *search_dir;
DIR *output_dir;



int main(int argc, char *argv[]) {

check_parameters(argc, argv);
get_directory_path(3, 4,"-d", search_dir, argc, argv);
//printf("\"%s\"\n", search_dir_path);
get_directory_path(5, 6,"-o", output_dir, argc, argv);
//create_sorted_csv_file();

return 0;

}


void check_parameters(int argc, char *argv[]) {

    if(argc < 3) {
        fprintf(stderr, "%s", "Must enter at least two parameters.\n");
        exit(-1);
    } else {
        if (strcmp(argv[1], "-c") != 0) {
            fprintf(stderr, "%s", "\"-c\" not present.\n");
            exit(-1);
        } else {
            sort_by = argv[2];
            check_sort_by();
        }
    }
}


void check_sort_by() {

    int i = 0;
    int match_found = 0;

    for(i = 0; i < NUM_HEADERS; i++) {
        if(strcmp(movie_headers[i], sort_by) == 0) {
            match_found = 1;
        }
    }

    if(match_found == 0) {
        fprintf(stderr, "%s", "Invalid column name.\n");
        exit(-1);
    }
}


void get_directory_path(int flag_index, int path_index, char *flag, DIR *dir, int argc, char* argv[]) {

    if(argc > path_index) {

        if(strcmp(argv[flag_index], flag) == 0) {
            char *dir_path = strdup(argv[path_index]);
            check_directory_exists(dir, dir_path);
        } else {
            fprintf(stderr, "\"%s\" not present.\n", flag);
            exit(-1);
        }

    } else {
        dir = opendir(".");
        printf("No directory specified. Using current directory\n");
    }
}


void check_directory_exists(DIR *dir, char* path) {

    dir = opendir(path);
    struct dirent *ent;

    if(!dir) {
        printf("Unable to access directory. Shutting down.\n");
        exit(-1);

    } else {
        printf("Directory exists\n");

        while ((ent = readdir (dir)) != NULL) {
            filename = strdup(ent->d_name);
            //printf("\"%s\"\n", filename);
            check_file_extension();
        }

        closedir (dir);
    }
}


void check_file_extension() {

    char *extension = strrchr(filename, '.');

    if(!extension || extension == filename) {
        extension = "";
    }

    if(strcmp(extension, ".csv") == 0) {
        //check CSV format
        trim_filename(filename);
    }

    //printf("\'%s\'\n", extension);
}


void trim_filename(char *filename) {

    char *trimmed_filename;
    trimmed_filename = strdup(filename);
    int len = strlen(trimmed_filename);
    trimmed_filename[len-4] = '\0';
    //printf("\"%s\"\n", trimmed_filename);
    rename_sorted_csv(trimmed_filename);

}


void rename_sorted_csv(char *trimmed_filename) {

    strcat(sorted_filename, trimmed_filename);
    strcat(sorted_filename, "-sorted-");
    strcat(sorted_filename, sort_by);
    strcat(sorted_filename, ".csv");
    printf("%s\n", sorted_filename);
    memset(sorted_filename, 0, sizeof(sorted_filename));

}

void create_sorted_csv_file() {

    strcat(output_file_path, output_dir_path);
    strcat(output_file_path, "/");
    strcat(output_file_path, "movie_metadata-sorted-movie_title-.csv");

    printf("%s\n", output_file_path);

}




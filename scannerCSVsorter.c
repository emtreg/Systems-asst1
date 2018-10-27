#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include "scannerCSVsorter.h"
#include "mergesort.c"


#define NUM_HEADERS 28
#define _POSIX_SOURCE
#undef _POSIX_SOURCE


char *sort_by;
char search_dir_path[5000];
char output_dir_path[5000];
char output_file_path[5000];
char *filename;
char sorted_filename[256];
DIR *search_dir;
DIR *output_dir;
char cwd[256];
char headers_array[NUM_HEADERS][256];

movie_data *front;
int pos_sort;
char headers_array[NUM_HEADERS][256];
int header_comma_count = 0;
int header_match;
int comma_match;
int valid_csv = 1;



int main(int argc, char *argv[]) {

check_parameters(argc, argv);
check_sort_by();

get_directory_paths(argc, argv);

printf("\"%s\"\n", search_dir_path);
printf("\"%s\"\n", output_dir_path);

create_sorted_csv(sorted_filename);
//printf("\"%s\"\n", output_file_path);

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
            return;
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


void get_directory_paths(int argc, char *argv[]) {

    if(argc == 3) {

        search_dir = opendir(".");
        getcwd(search_dir_path, sizeof(cwd));

        printf("No directory specified. Using current directory\n");

        output_dir = opendir(".");
        getcwd(output_dir_path, sizeof(cwd));

        printf("No directory specified. Using current directory\n");

    } else if(argc == 4 && strcmp(argv[3], "-d") == 0) {

        printf("Please specify search directory.\n");
        exit(-1);

    } else if(argc == 4 && strcmp(argv[3], "-o") == 0) {

        printf("Please specify output directory.\n");
        exit(-1);

    } else if(argc == 5 && strcmp(argv[3], "-o") == 0) {

        search_dir = opendir(".");
        getcwd(search_dir_path, sizeof(cwd));
        printf("No directory specified. Using current directory\n");

        strcpy(output_dir_path, argv[4]);
        check_directory_exists(output_dir, output_dir_path);

    } else if(argc == 5 && strcmp(argv[3], "-d") == 0) {

        strcpy(search_dir_path, argv[4]);
        check_directory_exists(search_dir, search_dir_path);

        output_dir = opendir(".");
        getcwd(output_dir_path, sizeof(cwd));
        printf("No directory specified. Using current directory\n");

    } else if(argc == 6 && strcmp(argv[5], "-o") == 0) {

        printf("Please specify output directory.\n");
        exit(-1);

    } else if(argc >= 7 && strcmp(argv[3], "-d") == 0 && strcmp(argv[5], "-o") == 0) {

        strcpy(search_dir_path, argv[4]);
        check_directory_exists(search_dir, search_dir_path);

        strcpy(output_dir_path, argv[6]);
        check_directory_exists(output_dir, output_dir_path);
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
            printf("\"%s\"\n", filename);
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
        check_csv_format();
    }

    printf("\'%s\'\n", extension);
}


void trim_filename() {

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



void create_sorted_csv(char sorted_filename[]) {

    strcpy(output_file_path, output_dir_path);
    strcat(output_file_path, "/");
    strcat(output_file_path, sorted_filename);

}

char* trimwhitespace (char *str) {

    char *str_copy = strdup(str);
    int len = strlen(str_copy);
    char *end;
    int i = 0;

    //Trim leading space
    while(str_copy[i] == ' ') {
        str_copy++;
    }

    // Trim trailing space
    int modified_length = strlen(str_copy)-1;

    while(str_copy[modified_length] == ' ') {
        str_copy[modified_length] = '\0';
        modified_length--;
    }

    return str_copy;
}

void push (movie_data *new_node) {
    movie_data *old_node = get_last_node();
    old_node->next = new_node;
    new_node->next = NULL;
}

// returns last node in linked list

movie_data* get_last_node() {
    movie_data *current = front;
    while (current->next != NULL) {
        current = current->next;
    }
    return current;
}

void store_headers(char *headers) {

    int str_length = strlen(headers);
    int count = 0;
    int index = 0;
    int i = 0;
    int comma_count = 0;
    int n = 0;
    int m =0;
    int header_match = 0;

    char header[str_length];

    for(i = 0; i < str_length; i++) {

        if(headers[i] == ',') {
            comma_count++;
            header[index] = '\0';
            strcpy(headers_array[count++], trimwhitespace(header));
            strcpy(header,"");
            index = 0;

        } else if (headers[i] == '\r') { //ignore carriage returns used in Windows line endings, see http://www.cs.toronto.edu/~krueger/csc209h/tut/line-endings.html
            continue;

        } else {
            header[index++] = headers[i];
        }
    }

    header[index] = '\0';
    strcpy(headers_array[count++], trimwhitespace(header));



    while(strcmp(headers_array[n], "") != 0) {

        for(m = 0; m <= 27; m++) {

            if(strcmp(headers_array[n], movie_headers[m]) == 0) {
                header_match = 1;
                break;
            }
        }

        if(header_match == 0) {
            printf("Not valid CSV\n");
            valid_csv = 0;
            return;
        }

        header_match = 0;
        n++;
    }

    header_comma_count = comma_count;
}



void check_sort_by_csv() {

    int i = 0;
    int match_found = 0;

    for(i = 0; i < NUM_HEADERS; i ++) {
        if(strcmp(headers_array[i], sort_by) == 0) {
            pos_sort = i;
            match_found = 1;
        }
    }

    if(match_found == 0) {
        fprintf(stderr, "%s", "Not valid CSV.\n");
        valid_csv = 0;
        return;
    }

}

void build_movie_data_node(char *data_str) {

    int str_length = strlen(data_str);
    char row_element[str_length];
    int i = 0;
    int count = 0;
    int char_count = 0;
    int double_quote_mode = 1;
    int comma_count = 0;

    movie_data *output;
    output = malloc(sizeof(movie_data));

    for(i = 0; i < strlen(data_str); i++) {

        if(data_str[i] == '\"') {
            if(double_quote_mode == 1) {
                double_quote_mode = 0;
            } else {
                double_quote_mode = 1;
            }
        } else if(data_str[i] == ',' && double_quote_mode == 1) {
            comma_count++;
            row_element[char_count] = '\0';
            assign_output(output, count++, row_element);
            strcpy(row_element, "");
            char_count = 0;
        } else {
            row_element[char_count++] = data_str[i];
        }
    }

    if(comma_count != header_comma_count) {

        printf("Not valid CSV\n");
        valid_csv = 0;
        return;
    }

    comma_count = 0;

    row_element[char_count] = '\0';
    assign_output(output, count, row_element);

    output->raw_row = strdup(data_str);

    push(output);

}


void assign_output(movie_data *output, int index, char *value) {
    if(strcmp(headers_array[index], "color") == 0) {
        output->color = strdup(value);
    } else if(strcmp(headers_array[index], "director_name") == 0) {
        output->director_name = strdup(value);
    } else if(strcmp(headers_array[index], "num_critic_for_reviews") == 0) {
        output->num_critic_for_reviews = atoi(value);
    } else if(strcmp(headers_array[index], "duration") == 0) {
        output->duration = atoi(value);
    } else if(strcmp(headers_array[index], "director_facebook_likes") == 0) {
        output->director_facebook_likes = atoi(value);
    } else if(strcmp(headers_array[index], "actor_3_facebook_likes") == 0) {
        output->actor_3_facebook_likes = atoi(value);
    } else if(strcmp(headers_array[index], "actor_2_name") == 0) {
        output->actor_2_name = strdup(value);
    } else if(strcmp(headers_array[index], "actor_1_facebook_likes") == 0) {
        output->actor_1_facebook_likes = atoi(value);
    } else if(strcmp(headers_array[index], "gross") == 0) {
        output->gross = atof(value);
    } else if(strcmp(headers_array[index], "genres") == 0) {
        output->genres = strdup(value);
    } else if(strcmp(headers_array[index], "actor_1_name") == 0) {
        output->actor_1_name = strdup(value);
    } else if(strcmp(headers_array[index], "movie_title") == 0) {
        output->movie_title = strdup(value);
    } else if(strcmp(headers_array[index], "num_voted_users") == 0) {
        output->num_voted_users = atoi(value);
    } else if(strcmp(headers_array[index], "cast_total_facebook_likes") == 0) {
        output->cast_total_facebook_likes = atoi(value);
    } else if(strcmp(headers_array[index], "actor_3_name") == 0) {
        output->actor_3_name = strdup(value);
    } else if(strcmp(headers_array[index], "facenumber_in_poster") == 0) {
        output->facenumber_in_poster = atoi(value);
    } else if(strcmp(headers_array[index], "plot_keywords") == 0) {
        output->plot_keywords = strdup(value);
    } else if(strcmp(headers_array[index], "movie_imdb_link") == 0) {
        output->movie_imdb_link = strdup(value);
    } else if(strcmp(headers_array[index], "num_user_for_reviews") == 0) {
        output->num_user_for_reviews = atoi(value);
    } else if(strcmp(headers_array[index], "language") == 0) {
        output->language = strdup(value);
    } else if(strcmp(headers_array[index], "country") == 0) {
        output->country = strdup(value);
    } else if(strcmp(headers_array[index], "content_rating") == 0) {
        output->content_rating = strdup(value);
    } else if(strcmp(headers_array[index], "budget") == 0) {
        output->budget = atof(value);
    } else if(strcmp(headers_array[index], "title_year") == 0) {
        output->title_year = atoi(value);
    } else if(strcmp(headers_array[index], "actor_2_facebook_likes") == 0) {
        output->actor_2_facebook_likes = atoi(value);
    } else if(strcmp(headers_array[index], "imdb_score") == 0) {
        output->imdb_score = atof(value);
    } else if(strcmp(headers_array[index], "aspect_ratio") == 0) {
        output->aspect_ratio = atof(value);
    } else if(strcmp(headers_array[index], "movie_facebook_likes") == 0) {
        output->movie_facebook_likes = atoi(value);
    }
}



void check_csv_format() {

    valid_csv = 1;

    parse_csv();

    if(valid_csv == 0) {
        return;
    }

    else if(valid_csv == 1) {
        printf("Valid CSV\n");
        movie_data *movie_node = parse_csv();
        trim_filename();

    }

}


movie_data* parse_csv() {

char *mode = "r";

FILE *f;

f = fopen(filename, mode);

    front = NULL;
    front = malloc(sizeof(movie_data)); //initialization and memory allocation for front node
    front->next = NULL;

    int buffer_size = 0;
    int char_count = 0;

    char *tmp;
    char *row;

    row = malloc(sizeof(char) * (buffer_size+1));
    row[buffer_size++] = '\0';

    int row_count = 0;

    char current_char;

    while((current_char = getc(f)) != EOF) {

        char_count++;

        if(current_char == '\n') {
            char *tmp_str = malloc(sizeof(char) *(char_count + 1));
            strcpy(tmp_str, row);

            if(row_count == 0) {
                store_headers(tmp_str);
                check_sort_by_csv();
                printf("%s\n", tmp_str);
            } else {
                build_movie_data_node(tmp_str);
            }

            printf("\"%s\"\n", trimwhitespace(tmp));
            free(tmp_str);

            buffer_size = 0;
            free(row);
            row = malloc(sizeof(char) * (buffer_size + 1));
            row[buffer_size++] = '\0';

            char_count = 0;
            row_count++;

        } else {
            tmp = realloc(row, buffer_size + 1);

            if(tmp != NULL) {
                row = tmp;
                row[buffer_size - 1] = current_char;
                row[buffer_size++] = '\0';
            }
        }
    }


    free(row);

    movie_data* n = mergeSort(front->next, sort_by);

    return n;

    /*

    while (n != NULL)
    {
        printf("%s\n", n->raw_row);
        n = n->next;
    }

    */

    return 0;

}


 /*if(argc > path_index) {

        if(strcmp(argv[flag_index], flag) == 0) {
           strcpy(dir_path, argv[argv_index]);
            check_directory_exists(dir, dir_path);
        } else {
            fprintf(stderr, "\"%s\" not present.\n", flag); //-o <directory> (use default behavior for -d), -d fatal error
            exit(-1);
        }

    } */


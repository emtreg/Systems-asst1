#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "simpleCSVsorter.h"
#define NUM_HEADERS 28
#include "mergesort.c"

movie_data *front;
char *sort_by;
int pos_sort;
char headers_array[NUM_HEADERS][256];


int main(int argc, char *argv[])
{

    front = NULL;
    front = malloc(sizeof(movie_data)); //initialization and memory allocation for front node
    front->next = NULL;

    sort_by = argv[2];
    check_parameters(argc);

    int buffer_size = 0;
    int char_count = 0;

    char *tmp;
    char *row;

    row = malloc(sizeof(char) * (buffer_size+1));
    row[buffer_size++] = '\0';

    int row_count = 0;

    char current_char;

    while((current_char = getchar()) != EOF) {

        char_count++;

        if(current_char == '\n') {
            char *tmp_str = malloc(sizeof(char) *(char_count + 1));
            strcpy(tmp_str, row);

            if(row_count == 0) {
                store_headers(tmp_str);
                check_sort_by();
                printf("%s\n", tmp_str);
            } else {
                build_movie_data_node(tmp_str);
            }

            //printf("\"%s\"\n", trimwhitespace(tmp));
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

    while (n != NULL)
    {
        printf("%s\n", n->raw_row);
        n = n->next;
    }

    return 0;

}

//checks if two arguments have been passed to program

void check_parameters(int argc) {
    if(argc < 3) {
        fprintf(stderr, "%s", "Must enter two parameters.\n");
    }
}


//trims leading and trailing whitespaces of tokens

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


// pushes a node to end of linked list

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

    char header[str_length];

    for(i = 0; i < str_length; i++) {

        if(headers[i] == ',') {
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

}

void check_sort_by() {

    int i = 0;
    int match_found = 0;

    for(i = 0; i < NUM_HEADERS; i ++) {
        if(strcmp(headers_array[i], sort_by) == 0) {
            pos_sort = i;
            match_found = 1;
        }
    }

    if(match_found == 0) {
        fprintf(stderr, "%s", "Invalid column name.\n");
        exit(-1);
    }

}


void build_movie_data_node(char *data_str) {

    int str_length = strlen(data_str);
    char row_element[str_length];
    int i = 0;
    int count = 0;
    int char_count = 0;
    int double_quote_mode = 1;

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
            row_element[char_count] = '\0';
            assign_output(output, count++, row_element);
            strcpy(row_element, "");
            char_count = 0;
        } else {
            row_element[char_count++] = data_str[i];
        }
    }

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

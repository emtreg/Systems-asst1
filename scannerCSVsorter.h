#define NUM_HEADERS 28

char movie_headers[NUM_HEADERS][256] = {

        "color",
        "director_name",
        "num_critic_for_reviews",
        "duration",
        "director_facebook_likes",
        "actor_3_facebook_likes",
        "actor_2_name",
        "actor_1_facebook_likes",
        "gross",
        "genres",
        "actor_1_name",
        "movie_title",
        "num_voted_users",
        "cast_total_facebook_likes",
        "actor_3_name",
        "facenumber_in_poster",
        "plot_keywords",
        "movie_imdb_link",
        "num_user_for_reviews",
        "language",
        "country",
        "content_rating",
        "budget",
        "title_year",
        "actor_2_facebook_likes",
        "imdb_score",
        "aspect_ratio",
        "movie_facebook_likes"

};


void check_parameters(int argc, char *argv[]);
void check_sort_by();
void get_directory_paths(int argc, char *argv[]); //int argv_index, int flag_index, int path_index, char *flag, DIR *dir, char dir_path[], int argc, char* argv[]);
void check_directory_exists();
void check_file_extension();
void trim_filename();
void rename_sorted_csv();
void create_sorted_csv(char sorted_filename[]);
void check_csv_format();


typedef struct node {
            char *raw_row;
            char *color;
            char *director_name;
            int num_critic_for_reviews;
            int duration;
            int director_facebook_likes;
            int actor_3_facebook_likes;
            char *actor_2_name;
            int actor_1_facebook_likes;
            double gross;
            char *genres;
            char *actor_1_name;
            char *movie_title;
            int num_voted_users;
            int cast_total_facebook_likes;
            char *actor_3_name;
            int facenumber_in_poster;
            char *plot_keywords;
            char *movie_imdb_link;
            int num_user_for_reviews;
            char *language;
            char *country;
            char *content_rating;
            double budget;
            int title_year;
            int actor_2_facebook_likes;
            double imdb_score;
            double aspect_ratio;
            int movie_facebook_likes;
            struct node *next;
    }movie_data;

    //void check_parameters(int argc);
    movie_data* get_last_node();
    char* trimwhitespace (char *str);
    void push (movie_data *new_node);
    void store_headers(char *headers);
    void check_sort_by_csv();
    void build_movie_data_node(char *data_str);
    void assign_output(movie_data *output, int index, char *value);
    movie_data* parse_csv(char *filename);
/*
 *  * ALWAYS CALL THIS FIRST AND CHECK FOR NULL getNumericalValue WILL FAIL IF SEARCHING FOR NON NUMERICAL VALUE
 *   */
char* getStringValues(movie_data head, char* header){
        if(!strcmp(header,"color")){
            return head.color;
        }
        if(!strcmp(header,"director_name")){
            return head.director_name;
        }
        if(!strcmp(header,"actor_2_name")){
            return head.actor_2_name ;
        }
        if(!strcmp(header,"actor_1_name")){
            return head.actor_1_name ;
        }
        if(!strcmp(header,"actor_3_name")){
            return head.actor_3_name ;
        }
        if(!strcmp(header,"genres")){
            return head.genres ;
        }
        if(!strcmp(header,"movie_title")){
            return head.movie_title ;
        }
        if(!strcmp(header,"plot_keywords")){
            return head.plot_keywords ;
        }
        if(!strcmp(header,"movie_imdb_link")){
            return head.movie_imdb_link ;
        }
        if(!strcmp(header,"language")){
            return head.language ;
        }
        if(!strcmp(header,"country")){
            return head.country ;
        }
        if(!strcmp(header,"content_rating")){
            return head.content_rating ;
        }
	return NULL;


};
double getNumericalValue(movie_data head, char* header){
	if(!strcmp(header,"num_critic_for_reviews")){
	    return head.num_critic_for_reviews;
	}
	if(!strcmp(header,"duration")){
        return head.duration;
	}
	if(!strcmp(header,"director_facebook_likes")){
        return head.director_facebook_likes;
	}
	if(!strcmp(header,"actor_3_facebook_likes")){
        return head.actor_3_facebook_likes;
	}
	if(!strcmp(header,"actor_1_facebook_likes")){
		return head.actor_1_facebook_likes;
	}
	if(!strcmp(header,"num_voted_users")){
	    return head.num_voted_users;

	}
	if(!strcmp(header,"cast_total_facebook_likes")){
		return head.cast_total_facebook_likes;
	}
	if(!strcmp(header,"facenumber_in_poster")){
        return head.facenumber_in_poster;
	}
	if(!strcmp(header,"num_user_for_reviews")){
        return head.num_user_for_reviews;
	}
	if(!strcmp(header,"title_year")){
        return head.title_year;

	}
	if(!strcmp(header,"actor_2_facebook_likes")){
        return head.actor_2_facebook_likes;
	}
	if(!strcmp(header,"movie_facebook_likes")){
        return head.movie_facebook_likes;
	}
    if(!strcmp(header,"aspect_ratio")){
        return head.aspect_ratio;
    }
    if(!strcmp(header,"imdb_score")){
        return head.imdb_score;
    }
    if(!strcmp(header,"budget")){
        return head.budget;
    }
    if(!strcmp(header,"gross")){
        return head.gross;
    }
}






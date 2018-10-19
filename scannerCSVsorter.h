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
void get_directory_path(int flag_index, int path_index, char *flag, DIR *dir, int argc, char* argv[]);
void check_directory_exists();
void check_file_extension();
void trim_filename(char *filename);
void rename_sorted_csv();
void create_sorted_csv_file();

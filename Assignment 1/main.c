#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//making the struct for our movies

struct movie{
  char title[1024];
  int year;
  char** languages;  //remember arr of pontr to char array
  int numLanguages;
  float rating;
  struct movie *next;
  
};

struct movie* createMovie(char * filename){
	struct movie* curr_movie = malloc(sizeof(struct movie));
	char *ptr;

	//title
	char *token = strtok_r(filename, "\t", &ptr);
   // 	curr_movie->title = calloc(strlen(token) + 1, sizeof(char));
    	strcpy(curr_movie->title, token);

	// The next token is the year
    	token = strtok_r(NULL, "\t", &ptr);
    	curr_movie->year = atoi(token);
	
	//languages
	 token = strtok_r(NULL, "\t", &ptr);
    	char *langStart = strchr(token, '[');
    	char *langEnd = strchr(token, ']');
    	if (langStart != NULL && langEnd != NULL) {
        // Calculate the length of the languages string
        	int langLength = langEnd - langStart - 1;
        // Allocate memory for the languages array
        	curr_movie->languages = calloc(1, sizeof(char *));
        // Tokenize languages further based on ';'
        	char *langToken = strtok(langStart + 1, ";");
        	curr_movie->numLanguages = 0;
        	while (langToken != NULL) {
        	    	curr_movie->languages[curr_movie->numLanguages] = calloc(strlen(langToken) + 1, sizeof(char));
        	    	strcpy(curr_movie->languages[curr_movie->numLanguages], langToken);
        	    	curr_movie->numLanguages++;
            // Resize the languages array
        	    	curr_movie->languages = realloc(curr_movie->languages, (curr_movie->numLanguages + 1) * sizeof(char *));
        	    	langToken = strtok(NULL, ";");
		}
        }
	
	//rating
	token = strtok_r(NULL, "\t", &ptr);
        curr_movie->rating = atof(token);

	curr_movie->next = NULL;
	return curr_movie;
}



struct movie *processFile(char *filename){
	FILE *data_file = fopen(filename, "r");
	char *currLine = NULL;
    	size_t len = 0;
   	ssize_t nread;
    	char *token;
	int lineNum = 0;

// The head of the linked list
    	struct movie *head = NULL;
// The tail of the linked list
    	struct movie *tail = NULL;

	while ((nread = getline(&currLine, &len, data_file)) != -1) {
// Skip the first line
		if (lineNum == 0) {
            		lineNum++;
            		continue;
        	}
        
		struct movie *newNode = createMovie(currLine);
		if (head == NULL) {
            // This is the first node in the linked link
            // Set the head and the tail to this node
            		head = newNode;
            		tail = newNode;
        	} else {
            // This is not the first node.
            // Add this node to the list and advance the tail
            		tail->next = newNode;
            		tail = newNode;
        	}
        	lineNum++;
	}//while loop
	//free(currLine);
    	fclose(data_file);
    	return head;

}

//now below are the program options

//#1.Show movies released in the specified year
void movieYear(struct movie *head, int year){
	struct movie* curr = head;
	int found = 0;
	while (curr != NULL){
		if (curr->year  == year){
			printf("%s\n", curr->title);
			found = 1;
		}
	curr = curr->next;
	}
	if (!found) {
       		printf("No movies found for the year %d\n", year);
 	}   	
}

//2.Show highest rated movie for each year
void showHighestRatedMovies(struct movie *head) {
//my tracker
    int currentYear = -1; 
    float maxRating = -1; 
    char *maxRatedMovieTitle = NULL;

    struct movie *current = head;

    while (current != NULL) {
        if (current->year != currentYear) {
            // if there was a highest rated movie for the previous year, print it
            if (maxRatedMovieTitle != NULL) {
                printf("Highest rated movie for %d: %s (Rating: %.1f)\n", currentYear, maxRatedMovieTitle, maxRating);
            }

      //currentYear and reset maxRating and maxRatedMovieTitle for the new year
            currentYear = current->year;
            maxRating = current->rating;
            maxRatedMovieTitle = strdup(current->title);
        } else {
            // if the year of the current movie matches the current year being processed
            // update maxRating and maxRatedMovieTitle if the rating of the current movie is higher
            if (current->rating > maxRating) {
                maxRating = current->rating;
                maxRatedMovieTitle = strdup(current->title); 
            }
        }
//next mov
        current = current->next; 
    }

    //highest rated movie for the last year in the list
    if (maxRatedMovieTitle != NULL) {
        printf("Highest rated movie for %d: %s (Rating: %.1f)\n", currentYear, maxRatedMovieTitle, maxRating);
    }
}

//3. Show the title and year of release of all movies in a specific language

void langReleaseYear(struct movie *list){

    char language[20];
    int match = 0; // acts as bool variable whether the movie is of the language specified

    printf("Enter the language for which you want to see movies: ");
    scanf("%s", &language);

    struct movie * temp = list;
    while(temp){
        int i = 0;
        // for each of the movie languages
        while(temp->languages[i]){

            // prints the movie title and year if it is in the same language the user asked for
            if(strcmp(temp->languages[i], language) == 0){
                match = 1;
                printf("%d %s\n", temp->year, temp->title);
                break;
            }
            i++;
        }
        temp = temp->next;
    }

    if(!match)
        printf("No data about movies released in %s\n", language);
}





int main(int argc, char *argv[])
{	
	if (argc < 2){
        	printf("You must provide the name of the file to process\n");
        	printf("Example usage: ./students student_info1.txt\n");
        	return EXIT_FAILURE;
    	}
	struct movie* head = processFile(argv[1]);
  
	int choice = 0;
	while (choice != 4){	//as long as the user doesn't want to quit
		  printf("1. Show movies released in the specified year\n");
	    printf("2. Show highest rated movie for each year\n");
	    printf("3. Show the title&year of release of all movies in a specific language\n");
	    printf("4. Quit\n");
	    printf("Choose an option: ");
	    scanf("%d", &choice);
         
     while(!((choice == 1)||(choice == 2)||(choice == 3 )||(choice == 4))){
        printf("You entered an incorrect choice. Try again.");
        printf("\n\n1. Show movies released in the specified year");
        printf("\n2. Show highest rated movie for each year");
        printf("\n3. Show the title and year of release of all movies in a specific language");
        printf("\n4. Exit from the program\n");
        printf("\nEnter a choice from 1 to 4: ");
        scanf("%d", &choice);
    }

		if (choice == 1){
			printf("Enter the year");
			int year = 0;
			scanf("%d", &year);
			movieYear(list, year);
		}
		else if (choice == 2){
			showHighestRatedMovie(list);
		}
		else if (choice == 3){
		  langReleaseYear(list);
		}
 
		else if (choice == 4){
			printf("Program will exit");
		} 
		else printf("Invalid. Try again");
  
	} 

return EXIT_SUCCESS;
}


























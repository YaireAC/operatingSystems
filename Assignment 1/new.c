#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dynarray.h"

//using the include from the dynarray from cs 162
/*This is my original code for assignment 1*/

struct movie{
  char* title;
  int year;
  char** languages;  //remember arr of pontr to char array
  int numLanguages;
  float rating;
  struct movie *next;
  
};

struct movie *createMovie(struct movie* single_movie){
  struct movie* curr_movie = malloc(sizeof(struct movie));
  curr_movie->title = single_movie->title;
  curr_movie->year = single_movie->year;
  curr_movie->languages = malloc(5*sizeof(char));
  
  for (int i = 0; i < 5; i++){
    if(single_movie->languages[i]){
      curr_movie->languages[i] = calloc(strlen(single_movie->languages[i]) + 1, sizeof(char));
      strcpy(curr_movie->languages[i], single_movie->languages[i]);
    }else
      curr_movie->languages[i] = NULL;
  }  
    curr_movie->rating = single_movie->rating;
    
    return curr_movie;
  
}

struct movie *divider(char* currLine){
   struct movie* curr_movie = malloc(sizeof(struct movie));
   char* pntr;
   char* pntr2;
   
   //the title
   char* token = strtok_r(currLine, ",", &pntr);
   curr_movie->title = calloc(strlen(token) + 1, sizeof(char));
   strcpy(curr_movie->title, token);
   
   //the year
   token = strtok_r(NULL, ",", &pntr);
    int tokeni = atoi(token);
    curr_movie->year = tokeni; 
    
    //language
    token = strtok_r(NULL, ",", &pntr);
    token++;
   
   curr_movie->languages = malloc(5*sizeof(char));
   for(int i = 0; i < 5; i++){
        curr_movie->languages[i] = NULL;    
    }
    
    for(int i = 0; i < 5; i++){
        token = strtok_r(token, "[;]", &pntr2);
    
        if(token){
            curr_movie->languages[i] = calloc(strlen(token) + 1, sizeof(char));
            strcpy(curr_movie->languages[i], token);
        }
            
        token = strtok_r(NULL, "]", &pntr2); //cut off the "]"
    }
    
    //the rating
    token = strtok_r(NULL, "\n", &pntr);
    float tokendb = strtod(token, NULL);
    curr_movie->rating = tokendb;
    
    
    curr_movie->next = NULL;

    return curr_movie;

}

struct movie *processFile(char *filename){
  FILE *data_file = fopen(filename, "r");
	char *currLine = NULL;
	size_t len = 0;
   	ssize_t nread;
    char* token;
    char* pntr;


// The head of the linked list
    	struct movie *head = NULL;
// The tail of the linked list
    	struct movie *tail = NULL;
     
     getline(&currLine, &len, data_file);  //skip titles and lables
     int num = 0;
     
     while ((nread = getline(&currLine, &len, data_file)) != -1){
        num++;
        struct movie *newNode = divider(currLine);

        if (head == NULL){
            // firdt node then head and tail
            head = newNode;
            tail = newNode;
        }
        else{
            // not the first node, add this node to the list and advance the tail
            tail->next = newNode;
            tail = newNode;
        }
    }
    fclose(data_file);
    printf("Processed file %s and parsed data for %d movies \n", filename, num);

    return head;

}

//#1.Show movies released in the specified year
void movieYear(struct movie *list, int year){
  int match = 0;
  struct movie * temp = list;
    while(temp){
      if(temp->year == year){
          match = 1;
          printf("%s\n", temp->title);
      }
      temp = temp->next;
    }
    if(!match){
        printf("No data about movies released in the year %d\n", year);
    }
}

//2.Show highest rated movie for each year
void showHighestRatedMovie(struct movie *list){
   struct dynarray * da = dynarray_create();

    struct movie * temp = list;
    struct movie * temp2;
    struct movie * temp3;
    
    int placed; // acts as a bool variable whether the movie was put in a list or not
    
    while(temp){
        placed = 0;
        temp3 = createMovie(temp);
        temp3->next = NULL;
        
        // checks through each spot in the dynamic array
        for(int i = 0; i < dynarray_size(da); i++){
            temp2 = dynarray_get(da, i);
            
            // if there is already a spot in the array for that year, add temp3 to front of list
            if(temp2->year == temp3->year){ 
                temp3->next = temp2;
                dynarray_set(da, i, temp3);
                placed = 1;
                break;
            }
        }

        if(!placed){
            dynarray_insert(da, temp3);
        }
        
        temp = temp->next;
    }

    struct movie * m;
    struct movie * highest;

    for(int i = 0; i < dynarray_size(da); i++){
         
        m = dynarray_get(da, i);
        highest = m;

         
        while(m){
            
            if(m->rating > highest->rating)
                highest = m;

            m = m->next;
        }
        printf("%d %.1f %s\n", highest->year, highest->rating, highest->title);
    } 
	printf("option 2");
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





int main(int argc, char *argv[]){
  if (argc < 2)
    {
        printf("You must provide the name of the file to process\n");
        printf("Example usage: ./movies movie_info1.txt\n");
        return EXIT_FAILURE;
    }
    struct movie *list = processFile(argv[1]);
    
    int choice = 0;
    do {
        printf("1. Show movies released in the specified year\n");
        printf("2. Show highest rated movie for each year\n");
        printf("3. Show the title&year of release of all movies in a specific language\n");
        printf("4. Quit\n");
        printf("Choose an option: ");
        if (scanf("%d", &choice) != 1) {
            // Clear input buffer in case of non-integer input
            while (getchar() != '\n');
            choice = 0; // Reset choice to prompt again
        }
        if (choice < 1 || choice > 4) {
            printf("Invalid choice. Please enter a number between 1 and 4.\n");
        } else {
            switch (choice) {
                case 1:
                    printf("Enter the year: ");
                    int year = 0;
                    if (scanf("%d", &year) != 1) {
                        printf("Invalid input. Please enter a valid year.\n");
                        // Clear input buffer in case of non-integer input
                        while (getchar() != '\n');
                    } else {
                        movieYear(list, year);
                    }
                    break;
                case 2:
                    showHighestRatedMovie(list);
                    break;
                case 3:
                    langReleaseYear(list);
                    break;
                case 4:
                    printf("Program will exit\n");
                    break;
            }
        }
    } while (choice != 4);

return EXIT_SUCCESS;



}
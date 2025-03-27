
#define _GNU_SOURCE

#include <fcntl.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>


#define PREFIX "movies_"


//same as before
struct movie{
    char *title;
    int year;
    char **languages; //remember arr of pontr to char array
    float rating;
    struct movie *next;
};




struct movie *createMovie(char *currLine)
{
    struct movie *curr_movie = malloc(sizeof(struct movie));

    //the difference is that we only interested in the year
    //not too interested in the others
    char *ptr;

    //the title
    char *token = strtok_r(currLine, ",", &ptr);
    curr_movie->title = calloc(strlen(token) + 1, sizeof(char));
    strcpy(curr_movie->title, token);
  
    //the year
    token = strtok_r(NULL, ",", &ptr);
    int tokeni = atoi(token);
    curr_movie->year = tokeni; 
    
  
    char *ptr2 = ptr;

    //language
    token = strtok_r(NULL, ",", &ptr);
    token++;   
    
    
    curr_movie->languages = malloc(5*sizeof(char*)); 
    for(int i = 0; i < 5; i++){
        curr_movie->languages[i] = NULL;    
    }


    for(int i = 0; i < 5; i++){
        token = strtok_r(token, "[;]", &ptr2);
    
        if(token){
            curr_movie->languages[i] = calloc(strlen(token) + 1, sizeof(char));
            strcpy(curr_movie->languages[i], token);
        }
            
        token = strtok_r(NULL, "]", &ptr2); //cut off the "]"
    }

    //the rating
    token = strtok_r(NULL, "\n", &ptr);
    float tokendb = strtod(token, NULL);
    curr_movie->rating = tokendb; 

  
    curr_movie->next = NULL;

    return curr_movie;
}



//adding it compared to last hw
void list_free(struct movie* m)
{

    struct movie *temp = m;

    while(temp){
        m = temp->next;
        free(temp->title);
        for(int i = 0; i < 5; i++){
            if(temp->languages[i])
                free(temp->languages[i]);
        }
        free(temp->languages);
        free(temp);
        temp = m; 
    }
}


void processFile(char *filePath)
{
    
    int ran_num = random() % 100000; //random number from 0-99999
    char* direc_name = malloc(25*sizeof(char));

    sprintf(direc_name, "aguilary.movies.%d", ran_num); 
    mkdir(direc_name, 0750);
    printf("Created directory with name %s", direc_name);
//using the codes from assignment 1 to do some of the stuff below
//some is used but not all and some is changed
    FILE *movieFile = fopen(filePath, "r");

    char *currLine = NULL;
    size_t len = 0;     
    ssize_t nread;        

    
    struct movie *head = NULL;
    struct movie *tail = NULL;
    
   
    getline(&currLine, &len, movieFile); //skip titles and lables

   
    while ((nread = getline(&currLine, &len, movieFile)) != -1)
    {
        struct movie *newNode = createMovie(currLine);  
        if (head == NULL)
        {  // firdt node then head and tail
            head = newNode;
            tail = newNode;
        }
        else
        {
        // firdt node then head and tail
            tail->next = newNode;
            tail = newNode;
        }
    }
    free(currLine);
    fclose(movieFile);


    struct movie *temp = head;
    char *year_path = malloc(50*sizeof(char));
    char *dir_path = malloc(25*sizeof(char));
    char *movie_title;
    int fd;

    sprintf(dir_path, "./%s", direc_name);
    DIR* currDir = opendir(direc_name);

   
    while(temp){

         
        sprintf(year_path, "%s/%d.txt", direc_name, temp->year);

        
        fd = open(year_path, O_RDWR | O_CREAT | O_APPEND, 0640); 

        
        movie_title = calloc(strlen(temp->title) + 3, sizeof(char));
        sprintf(movie_title, "\n%s", temp->title);

        write(fd, movie_title, strlen(movie_title));
        close(fd);

        temp = temp->next;

        free(movie_title);
    }

    closedir(currDir);

    list_free(head);
    free(year_path);
    free(dir_path);
    free(direc_name);
}

//choose 1 or 2
int chooseOption(){
    
    int choice;

    printf("\n\n1. Select file to process");
    printf("\n2. Exit the program\n");
    printf("\nEnter a choice 1 or 2: ");
    scanf("%d", &choice);

    while(!((choice == 1)||(choice == 2))){
        printf("You entered an incorrect choice. Try again.");
        printf("\n\n1. Select file to process");
        printf("\n2. Exit the program\n");
        printf("\nEnter a choice 1 or 2: ");
        scanf("%d", &choice);
    }
    
    return choice;
}


//choose 1 2 or 3 with the error checker
int chooseFile(){
    
    int choice;

    printf("\n\nWhich file you want to process?");
    printf("\nEnter 1 to pick the largest file");
    printf("\nEnter 2 to pick the smallest file");
    printf("\nEnter 3 to specify the name of a file\n");
    printf("\nEnter a choice from 1 to 3: ");
    scanf("%d", &choice);

    while(!((choice == 1)||(choice == 2) || (choice == 3))){
        printf("You entered an incorrect choice. Try again.");
        printf("\n\nWhich file you want to process?");
        printf("\nEnter 1 to pick the largest file");
        printf("\nEnter 2 to pick the smallest file");
        printf("\nEnter 3 to specify the name of a file\n");
        printf("\nEnter a choice from 1 to 3: ");
        scanf("%d", &choice);
    }
    
    return choice;
}


//option 1 , finding the larger one
char* findLargestFile(){
  DIR* currDir = opendir(".");
  struct dirent *aDir;
  struct stat dirStat;
  int found = 0; 
  int largestSize = 0;
  char* entry = malloc(256*sizeof(char));

  
  while((aDir = readdir(currDir)) != NULL){

    
    if(strncmp(PREFIX, aDir->d_name, strlen(PREFIX)) == 0 && strstr(aDir->d_name, ".csv") != NULL){
      
      
      stat(aDir->d_name, &dirStat);  
    
      
      if(dirStat.st_size > largestSize){
          found = 1;
          largestSize = dirStat.st_size;
          memset(entry, '\0', sizeof(entry));
          strcpy(entry, aDir->d_name);
        }
    }
  }

  closedir(currDir);

  free(aDir);

  if(!found){
    printf("There was no file with the prefix 'movies_' and extension .csv. Select another option.");
    free(entry);
    entry = NULL;
  }

  return entry;
}


//2nd op to smallest file

char* findSmallestFile(){
//basically starting off the same as the prev op
  DIR* currDir = opendir(".");
  struct dirent *aDir;
  struct stat dirStat;
  int found = 0; //acts as boolean
  int smallestSize = __INT_MAX__;
  char* entry = malloc(256*sizeof(char));

  
  while((aDir = readdir(currDir)) != NULL){

    
    if(strncmp(PREFIX, aDir->d_name, strlen(PREFIX)) == 0 && strstr(aDir->d_name, ".csv") != NULL){
      
      
      stat(aDir->d_name, &dirStat);  
    
      
      if(dirStat.st_size < smallestSize){
          found = 1;
          smallestSize = dirStat.st_size;
          memset(entry, '\0', sizeof(entry));
          strcpy(entry, aDir->d_name);
        }
    }
  }
  
  closedir(currDir);
  
  free(aDir);

  if(!found){
    printf("There was no file with the prefix 'movies_' and extension .csv. Select another option.");
    free(entry);
    entry = NULL;
  }

  return entry;
}

//the third option, which is Enter 3 to specify the name of a file
char* findFile(){
  //same
  DIR* currDir = opendir(".");
  struct dirent *aDir;
  struct stat dirStat;
  int found = 0; 
  char* entry = malloc(256*sizeof(char));

  printf("Enter the complete file name: ");
  scanf("%s", entry);

  
  while((aDir = readdir(currDir)) != NULL){

    if(strcmp(entry, aDir->d_name) == 0){
        found = 1;
        memset(entry, '\0', sizeof(entry));
        strcpy(entry, aDir->d_name);
    }
  }
  //same
  closedir(currDir);
  free(aDir);

  if(!found){
    printf("The file %s was not found. Try again.", entry);
    free(entry);
    entry = NULL;
  }
  
  return entry;
}



int main(int argc, char *argv[])
{
    int option;
    int choice;
    char* file;

    srandom(time(0)); 
    option = chooseOption();

    while(option != 2){
        
        do{
            choice = chooseFile();

            if(choice == 1)
                file = findLargestFile();
            else if(choice == 2)
                file = findSmallestFile();
            else if(choice == 3)
                file = findFile();
            
        }while(!file); 
        
                
        printf("Now processing the chosen file named %s\n", file);
        processFile(file);

        free(file);   

        option = chooseOption();
        
    }    

    return EXIT_SUCCESS;
}

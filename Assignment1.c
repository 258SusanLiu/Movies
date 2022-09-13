/* Assignment 1: Movies
 * Started: 9/24/20
 * Finished: 10/11/20
 * Name: Susan Liu
 * Email: liusus@oregonstate.edu
 */
 
 //Note to self..... Remember to read all the directions.
 //ESPECALLY AT THE BOTTOMMMMMMM
 //Otherwise panicing is useless..................
 
 //Assuming that for the user choising which number they will only choose numbers
 //When user chooses languague they will only use letters A-z, a-z the first character of language will be uppercase
 
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*struct for movie information*/
struct movie{
  char *title;
  int year;
  char *language;
  double rating;
  struct movie *next;
};

/* Parse the current line which is space delimited and create a movie struct with data*/
struct movie *createMovie(char *currLine){
    struct movie *currMovie = (struct movie*)malloc(sizeof(struct movie));
    
    //use with strtok_r
    char *saveptr;
    
    //First token is title
    char *token = strtok_r(currLine, ",", &saveptr);
    
    
    currMovie->title = (char*)calloc(strlen(token) + 1, sizeof(char));
    strcpy(currMovie->title, token);

    
    //Next token is year
    token = strtok_r(NULL, ",", &saveptr);
    
    
    //printf("%s\n",token);
    currMovie->year = atoi(token);
    
    
    //Next token is language
    token = strtok_r(NULL, ",", &saveptr);
    currMovie->language = (char*)calloc(strlen(token) + 1, sizeof(char));
    strcpy(currMovie->language, token);
    
    //Final token is rating
    token = strtok_r(NULL, "\n", &saveptr);
    //printf(token);
    currMovie->rating = atof(token);
    //printf("%f/n",token);
        
    //Set next node to null in newly created movie entry
    currMovie->next = NULL; 
    return currMovie;
};

/*Return linked list of movies by parsing data from each line of specific file*/
struct movie *processFile(char *filePath){
  //Open specifield file for reading only
  
  FILE *movieFile = fopen(filePath, "r");

  if(movieFile == 0){
    printf("File did not open\n");
    return 0;
  }
  
  char *currLine = NULL;
  size_t len = 0;
  size_t nread;
  char *token;

  //head of linked list
  struct movie *head = NULL;
  //tail of the linked list
  struct movie *tail = NULL; 

  //throw out first line of the file it is column headings
  getline(&currLine, &len, movieFile);
  
  //read file line by line
  while((nread = getline(&currLine, &len, movieFile)) != -1){
   
    //prints all the movie info line by line   
    //printf("currLine: %s\n", currLine);

    if(strlen(currLine) == 1){
      break;
    }	
    //Get new movie node corresponding to the current line
    struct movie *newNode = createMovie(currLine);
    
    //Is this the first node in the linked list?
    if (head == NULL){
      //This is the first node in linked link
      //Set the head and the tail to this node
      head = newNode;
      tail = newNode;
    }
    else{
      //This is not the first node.
      //Add this node to the list and advance to the tail
      tail->next = newNode;
      tail = newNode;
    }
  }  
  fclose(movieFile);
  return head;
};

/*sort data*/
void sortList(struct movie** head){
  //initilize linked list to be sorted
  struct movie *sort= NULL;
  
  //traverse linked list and insert movie to be sorted (by year)
  struct movie *curr= *head;
  while(curr != NULL){
    //store next movie for next iteration
    struct movie *next = curr->next;
    //insert current into sorted list
    sortInsert(&sort, curr);
    
    //update curr info
    curr = next;
  }
  //reset head to point to sort linked list
  *head = sort;
}

//inserts new movie info into list
void sortInsert(struct movie **head, struct movie *new_list){
  struct movie* curr;
  
  //special case for if head is end
  if(*head == NULL || (*head)->year >= new_list->year){
    new_list->next = *head;
    *head = new_list;
  }
  else{
    //locate node before point of insertion
    curr = *head;
    while(curr->next != NULL && curr->next->year <new_list->year){
      curr = curr->next;
    }
    new_list->next = curr->next;
    curr->next = new_list;
  }
}

/*print linked list that are sorted with highest rating for each year*/
void printList(struct movie *list){
  struct movie *temp = list;
  float original=0;
  int y = list->year;
  
  printf("Showing highest rated movies for each year\n\n"); 
  while(temp != NULL){
    //check if year is same
    if(y == temp->year){
      //compare rating to see which is larger
      if(temp->rating > original){
        original = temp->rating;
        printf("%d  %s  %.2f \n",temp->year,temp->title,original);
      }
    }
    else{
      y++;
    }
    temp = temp->next;
  }
  printf("\n");
  y=list->year;
}

//prints linked list from smalles year to biggest year
/*void printList(struct movie *list){
  struct movie *temp = list;
  
  while(temp != NULL){
    printf("%d  %s  %.2f \n",temp->year,temp->title,temp->rating);
    temp = temp->next;
  }
  printf("\n");
}*/

/*Print the linked list of movies*/
void printMovieList(struct movie *list, int sec){
  //sec is options 1 , 2, 3
  int i = 1;
  int y,z;
  double x;
  int year;
  char lang[15];
  
  //ask for year
  if (sec == 1){ 
    printf("Showing movies released in a specific year.\n");
    printf("Please choose year.\n\n");
    fflush(stdin);
    printf("Year chosen: ");
    scanf("%d", &y);
    printf("\n");
    while(list != NULL){
      //print movies for the year
      if(list->year == y){
        printf("%s\n", list->title);
        i++;
      }
      list = list->next;
    }
    if(list==NULL&& i==1){
      printf("No data about movies released in year %d\n\n",y);
    }
    printf("\n");
    return; 
  }
  //first sort list and then print sorted list with movies with highest ratings each year
  else if (sec == 2){
    sortList(&list);
    printList(list); 
    return;             
  }
  else if (sec == 3){
    int count = 0;
    
    printf("Showing title and year of releace of all movies in specific language\n");
    printf("Please choose language\n\n\n");    
    scanf("%s",&lang);
    printf("You have chosen language: %s\n\n", lang);
    
    while(list != NULL){
      //strstr looks if there is a string inside a string
      //checks if user entered item that is inside list->language
      if(strstr(list->language,lang)){
        printf("%d  %s\n", list->year, list->title);
        list = list->next;
        count++;
      }
      else{
        list = list->next;
      }
    }
    if(count == 0){
      printf("There are no movies in language: %s\n\n\n", lang);
    }
    else{
      printf("\n\n");
    }
  }
  return;
}


int main (int argc, char *argv[]){
  FILE *ptr;
  int num;
  int year;
  int sec;
  char c;
  int counts;
  char filename[30];

  // then count the number of lines in the file
  ptr = fopen(argv[1],"r");
  if(ptr == NULL){
    printf("Error!\n");
  }
  else{ 
    //print out the number of list
    for (c = getc(ptr); c != EOF; c = getc(ptr)){
      if (c == '\n'){
        counts++;
      }
    }
    fclose(ptr);
    
    //How to read the file name? ptr is file 
    printf("Proccesed file %s for %d movies\n", argv[1], counts-1); 
  }
  
  struct movie *list = processFile(argv[1]);
  printf("\n\n");  
  printf("HI! Welcome to Assignment 1: Movies\n");
  printf("We will be using the CVS data from the file.\n\n\n"); 
  
  //continue to ask user what choice they want unless its 4 then end program 
  while(num!=4){
    printf("Choose a number between 1 - 4 please.\n");
    printf("1. Show movies released in the specific year\n");
    printf("2. Show highest rated movie for each year\n");
    printf("3. Show the title and year of release of all movies in a specific language\n");
    printf("4. Exit from program\n\n\n");
    scanf("%d",&num);
    printf("\n\n");
    
    //Show movies released in specific year
    //print the movies in the year, if none print message none
    if(num==1){   
      printMovieList(list,1);
    }
    //show highest rated movie for each year
    //show year, rating, name of movie
    else if(num==2){
      printMovieList(list,counts);
      printMovieList(list,2);
    }
    //Show movies in specific language
    //show year or release and specific language
    else if (num==3){
      printMovieList(list,3);
    }
    
    //if user prints number not 1-4 then ask them to choose again
    else if (num!=4 && num!=2 && num!=2 && num!=1){
      printf("Not a valid choice. Please choose again\n\n\n"); 
    }
  }
  //exit the program
  if (num==4){
    printf("Leaving program. Have a nice Day!\n");
    printf("Goodbye\n\n\n");
  }
  return 0;
}

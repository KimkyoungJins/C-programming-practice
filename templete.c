// Student ID, Name, Honor code

// 나 김경진은 하나님과 사람 앞에서 정직하고 성실하게 코딩테스트를 수행하겠습니다. 

// Your thoughts here!


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX 100
struct st_person{
  char name[20]; 	// Name (single word, no duplicates)
  char gender;  	// 'F' - Female, 'M' - Male
  int country;	// Country code (index of COUNTRY_NAME)
  int group; 		// Group number
};
char COUNTRY_NAME[10][5] = {"KR", "US", "JP", "CN", "KH", "AU", "GB", "FR", "TZ", "CD"};

// functions with complete code
int readFile(struct st_person* p[], char filename[20]); 	// Load from Data file
int askMenu();                                      		// View menu
char* toString(struct st_person* p, int type);      		// Generate string with information of a member
void listAll(struct st_person* p[], int size, int groups);	// List all members
void listGroup(struct st_person* p[], int size, int groups);        // List all members by group
void findName(struct st_person* p[], int size);     		// Find member by name
void reportData(struct st_person* p[], int size, int groups);  	// View the Report
void saveFile(struct st_person* p[], int size); 			// Save to Data file

// Functions with partial code. You must complete these functions.
int addPerson(struct st_person* p[], int size);    			// Add a member
void editPerson(struct st_person* p[], int size);  			// Modify information of a member
int deletePerson(struct st_person* p[], int size); 			// Delete a member
int makeGroup(struct st_person* p[], int size); 			// Re-make groups
void saveReport(struct st_person* p[], int size, int groups); 	// Save to Report file
void saveListGroup(struct st_person* p[], int size, int groups, FILE* file); // Sub function of saveReport() 

int main(void){
  struct st_person* data[MAX];
  int count, groups=1;
  int menu, quit = 0, edited = 1;

  srand(time(0));
  count = readFile(data, "people.txt");
  printf("===== Global Hanst Member Service =====\n");
  printf("> %d people's information loaded!\n", count);

  while (quit == 0){
    menu = askMenu();
    switch (menu){
    case 1:
      listAll(data, count, groups);
      break;
    case 2:
      count = addPerson(data, count);
      edited = 1;
      break;
    case 3:
      editPerson(data, count);
      edited = 1;
      break;
    case 4:
      groups = makeGroup(data, count);
      break;
    case 5:
      findName(data, count);
      break;
    case 6:
      count = deletePerson(data, count);
      edited = 1;
      break;
    case 7:
      reportData(data, count, groups);
      break;
    case 8:
      saveReport(data, count, groups);
      break;
    case 0:
      quit = 1;
      break;
    default:
      quit = 1;
    }
  }
  if (edited == 1){
    int yesno;
    printf("> Do you want to save data file? (1 Yes 0 No) > ");
    scanf("%d", &yesno);
    if (yesno == 1)
      saveFile(data, count);
  }
  printf("> Bye!\n");
  return 0;
}

// 완성 코드로 제공되는 함수
int readFile(struct st_person* p[], char filename[20]){
  int i = 0;
  char name[50];
  char gender;
  int country, group;
  FILE *file;
  file = fopen(filename, "r");
  while (!feof(file)){
    int size = fscanf(file, "%s %c %d %d", name, &gender, &country, &group);
    if (size < 4)	break;
    p[i] = (struct st_person* )malloc(sizeof(struct st_person));
    strcpy(p[i]->name, name);
    p[i]->gender = gender;
    p[i]->country = country;
    p[i]->group = group;
    i++;
  }
  fclose(file);
  return i;
}

int askMenu(){
  int menu;
  printf("\n[ 1:List 2:Add 3:Edit 4:ReGroup 5:Find 6:Delete 7:Report 8:Save 0:Quit ]\n");
  while (1){
    printf("> Enter a menu > ");
    scanf("%d", &menu);
    if (menu >= 0 && menu <= 8)
      break;
    else
      printf("> Wrong number!\n");
  }
  return menu;
}

char* toString(struct st_person* p, int type){
  char temp[100];
  if (type == 1)
    sprintf(temp, "%-10s [%c / %s]", p->name, p->gender, COUNTRY_NAME[p->country]);
  else 
    sprintf(temp, "%-10s [%c / %s] - Group %d", p->name, p->gender, COUNTRY_NAME[p->country], p->group);
  return temp;
}

void listAll(struct st_person* p[], int size, int groups){
  printf("======= List All =======\n");
  int f_count=0, m_count=0;
  printf("There are %d persons with %d groups\n", size, groups);
  for (int i = 0; i < size; i++){
    printf("[%2d] %s\n", i + 1, toString(p[i], 2));
    if(p[i]->gender == 'F') f_count++;
    else m_count++;
  }
  printf("Female %d persons, Male %d persons.\n", f_count, m_count);
}

void listGroup(struct st_person* p[], int size, int groups){
  //printf("======= List by Group =======\n");
  printf("\nThere are %d groups\n", groups);
  for(int g=1; g<=groups; g++){
    int f_count=0, m_count=0;
    printf("\nGroup #%d\n", g);
    for (int i = 0; i < size; i++){
      if(p[i]->group == g){
        printf("- %s\n", toString(p[i], 1));
        if(p[i]->gender == 'F') f_count++;
        else m_count++;
      }
    }
    printf("= Female %d persons, Male %d persons.\n", f_count, m_count);
  }
}

void findName(struct st_person* p[], int size){
  char name[50];
  int count = 0;
  printf("======= Find Name =======\n");
  printf("> Enter the name of person > ");
  scanf("%s", name);
  for (int i = 0; i < size; i++){
    if (strstr(p[i]->name, name)){
      count++;
      printf("> [%d] %s\n", i + 1, toString(p[i], 2));
    }
  }
  printf("%d persons found.\n", count);
}

void reportData(struct st_person* p[], int size, int groups){
  int found, total = 0;
  int count[10] = {0};
  printf("======= Report =======\n");
  int f_count=0, m_count=0;
  for (int i = 0; i < size; i++){
    if(p[i]->gender == 'F') f_count++;
    else m_count++;
    count[p[i]->country]++;
  }
  printf("There are %d persons with %d groups\n", size, groups);
  printf("- Female - %d persons.\n- Male - %d persons.\n", f_count, m_count);
  printf("\nNumber of people per country.\n");
  for(int i=0; i<10; i++){
    printf("[%2d] %s - %d persons\n", i+1, COUNTRY_NAME[i], count[i]);
  }
  listGroup(p, size, groups);
}

void saveFile(struct st_person* p[], int size){
  FILE *file;
  file = fopen("people.txt", "w");
  for (int i = 0; i < size; i++){
    fprintf(file, "%s %c %d %d\n", p[i]->name, p[i]->gender, p[i]->country, p[i]->group);
  }
  printf("> %d people's information saved.\n", size);
  fclose(file);
}

// 직접 구현해야 하는 함수
/*
1. Check if the array is full
2. Allocate memory for structure variable
3. If allocaing memory is failed, print error message and return
4. get the user data such as gender name and country code, group
5. Save the datto to the arry
6. return added number 
*/
int addPerson(struct st_person* p[], int size){

  // check the array is full
  if(size >= MAX){
    printf("The arry is full.\n");
    return size;
  }

  int code = 0;

  // momory alloction
  struct st_person *addedperson = (struct st_person*)malloc(sizeof(struct st_person));

  // if allocating memory is failed
  if(addedperson == NULL){
    printf("Memory allocation failed!!\n");
    return size;
  }
  
  printf("======= Add a new Person =======\n");
  printf("> Enter name, gender(F or M), group number > ");

  //input data from the user
  scanf(" %s", addedperson->name);
  scanf(" %c", &addedperson->gender);
  scanf(" %d", &addedperson->group);


  // nationality
  for(int i=0; i<10; i++)
    printf("%s-%d ", COUNTRY_NAME[i], i);
  printf("\n> Enter a country code > ");
  scanf("%d", &code);
  
  addedperson->country = code;


  // print the result
  printf("[%d] %s  [%c / %s] - group %d\n", size + 1, addedperson->name, addedperson->gender, COUNTRY_NAME[code], addedperson->group);

 // add it to array
  p[size] = addedperson;

  return size + 1;
}

/*
0. set the new variable
1. let user enter the name
2. check the data through strstr function
3. consider if the counts is over 1.
3-2. list the data that contains the character
3-3. ask user to choose the arry number
4. if count == 1 
5. if count 0 -- >not found11
6. edit the data
7. based on scanf function, input the data 
8. if it is stirng, use strcpy function
*/

void editPerson(struct st_person* p[], int size){
  char name[20];
  int count = 0;
  int index = 0;
  int answer = 0;
  int code = 0;

  //memory allocation
  struct st_person *pdata = (struct st_person*)malloc(sizeof(struct st_person));
  
  printf("======= Edit data of a Person =======\n");
  printf("> Enter the name of person > ");
  scanf("%s", name);
  for (int i = 0; i < size; i++){
    if (strstr(p[i]->name, name)){
      count++;
      printf("> [%d] %s\n", i + 1, toString(p[i], 2));
      index = i;
    }
  }

  // if there are over 1
  if(count > 1){
    int num = 0;
    printf("> Enter a number of persons above >\n");
    scanf("%d", &num);

    if(num - 1 >= 0 && num - 1 < size){
      printf("> Enter new name, gender, group(F or N), group number\n");
      scanf(" %s", pdata->name);
      scanf(" %c", &pdata->gender);
      scanf(" %d", &pdata->group);

      //modify the data
      strcpy(p[num-1]->name, pdata->name);
      p[num-1]->gender = pdata->gender;
      p[num-1]->group = pdata->group;
    }

    //Nationality
    for(int i = 0; i <10; i++)
      printf("%s-%d ", COUNTRY_NAME[i], i);
    
      printf("Enter a country code>\n");
      scanf(" %d", &code);
      pdata->country = code;

      //printf the result
      printf("Data Modified.\n");
      
      printf("> [%d] %s\n", num-1, toString(p[num-1], 2));
      
    
    
    
  }
  
  // if there is only one

  if(count == 1){
    // get the edditting data from user
    printf("> Enter new name, gender(F or M), group number\n");
    toString(p[index], 2);
    scanf(" %s", pdata->name);
    scanf(" %c", &pdata->gender);
    scanf(" %d", &pdata->group);

    //modify the data
    strcpy(p[index]->name, pdata->name);
    p[index]->gender = pdata->gender;
    p[index]->group = pdata->group;

    
    //Nationality
    for(int i = 0; i <10; i++)
      printf("%s-%d ", COUNTRY_NAME[i], i);

      printf("Enter a country code>\n");
      scanf(" %d", &code);
      pdata->country = code;

      //printf the result
      printf("Data Modified.\n");

      printf("> [%d] %s\n", index, toString(p[index], 2));

  }
  // if there is nothing
  if (count == 0){
    printf("Not found.\n");
    return;
  }

  // deallocate the momory
    free(pdata);

}

/*
1. Enter the name you want delete
2. through strstr function, find the name
3. if count is over 1
4. if count is 1
5. if count 0 -- >not found
6. delete the data throught for loop 
6-1. by changin the arry
7. set the last element of the arryy NULL;
8. return the new size of the arry

*/

int deletePerson(struct st_person* p[], int size){
  char name[20] = "";
  int count = 0;
  int answer = 0;
  int index = 0;

  
  printf("======= Delete a Person =======\n");
  printf("> Enter the name of person > ");
  scanf("%s", name);

  
  for (int i = 0; i < size; i++){
    if (strstr(p[i]->name, name)){
      count++;
      printf("> [%d] %s\n", i + 1, toString(p[i], 2));
      index = i;
    }
  }

  // if there are over 1
  if(count > 1){
    int num = 0;
    printf("> Enter a number of persons above >\n");
    scanf("%d", &num);
    
     if (num - 1 >= 0 && num - 1 < size)
      {
        printf("%s\n", toString(p[num - 1], 2));
      }

    printf("> confirm of deletion (1 Yes 0 NO) > ");
    scanf(" %d", &answer);

    if(answer == 1){
      for(int i = num-1; i < size; i++){
        p[i] = p[i+1];
      }
      printf("Deleted.\n");
    }

    else if (answer == 0){
      return -1;
    }

  }

  // if there is only one
  if(count == 1){

    printf("> Confirm of deletion (1 Yes 0 No) > \n");
    scanf(" %d", &answer);


    if(answer == 1){
      for(int i = index; i < size; i++){
        p[i] = p[i+1];
      }
      printf("Deleted.\n");
    }

    else if (answer == 0){
      return -1;
    }

  }
    
    // if anyone is not found
    if (count == 0){
    printf("Not found.\n");
    return;
  }
  

  // set the last pointer array is NUll
    p[size] = NULL;


  return size - 1;
}


/*
0. set the random number generator
1. aks user to enter the group number
2. if the group number is over the size or under 0
3. Shuffle the arrary of persons
4. Assign each person to group
4-1. for each group should have similar number of people.
5. print the number of persons in each group
6. print a message indicating the number of groups made
7. return the number of groups

*/

int makeGroup(struct st_person* p[], int size){

  // set the random number generator
  srand(time(NULL));

  printf("======= Make New Groups =======\n");
  int persons, groups, remains, no;
  int assigned[MAX] = {0};

  
  printf("> Enter the number of groups > ");
  scanf("%d", &groups);

  //Check if the number of groups is valid
  if (groups <= 0 || groups > size) {
    
  printf("invalid number of groups.\n");

  }

  // Shuffle the people
  for(int i = size-1; i > 0; i--){
    int j = rand() % (i + 1);
    struct st_person* temp = p[i];
    p[i] = p[j];
    p[j] = temp;
  }

  // assign each person to a group fairly
  for(int i = 0; i <size; i++){
    int group = i % groups;
    p[i]->group = group;
    assigned[group] += 1;
  }


  // prin the number of persons in each group
  for(int i = 0; i < groups; i++){
    printf("Group %d : %d persons\n", i + 1, assigned[i]);
  }

  printf("> %d Groups are made.\n", groups);
  return groups;
}

/*
1. initialize variables for gender and country code
2. get the current time and convert to string
3. Open the file "w" mode and write curren date and time
4. write total number of people and groups
5. increment variables based on gender
6. write gender distribution to file and country as well
7. print the message indicating repoor has been saved.
8. close the file
*/
void saveReport(struct st_person* p[], int size, int groups){
  int found, total = 0;
  int count[10] = {0};
  int f_count=0, m_count=0;
  time_t t = time(NULL)+(9*60*60);
  struct tm tm = *localtime(&t);
  char today[30];
  sprintf(today, "%d/%d/%d %d:%d:%d",
      tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
      tm.tm_hour, tm.tm_min, tm.tm_sec);
  //printf("%s\n", today);
  FILE *file = fopen("report.txt", "w");
  fprintf(file, "Global Hanst Members [%s]\n", today);

  fprintf(file, "\nThere are %d persons with %d groups\n", size, groups);
  for (int i = 0; i < size; i++){
    fprintf(file, "- %s\n", toString(p[i], 2));
    if(p[i]->gender == 'F') f_count++;
    else m_count++;
    count[p[i]->country]++;
  }

  //Add gender distribition
  fprintf(file, "\nGender Distribution:\n");
  fprintf(file, "Female: %d\n", f_count);
  fprintf(file, "Male: %d\n", m_count);

  //Add country distribution
  fprintf(file, "\nCountry Distribution:\n");
  for(int i = 0; i < 10; i++){
    if(count[i] > 0){
      fprintf(file, "%s: %d\n", i, count[i]);
    }
  }




  printf("> Report file (report.txt) saved.\n");
  fclose(file);
}
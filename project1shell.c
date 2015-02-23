/* project1shell.c
 * Sept. 26, 2013
 * Nathan Samano
 * 
 * Write a robust program that operates as a simple shell
 */

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define CHILD 0
#define SIZE 256
#define EOS '\0'

//char hello(char command[SIZE]);
//char prompt(char command[SIZE], char buffer[SIZE]);
//void forkk(char command[SIZE]);

////////////////////////////////////////////////////////////////////////////////////

int main(void) {

/* variable delcarations */
char **command = NULL;        // initialize user command to null
char buffer[SIZE]="";         // initialize a buffer to empty string

int i = 0;                    //
int j = 0;                    // initialize counting variables
int k = 0;                    //

int pid = 0;                  // initialize process id to 0
int childvalue = 0;           // initialize the value of the child to 0
int retval = 0;               // initialize execvp()

////////////////////////////////////////////////////////////////////////////////////
 
    /**** prompt the user with the program ****/
    printf("\n*** Welcome to Nathan's Shell! ***\n\n");

    /**** implements exit and commands ****/
    while(1) /*Alex explained while(1)*/ {
    
       /******** initialize array of strings *******/

       if (command != NULL) { // free any previously allocated memory
          for (i=0; i<SIZE; i++) {  
             free(command[i]); // free each string
          }    
          free(command); // free array of strings
       }

       command = (char**) malloc(sizeof(char*) * SIZE); // array of strings(char*)
       if (command == NULL) { // check return value for error
          printf("Error: Out of memory.\n");
          exit(0); // return; // exit with and error code
       }
       for (i=0; i<SIZE; i++) { // allocate memory for each string
          command[i] = (char*)malloc(sizeof(char)*SIZE);
       
          if (command[i] == NULL) { // check for error
             printf("Error: Empty input.\n");
             return;
          }
       }
   
///////////////////////////////////////////////////////////////////////////////////

       /**** prompt & get user input ****/
       printf("nshell:~$ ");
       fgets(buffer, sizeof(buffer), stdin);
       //getc(stdin);

       /*********** implements exit ************/
       if (strcmp(buffer, "exit\n") == 0) {
          break;
       }       

       /**** prints string for testing ****/
       if (strcmp(buffer, "hello\n") == 0) {
          printf("Hello there!\n");
       //   strcpy(command[0],""); // change to buffer
       } 

      /********** enforce max num char **********/
      if (strlen(buffer) > SIZE) {
         printf("Error: Exceeding max num char.\n");
         strcpy(buffer, "");
      }
      else {

      /*********** fork function ***********/
          for (i=0; i<strlen(buffer); i++) { // handle multiple arguments
             if ((buffer[i] == ' ' || buffer[i] =='\t') && i==0) { // handle leading whitespace
                while (buffer[i] == ' ' || buffer[i] == '\t') {
                   i++;
                }
                command[0][0] = buffer[i];
                if (command[0][0] == '\n') {
                   command[j++][k] = EOS;
                   //command[j] = (char*) NULL;
                }
             }
             if (((buffer[i] == ' ' || buffer[i] == '\t') && (buffer[i+1] == ' ' || buffer[i+1] == '\t'))) { // if char contains consecutive spaces or tabs or if it starts with a space or tab skip and move to the next char (do nothing)
             }
             else {
                if (buffer[i] == ' ' || buffer[i] == '\t' || buffer[i] == '\n') {    // are we at a token separator or end of line?
                   printf("Stuffing element no. %d\n", j);      // enter word separator; start a new string
                   command[j++][k] = EOS;
                   k=0;                                         // start at the beginning
                   if (buffer[i] == '\n') {                     // put NULL at end of string
                      printf("last string is number %d\n", j);
                      if (buffer[i-1] == ' ' || buffer[i-1] == '\t') { // handle trailing whitespace
                         command[j-1] = (char*) NULL;
                      }
                      command[j] = (char*) NULL;  // man execvp said to use char* ahead of NULL
                   }
                }
                else {
                   command[j][k++] = buffer[i]; // takes char
                }
             } //else   
          } //for

          /* print command */
          printf("I am about to print!!!\n");
          for (i=0; command[i]; i++) {
             printf("Element No. %d %s of length %d\n", i, command[i], strlen(command[i]));
          }
   
/* fork process */

       pid = fork();

       if (pid != CHILD) {   /* this is the parent */
          printf("I am the parent.  Ready to wait on the child.\n");
          pid = waitpid(-1, &childvalue, 0);
          printf("Child %d returned a value of %x in hex.\n", pid, childvalue);
          strcpy(command[0],"");     // reset command
          strcpy(buffer, "");        // reset buffer
          i=j=0;                     // back to starting position
          continue;
       }
       else {  /* this is the child */
          printf("I am the child.\n");
          retval = execvp(command[0], command);
          exit(2);
          if (retval != -1) { // error
             printf("Error\n");
             exit(2);
          }
       }

/////////////////////////////////////////////////////////////////////////////////
     
       } // else
    } // while(1)

} // main


///////// Functions (I don't use) //////////

/********* allow user command input *********/
/*
char prompt(char command[SIZE], char buffer[SIZE]) {

       printf("nshell:~$ ");
       fgets(buffer, sizeof(buffer), stdin);
       sscanf(buffer, "%s", command);
}
*/
/************ fork process **************/
/*
void forkk(char command[SIZE]) {
    int pid = 0;
    int childvalue = 0;

    if (strcmp(command, "fork") == 0) {
       
       pid = fork();

       if (pid != CHILD) {   // this is the parent 
           printf("I am the parent.  Ready to wait on the child.\n");
           pid = waitpid(-1, &childvalue, 0);
           printf("Child %d returned a value of %x in hex.\n", pid, childvalue);
           command[0] = ' ';
           return;
       }
       else {  // this is the child 
           printf("I am the child.\n");
           command[0] = ' ';
           exit(2);
       }
    }
}
*/
/************* Hello Test ****************/
/*
char hello(char buffer) {
    if (strcmp(command, "hello") == 0) {
    printf("Hello there!\n");
    command[0] = ' ';
    }
}
 */

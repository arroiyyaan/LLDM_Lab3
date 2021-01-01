/*
*LLDM LAB 3
*Rilwanu Ar Roiyyaan KASNO
*M1 EECIP - EOE Track
*TELECOM SudParis - IP Paris
*Supervised by: Dr. Jose Alfredo Alvarez Aldana
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>

#define MAXLINE 80 //The maximum length command
#define MAXCOM 1000 //max number of letters

#define clear() printf("\033[H\033[J") //shell clearing

char* store;

//shell opening display
void shell_opening() {
  clear();
  printf("\n===LLDM - LABORATORY 3===\n\n");
  char* username = getenv("USER");
  printf("USER: @%s\n", username);
  printf("\n=========================\n\n");
  sleep(1);
  clear();
}

//reading input
int readInput(char* str) { //reading line
  char* buffer;

  buffer = readline("\nosh> ");
  if(strlen(buffer) != 0){ //determining the length of the read line
    add_history(buffer); //add to system history
    strcpy(str, buffer); //copy from buffer to str
    // store = str;
    return 0;
  } else {
    return 1;
  }
}

//to print current work Directory
void printDir() {
  char cwd[1024];
  getcwd(cwd, sizeof(cwd)); //obtaining cwd and size
  printf("\nDirectory: %s", cwd);
}

//builtin command execution
int builtinCmdHandler(char** parsed) {
  int nocmd = 4, i, switchArg = 0;
  char* listcmd[nocmd];
  char* username;
  char* str;
  listcmd[0] = "exit"; listcmd[1] = "cd"; listcmd[2] = "hello";
  listcmd[3] = "history";

  for(i = 0; i < nocmd; i++) {
    if(strcmp(parsed[0], listcmd[i]) == 0) {
      switchArg = i + 1;
      break;
    }
  }

  switch(switchArg) {
  case 1:
    printf("\nGood Bye\n"); //print 'good bye' then exit
    exit(0);
  case 2:
    chdir(parsed[1]); //run chdir()
    return 1;
  case 3:
    username = getenv("USER"); //obtain user by running getenv()
    printf("\nHello %s ", username);
    return 1;
  case 4:
    // puts(store);
    printf("\nchecking history");
  default:
    break;
  }
  return 0;
}

//for finding pipe
int parsePipe(char* str, char** strpiped) {
  int i;
  for(i=0; i<2; i++) {
    strpiped[i] = strsep(&str, "|");
    if(strpiped[i] == NULL)
      break;
  }

  if(strpiped[1] == NULL)
    return 0; //if no pipe found
  else {
    return 1;
  }
}

//command words parsing
void parseSpace(char* str, char** parsed) {
  int i;
  for(i=0; i<MAXLINE; i++) {
    parsed[i] = strsep(&str, " "); //separating space character

    if(parsed[i] == NULL)
      break;
    if(strlen(parsed[i]) == 0)
      i--;
  }
}

//string processing
int stringProcess(char* str, char** parsed, char** parsedpipe) {
  char* strpiped[2];
  int piped = 0;

  piped = parsePipe(str, strpiped);

  if(piped) {
    parseSpace(strpiped[0], parsed);
    parseSpace(strpiped[1], parsedpipe);
  } else {
    parseSpace(str, parsed);
  }

  if(builtinCmdHandler(parsed))
    return 0;
  else
    return 1 + piped;
}

int main(void) {
    char inputString[MAXCOM];
    char* parsedArgsPiped[MAXLINE];
    char *args[MAXLINE / 2 + 1]; //command line arguments
    int should_run = 1; //flag to determine when to exit program

    //shell opening display
    shell_opening();

    while (should_run) {
        /*(A) printing current directory*/
        printDir();
        /*(B) reading input*/
        readInput(inputString);

        //parsing process
        stringProcess(inputString, args, parsedArgsPiped);

        /*After reading user input, the steps are:*/
        /*(1) fork a child process using fork()*/
        pid_t pid = fork();

        /*(2) the child process will invoke execvp()*/
        if(pid == -1) {
          printf("\nOops..failed to fork child");
        } else if(pid == 0) {
          execvp(args[0], args);
          exit(0);
        } else {
          wait(NULL);
        }
    }
    return 0;
}

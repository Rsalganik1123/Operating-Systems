#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include <wordexp.h>
#include<dos.h>


void writeFIFO(char *line, char *address) 
{
     
     //wordexp will fail if it gets a \n so i'm just taking it out
     //and setting it to NULL
     int length = strlen(line);
     line[length-1] = '\0';

     //initialize var wordexp_t p to hold an array of string that is delimited
     wordexp_t p;
     wordexp(line, &p, 0);

     pid_t  pid;
     int    status;
     
     
     if ((pid = fork()) < 0) {    //forking to child proc

          printf("forking child process failed\n");
          exit(1);
     } else if (pid == 0) {         //when child created, pid = 0 
          //printf("%s - [%s]\n", address, line);
          //fflush(stdout);
          close(1);
          if(open(address, O_WRONLY) <0) {
               perror("pipe open failed"); 
               exit(0);
          } 
          if (execvp(p.we_wordv[0], p.we_wordv) <0)  {     /* execute the command  */
               perror("exec failed\n");
               exit(1);
          }

     } else {                                  /* for the parent:      */
          while (wait(&status) != pid)       /* wait for completion  */
               ;
     }
}

void readFIFO(char *line, char *address)
{
	
     //wordexp will fail if it gets a \n so i'm just taking it out
     //and setting it to NULL
     int length = strlen(line);
     line[length-1] = '\0';

     //initialize var wordexp_t p to hold an array of string that is delimited
     wordexp_t p;
     wordexp(line, &p, 0);

     pid_t  pid;
     int    status;
     
     
     if ((pid = fork()) < 0) {    //forking to child proc

          printf("forking child process failed\n");
          exit(1);
     }
     else if (pid == 0) {         //when child created, pid = 0 
          //printf("%s - [%s]\n", address, line);
          //fflush(stdout);
          close(0);

          if(open(address, O_RDONLY) <0) {
               perror("pipe open failed"); 
          } 
          if (execvp(p.we_wordv[0], p.we_wordv) <0)  {     /* execute the command  */
               perror("exec failed\n");
               exit(1);
          }

     }
     else {                                  /* for the parent:      */
          while (wait(&status) != pid)       /* wait for completion  */
               ;
     }
}

int  main(int argc, char *argv[])
{
    
     char *line= malloc(512); 
     if (line == NULL){
          perror("Malloc failed\n");
     }

    char *address = malloc(512); 
     if (address == NULL){
          perror("Malloc failed\n");
     }
     

     address = argv[2]; 
     while(1){
     
          if (fgets(line, 512, stdin) == NULL) break; 
         
          if (strcmp(line, "exit") == 0) {
               exit(0);
          }
          
          
          //*command++ = NULL;
          if (strcmp((argv[1]), "R") == 0) {
               //readFIFO(line, fd);
               //printf("reading\n"); 
               readFIFO(line, address);
          }
          if (strcmp((argv[1]), "W") == 0) {
               //writeFIFO(line, fd);
               // printf("writing\n");
               writeFIFO(line, address);
          }
     }
     
     
     return (0); 
}

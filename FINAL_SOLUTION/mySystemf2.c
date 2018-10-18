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
#include <time.h>




double gettime(){
     struct timespec tp; 
     clock_gettime(CLOCK_REALTIME, &tp); 
     double x = tp.tv_sec * 1000000.0; 
     x = x + tp.tv_nsec/1000.0; 
     return x; 
}

void  execute(char *line)
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
     
     double x = gettime();  
     if ((pid = fork()) < 0) {    //forking to child proc

          printf("forking child process failed\n");
          exit(1);
     }
     else if (pid == 0) {         //when child created, pid = 0 

          if (execvp(p.we_wordv[0], p.we_wordv) <0)  {     /* execute the command  */
               perror("exec failed\n");
               exit(1);
          }
           
     }
     else { 
                                           /* for the parent:      */
          while (wait(&status) != pid)       /* wait for completion  */
               ;
          double y = gettime(); 
          printf("%e\n", (y-x) );
     }
}

int  main(int argc, char *argv[])
{
    
     char *line= malloc(512); 
     if (line == NULL){
          perror("Malloc failed\n");
     }
     //char *command[32];

     
     //printf("Timing is: %d : %d: %d\n", t.ti_hour, t.ti_min, t.ti_sec);
    
     while(1){
     
     	if (fgets(line, 512, stdin) == NULL) break; 
         
          //if (strcmp(argv[1], "exit") == 0) {
          //     exit(0);
          //}
          
          
          //*command++ = NULL; 
     	execute(line); //execute(command);

     }
     
     
     return (0); 
}
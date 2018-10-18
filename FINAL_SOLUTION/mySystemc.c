#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <linux/sched.h>
#include <signal.h>
#include <wordexp.h>
#include <ctype.h>
#include <fcntl.h>
#include <time.h>

double gettime(){
     struct timespec tp; 
     clock_gettime(CLOCK_REALTIME, &tp); 
     double x = tp.tv_sec * 1000000.0; 
     x = x + tp.tv_nsec/1000.0; 
     return x; 
}
int clone_function( void* arg ){
     char *line = (char*) arg;
     //wordexp will fail if it gets a \n so i'm just taking it out
     //and setting it to NULL
     int length = strlen(line);
     line[length-1] = '\0';

     //initialize var wordexp_t p to hold an array of string that is delimited
     wordexp_t p;
     wordexp(line, &p, 0);

     //this version can execute cd 
     execl("/bin/sh", "sh", "-c", line, (char*) 0);

     //if (execvp(p.we_wordv[0], p.we_wordv) <0) {
     //    perror("exec failed \n");
     //     exit(1);
     //}

}

int main(int argc, char* argv[]){
        char *line= malloc(512);
        if (line == NULL){
          perror("Malloc failed\n");
        }


        void* clone_process_stack = malloc(8192);
        void* stack_top = clone_process_stack + 8192;
        int clone_flags = CLONE_VFORK | CLONE_FS ;
        double x = gettime(); 
        while(1){
          //get line from file or stdin
          if(fgets(line,512,stdin) == NULL) break;
          //call clone 
          int child_pid = clone( &clone_function, stack_top, clone_flags | SIGCHLD , (void*) line);

          int status;
          //wait for child to terminate
          wait(&status);
          double y = gettime(); 
          printf("%e\n", (y-x) );
      
          }
     return (0); 
}






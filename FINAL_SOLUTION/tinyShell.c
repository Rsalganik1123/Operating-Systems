#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>





int  main(int argc, char *argv[])
{
    
     char *line= malloc(512); 
     if (line == NULL){
          perror("Malloc failed\n");
     }
    

     while(1){
     
     	if (fgets(line, 512, stdin) == NULL) break; 
         
       
        int length = strlen(line);
    	line[length-1] = '\0';

    	if (length > 1 ) {
			system(line); 
		}
		
		break;         

     }	
     return 0;
}
	


	
		

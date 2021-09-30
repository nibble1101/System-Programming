/***** LAB3 base code *****/ 
#include"helper.h"

int main(int argc, char *argv[ ], char *env[ ])
{
  int j = 0;
  int pid, status;
  char *s, *pipedLine[100];
  char line[28], tempLine[28], partialLine[28];


  // The base code assume only ONE dir[0] -> "/bin"
  // YOU do the general case of many dirs from PATH !!!!
  home = getenv("HOME");
  dir[0] = "/bin";
  ndir   = 1;
  // show dirs
  for(int i=0; i<ndir; i++)
    printf("dir[%d] = %s\n", i, dir[i]);
  
  while(1)
  {
    memset(line, 0, sizeof(line));
    j = 0;
    printf("sh %d running\n", getpid());
    printf("enter a command line : ");
    fgets(line, 128, stdin);
    line[strlen(line) - 1] = 0;
    if (line[0]==0)
      continue;
    
    strcpy(tempLine, line);


    s = strtok(tempLine, "|");
    while(s)
    {
        if(s[0] == ' ')
            memmove(s,s+1, strlen(s));
        if(s[strlen(s)-1] == ' ')
            s[strlen(s)-1] = '\0';
        pipedLine[j] = s;
        j++;
        //printf("%s",s);
        s = strtok(0, "|");
    }

    if(j == 1)
    {
        //s = NULL;
        if(strcmp(tempLine,"a.out") == 0)
            execl("/home/ritik/Desktop/cpts360/Lab3/a.out","/home/ritik/Desktop/cpts360/Lab3/a.out",NULL);
        s = strtok(tempLine, ";");
        int i = 0;
        while(s)
        {
            if(s[0] == ' ')
                memmove(s,s+1, strlen(s));
            strcpy(partialLine, s);
            s = strtok(0, ";");
            executeSingleCommand(&partialLine[0], env);
        }
        
    }
    else
    {
        for(int i=0; i<j-1; i++)
        {
            executePipelinedCommand(pipedLine[i], pipedLine[i+1], env);
        }
    }

  }
}

/********************* YOU DO ***********************
1. I/O redirections:

Example: line = arg0 arg1 ... > argn-1

  check each arg[i]:
  if arg[i] = ">" {
     arg[i] = 0; // null terminated arg[ ] array 
     // do output redirection to arg[i+1] as in Page 131 of BOOK
  }
  Then execve() to change image


2. Pipes:

Single pipe   : cmd1 | cmd2 :  Chapter 3.10.3, 3.11.2

Multiple pipes: Chapter 3.11.2
****************************************************/

    

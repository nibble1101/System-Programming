#ifndef HELPER_H
#define HELPER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>

char gpath[128];    // hold token strings 
char *arg[64];      // token string pointers
int  n;             // number of token strings

char dpath[128];    // hold dir strings in PATH
char *dir[64];      // dir string pointers
int  ndir;          // number of dirs
char *home;         // stores the home directory path

int tokenize(char *pathname) // YOU have done this in LAB2
{                            // YOU better know how to apply it from now on
  char *s;
  strcpy(gpath, pathname);   // copy into global gpath[]
  s = strtok(gpath, " ");    
  n = 0;
  while(s){
    arg[n++] = s;           // token string pointers   
    s = strtok(0, " ");
  }
  arg[n] =0;                // arg[n] = NULL pointer 
}

void executeSingleCommand(char *pathname, char* env[])
{
    tokenize(pathname);
    int pid, status;
    char line[28];
    char *cmd;

    for (int i=0; i<n; i++){  // show token strings   
        printf("arg[%d] = %s\n", i, arg[i]);
    }
    //getchar();
    

    cmd = arg[0];         // line = arg0 arg1 arg2 ...
    

    if (strcmp(cmd, "cd")==0){

        chdir(arg[1]);
        return;
    }
    if (strcmp(cmd, "exit")==0)
      exit(0); 

    int redirect = 0;
    int rediri = 0;
    int i = 0;
    while(i<n)
    {

        if(strcmp(arg[i], "<") == 0)
        {
            redirect = 1;
            rediri = i;
        }
            
        else if(strcmp(arg[i], ">") == 0)
        {
            redirect = 2;
            rediri = i;
        }
        else if(strcmp(arg[i], ">>") == 0)
        {
            redirect = 3;
            rediri = i;
        }
        i++;
    }

    if(!rediri)
    {
        arg[i] = NULL;
    }
    else
    {
        arg[rediri] = NULL;
    }
        

    pid = fork();
     
    if (pid)
    {
       printf("sh %d forked a child sh %d\n", getpid(), pid);
       printf("sh %d wait for child sh %d to terminate\n", getpid(), pid);
       pid = wait(&status);
       printf("ZOMBIE child=%d exitStatus=%x\n", pid, status); 
       printf("main sh %d repeat loop\n", getpid());
     }
     else{
       printf("child sh %d running\n", getpid());


       if(redirect == 1)
       {
           close(0);
           open(arg[rediri+1], O_RDONLY);
       }
       else if(redirect == 2)
       {
           close(1);
           open(arg[rediri+1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
       }
       if(redirect == 3)
       {
           close(1);
           open(arg[rediri+1], O_WRONLY | O_APPEND | O_CREAT, 0644);
       }
       
       // make a cmd line = dir[0]/cmd for execve()
       strcpy(line, dir[0]); strcat(line, "/"); strcat(line, cmd);
       printf("line = %s\n", line);

       int r = execve(line, arg, env);

       printf("execve failed r = %d\n", r);
       exit(1);
     }
}


void executePipelinedCommand(char *partialLineA, char* partialLineB, char* env[])
{
    int pid, statusA, statusB, pd[2];
    char line[28], *x;
    char *cmdA, *cmdB;
    char* argA[64], *argB[64];
    tokenize(partialLineA);
    for(int i = 0; i<n; i++)
    {
        x = (char*)malloc(sizeof(*arg[i]));
        strcpy(x,arg[i]);
        argA[i] = x;
    }
    for (int i=0; i<n; i++){  // show token strings   
        printf("argA[%d] = %s\n", i, argA[i]);
    }
    //cmdA = arg[0];

    int redirectA = 0;
    int rediriA = 0;
    int i = 0;
    while(i<n)
    {

        if(strcmp(argA[i], "<") == 0)
        {
            redirectA = 1;
            rediriA = i;
        }
            
        else if(strcmp(argA[i], ">") == 0)
        {
            redirectA = 2;
            rediriA = i;
        }
        else if(strcmp(argA[i], ">>") == 0)
        {
            redirectA = 3;
            rediriA = i;
        }
        i++;
    }
    if(!rediriA)
    {
        argA[i] = NULL;
    }
    else
    {
        argA[rediriA] = NULL;
    }
    
    tokenize(partialLineB);
    for(int i = 0; i<n; i++)
    {
        x = (char*)malloc(sizeof(*arg[i]));
        strcpy(x,arg[i]);
        argB[i] = x;
    }
    printf("\n");
    for (int i=0; i<n; i++){  // show token strings   
        printf("argB[%d] = %s\n", i, argB[i]);
    }
    //getchar();

    int redirectB = 0;
    int rediriB = 0;
    i = 0;
    while(i<n)
    {

        if(strcmp(argB[i], "<") == 0)
        {
            redirectB = 1;
            rediriB = i;
        }
            
        else if(strcmp(argB[i], ">") == 0)
        {
            redirectB = 2;
            rediriB = i;
        }
        else if(strcmp(argB[i], ">>") == 0)
        {
            redirectB = 3;
            rediriB = i;
        }
        i++;
    }
    if(!rediriB)
    {
        argB[i] = NULL;
    }
    else
    {
        argB[rediriB] = NULL;
    }
    

    cmdA = argA[0];         // line = arg0 arg1 arg2 ...
    cmdB = argB[0];
        
    pipe(pd);
    pid = fork();
    if (pid)
    {
        close(pd[0]);
        close(1);
        dup(pd[1]);
        close(pd[1]);
       if(redirectA == 1)
       {
           close(0);
           open(argA[rediriA+1], O_RDONLY);
       }
       else if(redirectA == 2)
       {
           close(1);
           open(argA[rediriA+1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
       }
       if(redirectA == 3)
       {
           close(1);
           open(argA[rediriA+1], O_WRONLY | O_APPEND | O_CREAT, 0644);
       }
       
       // make a cmd line = dir[0]/cmd for execve()
       strcpy(line, dir[0]); strcat(line, "/"); strcat(line, cmdA);
       printf("line = %s\n", line);

       int r = execve(line, argA, env);

       printf("execve failed r = %d\n", r);
       //exit(1);
     }
     else{
       close(pd[1]);
       close(0);
        dup(pd[0]);
        close(pd[0]);
        
        //pid = wait(&statusA);
       if(redirectB == 1)
       {
           close(0);
           open(argB[rediriB+1], O_RDONLY);
       }
       else if(redirectB == 2)
       {
           close(1);
           open(argB[rediriB+1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
       }
       if(redirectB == 3)
       {
           close(1);
           open(argB[rediriB+1], O_WRONLY | O_APPEND | O_CREAT, 0644);
       }
       
       // make a cmd line = dir[0]/cmd for execve()
       strcpy(line, dir[0]); strcat(line, "/"); strcat(line, cmdB);
       printf("line = %s\n", line);

       int r = execve(line, argB, env);

       printf("execve failed r = %d\n", r);

     }
}

#endif

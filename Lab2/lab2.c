#include <stdio.h>             // for I/O
#include <stdlib.h>            // for I/O
#include <libgen.h>            // for dirname()/basename()
#include <string.h>

typedef struct node{
         char  name[64];       // node's name string
         char  type;           // 'D' for DIR; 'F' for file
   struct node *child, *sibling, *parent;
}NODE;

typedef struct stack{
  struct stack *prev;
  char name[64];

}Stack;

FILE *fp;


NODE *root, *cwd, *start;
char line[128];
char command[16], pathname[64], directoryname[32];

//                0         1      2    3     4       5       6      7        8         9       10
char *cmd[] = {"mkdir", "rmdir", "ls", "cd", "pwd", "creat", "rm", "reload", "save", "menu", "quit", 0};



int findCmd(char *command)
{
   int i = 0;
   while(cmd[i]){
     if (strcmp(command, cmd[i])==0)
         return i;
     i++;
   }
   return -1;
}

NODE *search_child(NODE *parent, char *name)
{
  NODE *p;
  printf("search for %s in parent DIR\n", name);
  p = parent->child;
  if (p==0)
    return 0;
  while(p){
    if (strcmp(p->name, name)==0)
      return p;
    p = p->sibling;
  }
  return 0;
}

int insert_child(NODE *parent, NODE *q)
{
  NODE *p;
  printf("insert NODE %s into END of parent child list\n", q->name);
  p = parent->child;
  if (p==0)
    parent->child = q;
  else{
    while(p->sibling)
      p = p->sibling;
    p->sibling = q;
  }
  q->parent = parent;
  q->child = 0;
  q->sibling = 0;
}


NODE* getDirectory(NODE *startPtr, char path_name[64])
{
  const char s[2] = {"/"};
  char *token = strtok(path_name, s);
  int flag = 0;
  NODE *ptr = NULL;
  while(token!= NULL)
  {
    ptr = search_child(startPtr, token);
    //printf("\n\nThe directory returned: %s\n",ptr->name);
    if(ptr == NULL || ptr->type == 'F')
    {
      flag=1;
      break;
    }
    else
      startPtr = ptr;
      token = strtok(NULL,s);
  }
  if(flag == 1)
  {
    return NULL;
  }
    
  else
  {
    return startPtr;
  }
}


/***************************************************************
 This mkdir(char *name) makes a DIR in the current directory
 You MUST improve it to mkdir(char *pathname) for ANY pathname
****************************************************************/

int mkdir(char path_name[64])
{
  NODE *p, *q;
  printf("mkdir: pathname=%s\n", path_name);

  if (!strcmp(path_name, "/") || !strcmp(path_name, ".") || !strcmp(path_name, "..")){
    printf("can't mkdir with %s\n", path_name);
    return -1;
  }

  if(strrchr(path_name,'/') != NULL)
  {
    strcpy(directoryname,strrchr(path_name,'/'));
    pathname[strlen(path_name)-strlen(directoryname)] = '\0';
    memmove(directoryname, directoryname+1, strlen(directoryname));
    if (path_name[0]=='/')
    {
      
      start = getDirectory(root,pathname);
      //printf("\n\nThe directory returned: %s",start->name);
      if(start == NULL)
      {
        printf("\nThe directory path not found.\n");
        return -1;
      }
    }
    else
    {
      start = getDirectory(cwd,pathname);
      if(start == NULL)
      {
        printf("\nThe directory path not found.\n");
        return -1;
      }
    }
  }
  else
  {
    strcpy(directoryname,path_name);
    start = cwd;
  }
  
  printf("check whether %s already exists\n", directoryname);
  p = search_child(start, directoryname);
  if (p){
    printf("name %s already exists, mkdir FAILED\n", directoryname);
    return -1;
  }
  printf("--------------------------------------\n");
  printf("ready to mkdir %s\n", directoryname);
  q = (NODE *)malloc(sizeof(NODE));
  q->type = 'D';
  strcpy(q->name, directoryname);
  insert_child(start, q);
  printf("mkdir %s OK\n", directoryname);
  printf("--------------------------------------\n");
    
  return 0;
}


int creat(char path_name[64])
{
  NODE *p, *q;
  printf("creat: pathname=%s\n", path_name);

  if (!strcmp(path_name, "/") || !strcmp(path_name, ".") || !strcmp(path_name, "..")){
    printf("can't creat with %s\n", path_name);
    return -1;
  }

  if(strrchr(path_name,'/') != NULL)
  {
    strcpy(directoryname,strrchr(path_name,'/'));
    pathname[strlen(path_name)-strlen(directoryname)] = '\0';
    memmove(directoryname, directoryname+1, strlen(directoryname));
    if (path_name[0]=='/')
    {
      
      start = getDirectory(root,pathname);
      //printf("\n\nThe directory returned: %s",start->name);
      if(start == NULL)
      {
        printf("\nThe file path not found.\n");
        return -1;
      }
    }
    else
    {
      start = getDirectory(cwd,pathname);
      if(start == NULL)
      {
        printf("\nThe file path not found.\n");
        return -1;
      }
    }
  }
  else
  {
    strcpy(directoryname,path_name);
    start = cwd;
  }
  
  printf("check whether %s already exists\n", directoryname);
  p = search_child(start, directoryname);
  if (p){
    printf("name %s already exists, creat FAILED\n", directoryname);
    return -1;
  }
  printf("--------------------------------------\n");
  printf("ready to creat %s\n", directoryname);
  q = (NODE *)malloc(sizeof(NODE));
  q->type = 'F';
  strcpy(q->name, directoryname);
  insert_child(start, q);
  printf("creat %s OK\n", directoryname);
  printf("--------------------------------------\n");
    
  return 0;
}


// This ls() list CWD. You MUST improve it to ls(char *pathname)
int ls()
{
  NODE *p = cwd->child;
  printf("cwd contents = ");
  while(p){
    printf("[%c %s] ", p->type, p->name);
    p = p->sibling;
  }
  printf("\n");
}





int initialize()
{
    root = (NODE *)malloc(sizeof(NODE));
    strcpy(root->name, "/");
    root->parent = root;
    root->sibling = 0;
    root->child = 0;
    root->type = 'D';
    cwd = root;
    printf("Root initialized OK\n");
}

int cd(char* path_name)
{
  NODE* ptr = NULL;
  if(strcmp(path_name, "..") == 0)
  {
    cwd = cwd->parent;
    printf("\nCWD changed to: %s\n", cwd->name);
  }
  else
  {
    ptr = getDirectory(cwd, path_name);
    if(ptr != NULL && ptr->type!='F')
    {
     cwd = ptr;
     printf("\nCWD changed to: %s\n", cwd->name);
    }
    else
    {
      printf("\nThe directory cannot be changed.\n");
    }
  }
}

void pwd(NODE* ptr)
{
  
  if(strcmp(ptr->name,"/") == 0)
  {
    return;
  }
    
  else
  {
    char filename[32];
    strcpy(filename,ptr->name);
    pwd(ptr->parent);
    printf("/%s",filename);
  }
}





int rmdir(char *path_name)
{
  NODE *p = NULL, *q = NULL;
  printf("rmdir: pathname=%s\n", path_name);

  if(strrchr(path_name,'/') != NULL) // Checking if the path is entered
  {
    strcpy(directoryname,strrchr(path_name,'/'));
    pathname[strlen(path_name)-strlen(directoryname)] = '\0';
    memmove(directoryname, directoryname+1, strlen(directoryname));
    if (path_name[0]=='/') // If the path is absolute
    {
      
      start = getDirectory(root,pathname); //Getting the parent directory
      if(start == NULL)
      {
        printf("\nThe directory path not found.\n");
        return -1;
      }
      p = start->child;
      if(p == NULL)
      {
        printf("\nThe directory path not found.\n");
        return -1;
      }
      else
      {
        if(strcmp(p->name,directoryname) == 0 && p->child == NULL && p->type == 'D') //If parent has MULTIPLE child [W]
        {
          if(p->sibling != NULL)    
          {
            p->parent->child = p->sibling;
            p->sibling = NULL;
            free(p);
            return 1;
          }
          else    //If parent has only ONE child [W]
          {
            p->parent->child = NULL;    
            free(p);
          }
        }
        else
        {
          while(p!=NULL && strcmp(p->name, directoryname) != 0) //Deleting from CWD and MIDDLE sibling [W]
          {
            q = p;
            p = p->sibling;
          }
          if(p == NULL)
            return -1;
          if(p->child == NULL && p->type == 'D')
          {
            q->sibling = p->sibling;
            free(p);
            return 1;
          }
        }
      }
    }
    else
    {
      start = getDirectory(cwd,pathname);
      if(start == NULL)
      {
        printf("\nThe directory path not found.\n");
        return -1;
      }

      p = start->child;
      if(p == NULL)
      {
        printf("\nThe directory path not found.\n");
        return -1;
      }
      else
      {
        if(strcmp(p->name,directoryname) == 0 && p->child == NULL && p->type == 'D')
        {
          if(p->sibling != NULL)
          {
            p->parent->child = p->sibling;
            p->sibling = NULL;
            free(p);
            return 1;
          }
          else
          {
            p->parent->child = NULL;
            free(p);
            return 1;
          }
        }
        else
        {
          while(p!=NULL && strcmp(p->name, directoryname) != 0)
          {
            q = p;
            p = p->sibling;
          }
          if(p == NULL)
            return -1;
          if(p->child == NULL && p->type == 'D')
          {
            q->sibling = p->sibling;
            free(p);
          }
        }
      }
    }
  }
  else
  {
    strcpy(directoryname,path_name);
    printf("Directory name: %s", directoryname);
    start = cwd;
    p = start->child;
    if(strcmp(p->name, directoryname)==0 && p->child == NULL && p->type == 'D')
    {
      if(p->sibling != NULL) // If CWD has MULTIPLE child and deleting FIRST child [W]
      {
        start->child = p->sibling;
        p->sibling = NULL;
        free(p);
      }
      else //If CWD has only ONE child [W]
      {
        p->parent->child = NULL;
        free(p);
      }
    }
    else
    {
      while(p!=NULL && strcmp(p->name, directoryname) != 0) //Deleting from CWD and MIDDLE sibling [W]
      {
        q = p;
        p = p->sibling;
      }
      if(p == NULL)
        return -1;
      if(p->child == NULL && p->type == 'D')
      {
        q->sibling = p->sibling;
        free(p);
      } 
    }
  }
    
  return 0;
}


int rm(char* path_name)
{
  NODE *p = NULL, *q = NULL;
  printf("rm: pathname=%s\n", path_name);

  if(strrchr(path_name,'/') != NULL) // Checking if the path is entered
  {
    strcpy(directoryname,strrchr(path_name,'/'));
    pathname[strlen(path_name)-strlen(directoryname)] = '\0';
    memmove(directoryname, directoryname+1, strlen(directoryname));
    if (path_name[0]=='/') // If the path is absolute
    {
      
      start = getDirectory(root,pathname); //Getting the parent directory
      if(start == NULL)
      {
        printf("\nThe directory path not found.\n");
        return -1;
      }
      p = start->child;
      if(p == NULL)
      {
        printf("\nThe directory path not found.\n");
        return -1;
      }
      else
      {
        if(strcmp(p->name,directoryname) == 0 && p->type == 'F') //If parent has MULTIPLE child [W]
        {
          if(p->sibling != NULL)    
          {
            p->parent->child = p->sibling;
            p->sibling = NULL;
            free(p);
            return 1;
          }
          else    //If parent has only ONE child [W]
          {
            p->parent->child = NULL;    
            free(p);
          }
        }
        else
        {
          while(p!=NULL && strcmp(p->name, directoryname) != 0) //Deleting from CWD and MIDDLE sibling [W]
          {
            q = p;
            p = p->sibling;
          }
          if(p == NULL)
            return -1;
          if(p->type == 'F')
          {
            q->sibling = p->sibling;
            free(p);
            return 1;
          }
        }
      }
    }
    else
    {
      start = getDirectory(cwd,pathname);
      if(start == NULL)
      {
        printf("\nThe directory path not found.\n");
        return -1;
      }

      p = start->child;
      if(p == NULL)
      {
        printf("\nThe directory path not found.\n");
        return -1;
      }
      else
      {
        if(strcmp(p->name,directoryname) == 0 && p->type == 'F')
        {
          if(p->sibling != NULL)
          {
            p->parent->child = p->sibling;
            p->sibling = NULL;
            free(p);
            return 1;
          }
          else
          {
            p->parent->child = NULL;
            free(p);
            return 1;
          }
        }
        else
        {
          while(p!=NULL && strcmp(p->name, directoryname) != 0)
          {
            q = p;
            p = p->sibling;
          }
          if(p == NULL)
            return -1;
          if(p->type == 'F')
          {
            q->sibling = p->sibling;
            free(p);
          }
        }
      }
    }
  }
  else
  {
    strcpy(directoryname,path_name);
    printf("Directory name: %s", directoryname);
    start = cwd;
    p = start->child;
    if(strcmp(p->name, directoryname)==0 && p->type == 'F')
    {
      if(p->sibling != NULL) // If CWD has MULTIPLE child and deleting FIRST child [W]
      {
        start->child = p->sibling;
        p->sibling = NULL;
        free(p);
      }
      else //If CWD has only ONE child [W]
      {
        p->parent->child = NULL;
        free(p);
      }
    }
    else
    {
      while(p!=NULL && strcmp(p->name, directoryname) != 0) //Deleting from CWD and MIDDLE sibling [W]
      {
        q = p;
        p = p->sibling;
      }
      if(p == NULL)
        return -1;
      if(p->type == 'F')
      {
        q->sibling = p->sibling;
        free(p);
      } 
    }
  }
    
  return 0;
}


void writeFile(NODE* node)
{
  if(node == NULL)
  {
    printf("1 Got executed");
    return;
  }
    
  fprintf(fp, "%c\t", node->type);
  
  NODE* temp = node;
  Stack *top = malloc(sizeof(Stack)), *pusher;
  strcpy(top->name, temp->name);
  top->prev=NULL;

  while(temp != root)
  {
    temp = temp->parent;
    if(temp != NULL)
    {
      pusher = malloc(sizeof(Stack));
      strcpy(pusher->name, temp->name);
      pusher->prev = top;
      top = pusher;
    }
  }

  top = top->prev;

  while(top != NULL)
  {
    fprintf(fp,"/%s",top->name);
    top = top->prev;
  }
  fprintf(fp,"\n");
  printf("\n");
  if(node->child != NULL)
  {
    writeFile(node->child);
  }
  if(node->sibling != NULL)
  {
    writeFile(node->sibling);
  }

}

int save()
{
  fp = fopen("myfile", "w+");
  if(fp == NULL)
  {
    printf("\nCannot access the file. \n");
    return 0;
  }
  writeFile(root->child);
  fclose(fp);
}

int quit()
{
  save();
  printf("Program exit\n");
  exit(0);
}

void reload()
{
  fp = fopen("myfile", "r");
  if(fp == NULL)
  {
    printf("\nCannot access the file. \n");
    return;
  }
  char type, l[100];
  while(1)
  {
    fgets(l, 100, fp);
    if(feof(fp))
      break;

    l[strlen(l)-1] = '\0';

    if(l != NULL || strcmp(l, "") != 0)
    {

      char *token = strtok(l, "\t");
      type = *token;
      token = strtok(NULL, "\t");
      strcpy(pathname, token);
      switch(type)
      {
        case 'F':
          creat(pathname);
          break;
        case 'D':
          mkdir(pathname);
          break;
      }
    }
    memset(directoryname,0,sizeof(directoryname));
    memset(pathname,0,sizeof(pathname));
  }
  fclose(fp);  

}

void menu()
{
  printf("\nNOTE: commands = [ mkdir | rmdir | ls | cd | pwd | creat | rm | reload | save | menu | quit ]\n");
}



int main()
{
  int index;

  initialize();

  menu();

  while(1){
      printf("Enter command line : ");
      fgets(line, 128, stdin);
      line[strlen(line)-1] = 0;

      command[0] = 0;     //The statement is for checking whether the -
      pathname[0] = 0;    // - command or pathname are left empty
      sscanf(line, "%s %s", command, pathname);
      printf("command=%s pathname=%s\n", command, pathname);
      
      if (command[0]==0) 
         continue;

      index = findCmd(command);     // THIS WILL GET US THE INDEX OF THE COMMAND

      switch (index){
        case 0: mkdir(pathname);                                                                break;
        case 1: rmdir(pathname);                                                                break;
        case 2: ls();                                                                           break;
        case 3: cd(pathname);                                                                   break;
        case 4: 
        {
          printf("\nPresent Working Directory: ");
          if(strcmp(cwd->name,"/") == 0)
            printf("/\n");
          else
          {
            pwd(cwd);
            printf("\n");                 
            
          }
                                                                                                break;
        }
        case 5: creat(pathname);                                                                break;
        case 7: reload();
        case 6: rm(pathname);                                                                   break;
        case 8: save(root);                                                                     break;
        case 9: menu();                                                                         break;
        case 10: quit();                                                                        break;
      }
  }
}


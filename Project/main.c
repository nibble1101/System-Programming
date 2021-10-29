/****************************************************************************
*                   KCW: mount root file system                             *
*****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <ext2fs/ext2_fs.h>
#include <string.h>
#include <libgen.h>
#include <sys/stat.h>
#include <time.h>

#include "type.h"
#include"util.c"
#include"cd_ls_pwd.c"

extern MINODE *iget();
int quit();

MINODE minode[NMINODE];
MINODE *root;
PROC   proc[NPROC], *running;

char gpath[128]; // global for tokenized components
char *name[64];  // assume at most 64 components in pathname
int   n;         // number of component strings

int fd, dev;
int nblocks, ninodes, bmap, imap, iblk;
char line[128], cmd[32], pathname[128];
char *disk = "diskimage";

int init()
{
  int i, j;
  MINODE *mip;
  root = 0;
  PROC   *p;

  printf("init()\n");

  // // Initializing the data structures.
  // proc[0] = malloc(sizeof(PROC));
	// proc[1] = malloc(sizeof(PROC));
	// running = malloc(sizeof(PROC));

  for (i=0; i<NMINODE; i++)
  {
    mip = &minode[i];
    mip->dev = mip->ino = 0;
    mip->refCount = 0;
    mip->mounted = 0;
    mip->mptr = 0;
  }
  for (i=0; i<NPROC; i++)
  {
    p = &proc[i];
    p->pid = i;
    p->uid = p->gid = 0;
    p->cwd = 0;
  }
}

// load root INODE and set root pointer to it
int mount_root()
{
  printf("\nmount_root()\n");
  printf("\nchecking EXT2 FS ....");

  if ((dev = open(disk, O_RDWR)) < 0){
    printf("\nopen %s failed\n", disk);
    exit(1);
  }

  /********** read super block  ****************/
  char superBuf[1024];
  get_block(dev, 1, superBuf);
  SUPER *sp = (SUPER*)superBuf;

   /* verify it's an ext2 file system ***********/
  if (sp->s_magic != 0xEF53){
      printf("\nmagic = %x is not an ext2 filesystem\n", sp->s_magic);
      exit(1);
  }
  
  nblocks = sp->s_blocks_count;
  ninodes = sp->s_inodes_count;

  GD group;
  char groupBuf[1024];
  get_block(dev, 2, groupBuf);
  GD *gd = (GD*)groupBuf;
  bmap = gd->bg_block_bitmap;
  imap = gd->bg_inode_bitmap;
  iblk = gd->bg_inode_table;
  printf("\nbmp=%d imap=%d inode_start = %d\n", bmap, imap, iblk);

  root = iget(dev, 2);
  proc[0].cwd = iget(dev, 2);
	proc[1].cwd = iget(dev, 2);

  printf("root refCount = %d\n", root->refCount);

  printf("creating P0 as running process\n");
  running = &proc[0];
  running->status = READY;
  running->cwd = iget(dev, 2);
  printf("root refCount = %d\n", root->refCount);

}



int main(int argc, char *argv[ ])
{
  int ino;
  init();  
  mount_root();
  
  // WRTIE code here to create P1 as a USER process
  
  while(1){
    printf("input command : [ls|cd|pwd|quit] ");
    fgets(line, 128, stdin);
    line[strlen(line)-1] = 0;

    if (line[0]==0)
       continue;
    pathname[0] = 0;

    sscanf(line, "%s %s", cmd, pathname);
    printf("cmd=%s pathname=%s\n", cmd, pathname);
  
    if (strcmp(cmd, "ls")==0)
       ls();
    else if (strcmp(cmd, "cd")==0)
       cd();
    else if (strcmp(cmd, "pwd")==0)
       pwd(running->cwd);
    else if (strcmp(cmd, "quit")==0)
       quit();
    memset(line, 0, sizeof(line));
    memset(cmd, 0, 64);
	  memset(pathname, 0, 64);

  }
}

int quit()
{
  int i;
  MINODE *mip;
  for (i=0; i<NMINODE; i++){
    mip = &minode[i];
    if (mip->refCount > 0)
      iput(mip);
  }
  exit(0);
}

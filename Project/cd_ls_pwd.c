/************* cd_ls_pwd.c file **************/
#include"type.h"

int cd()
{
  // printf("cd: under construction READ textbook!!!!\n");

  int ino = getino(pathname);
  MINODE *mip = iget(dev, ino);
  if(S_ISDIR(mip->INODE.i_mode))
  {
    iput(running->cwd);
    running->cwd = mip;
  }
  else
  {
    printf("\n****The file type is not directory.****\n");
  }

}

int ls_file(MINODE *mip, char *name)
{
  INODE *ip;
	char c, *time, dir[256];

	ip = &mip->INODE;

	// Print file mode
	if(S_ISDIR(ip->i_mode))
		printf("d");
	else if (S_ISREG(ip->i_mode))
		printf("-");
	else if(S_ISLNK(ip->i_mode))
		printf("l");
	else
		printf("-");

  // Permissions checking
	for (int i = 8; i >= 0; i--)
	{
		if (i % 3 == 2)
		{
			if (ip->i_mode & (1 << i))
				c = 'r';
			else
				c = '-';
		}
		if (i % 3 == 1)
		{
			if (ip->i_mode & (1 << i))
				c = 'w';
			else
				c = '-';
		}
		if (i % 3 == 0)
		{
			if (ip->i_mode & (1 << i))
				c = 'x';
			else
				c = '-';
		}

		putchar(c);
	}
	// Other info/name printing
	printf(" %d %d %d %.4d", ip->i_links_count, ip->i_uid, ip->i_gid, ip->i_size);
	time = ctime(&(ip->i_mtime));
	time[strlen(time)-1] = '\0';
	printf(" %s ", time);
  printf(" %s\n", name);
}

int ls_dir(MINODE *mip)
{
  // printf("ls_dir: list CWD's file names; YOU FINISH IT as ls -l\n");

  char buf[BLKSIZE], temp[256];
  DIR *dp;
  char *cp;
  int ino;

  get_block(dev, mip->INODE.i_block[0], buf);
  dp = (DIR *)buf;
  cp = buf;
  
  while (cp < buf + BLKSIZE){
     strncpy(temp, dp->name, dp->name_len);
     temp[dp->name_len] = 0;
     ino = dp->inode;
     mip = iget(dev,ino);
     ls_file(mip, temp);

     cp += dp->rec_len;
     dp = (DIR *)cp;
  }
  printf("\n");
}

int ls()
{
  // printf("ls: list CWD only! YOU FINISH IT for ls pathname\n");
  MINODE *mip;
  if(strlen(pathname) == 0)
  {
		mip = running->cwd;
    ls_dir(mip);
  }
  else
  {
    if(pathname[0] == '/')
    {
      mip = root;
    }
    else
    {
      mip = running->cwd;
    }
  }
  printf("\npathname = %s", pathname);
  int ino = getino(pathname);
  mip = iget(dev, ino);
  if(S_ISDIR(mip->INODE.i_mode))
    ls_dir(mip);
  else
  {
    printf("\n****The file type is not directory.****\n");
  }

}

void rpwd(MINODE *wd, int tempIno)
{
  char buf[BLKSIZE], name[256];
	char *cp;
	DIR *dp;
	MINODE *parentMip;

	get_block(dev, wd->INODE.i_block[0], (char *)&buf);
	dp = (DIR *)buf;     // get first dir "."
	cp = buf + dp->rec_len;
	dp = (DIR *)cp;   // get second dir ".."

	if (wd->ino != root->ino)
	{
		int parentIno = dp->inode;
		parentMip = iget(dev, parentIno);
		rpwd(parentMip, wd->ino);
	}
	if (tempIno != 0)
	{
		while(dp->inode != tempIno)
		{
			cp += dp->rec_len;
			dp = (DIR *)cp;     // get second dir ".."
		}
		strncpy(name, dp->name, dp->name_len);
		name[dp->name_len] = '\0';
		printf("%s/", name);
	}
	return;
}
char *pwd(MINODE *wd)
{
  // printf("pwd: READ HOW TO pwd in textbook!!!!\n");
  if (wd->ino == root->ino){
    printf("/\n");
    return;
  }
  else
  {
    printf("/");
    rpwd(wd,0);
  }
  printf("\n");
  
}




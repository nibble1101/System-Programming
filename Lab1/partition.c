#include <stdio.h>
#include <fcntl.h>

#include <sys/types.h>
#include <unistd.h>

typedef unsigned char  u8;
typedef unsigned short u16;
typedef unsigned int   u32;

struct partition {
	u8 drive;             // drive number FD=0, HD=0x80, etc.

	u8  head;             // starting head 
	u8  sector;           // starting sector
	u8  cylinder;         // starting cylinder

	u8  sys_type;         // partition type: NTFS, LINUX, etc.

	u8  end_head;         // end head 
	u8  end_sector;       // end sector
	u8  end_cylinder;     // end cylinder

	u32 start_sector;     // starting sector counting from 0 
	u32 nr_sectors;       // number of of sectors in partition
  };


  /* sample code for Part 2 */      

char *dev = "vdisk";
int fd;
    
// read a disk sector into char buf[512]
int read_sector(int fd, int sector, char *buf)
{
    lseek(fd, sector*512, SEEK_SET);  // lssek to byte sector*512
    read(fd, buf, 512);               // read 512 bytes into buf[ ]
}

int main()
{
    struct partition *p;
    char buf[512];
    u32 totalSector;

    fd = open(dev, O_RDONLY);   // open dev for READ
    read_sector(fd, 0, buf);    // read in MBR at sector 0    

    p = (struct partition *)(&buf[0x1be]); // p->P1
    int i = 1;
    for(i = 1; i<=4;i++)
    {
        printf("%d.\n",i);
        printf("start_sector: %u\nnr_sectors: %u\nsys_type: %x\n\n", p->start_sector,p->nr_sectors,p->sys_type);
        p++;
    }
    p--;
    u32 extStart = p->start_sector;

    printf("P4 start_sector:%u\n", extStart);
    int localMBR = extStart;
    read_sector(fd, localMBR, buf);
    p = (struct partition *)(&buf[0x1be]); // p->P1
    int isEven = 1;    
    do
    {
        if (isEven == 1)
        {
            printf("start_sector: %u\nend_sectors: %u\nnr_sectors: %x\n\n",p->start_sector,p->end_sector,p->nr_sectors );
            extStart = extStart + p->start_sector;
            printf("\nTotal offset = %u",extStart);
            p++;
            if(p->start_sector == 0)
                break;
            
            isEven +=1;
        }
        if(isEven == 2)
        {
            //printf("\nEven start_sector: %u\n",p->start_sector);
            localMBR = p->start_sector + extStart;
            extStart = localMBR;
            printf("\nTotal offset = %u\n",extStart);
            //printf("\nfinal localMBR: %u", localMBR);
            read_sector(fd, localMBR, buf);
            p = (struct partition *)(&buf[0x1be]); // p->P1
            isEven = 1;
        }
        
        //p = (struct partition *)&buf[0x1BE];
        
        //localMBR = p->end_sector;
    }while(1);


}
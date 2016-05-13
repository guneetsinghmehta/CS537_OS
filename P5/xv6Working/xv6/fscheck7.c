#include<stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<unistd.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <assert.h>
//#include <dirent.h>
#include <stdbool.h>


#define BSIZE (512)
#define NDIRECT 12
#define NINDIRECT (BSIZE / sizeof(uint))
#define MAXFILE (NDIRECT + NINDIRECT)
#define IPB           (BSIZE / sizeof(struct dinode))
#define ROOTINO 1  // root i-number
#define dirent xv6_dirent  // avoid clash with host struct stat

// Block 0 is unused.
// Block 1 is super block.
// Inodes start at block 2.
struct superblock {
  uint size;         // Size of file system image (blocks)
  uint nblocks;      // Number of data blocks
  uint ninodes;      // Number of inodes.
};
#define DIRSIZ 14
struct dirent {
  ushort inum;
  char name[DIRSIZ];
};

struct dinode {
  short type;           // File type
  short major;          // Major device number (T_DEV only)
  short minor;          // Minor device number (T_DEV only)
  short nlink;          // Number of links to inode in file system
  uint size;            // Size of file (bytes)
  uint addrs[NDIRECT+1];   // Data block addresses
};

struct block512 {
	char string[512];
};

void *img_ptr;
struct superblock *sb;
int fsfd;

int checkInodes();
void rsect(uint sec, void *buf);
int checkRootInode();

int main(int argc, char *argv[])
{
	fsfd=open(argv[1],O_RDONLY);
	if(fsfd<0)//checks if file present or not
	{fprintf(stderr,"image not found.\n");return 1; }
	
	int rc;
	struct stat sbuf;
	rc=fstat(fsfd,&sbuf);
	assert(rc==0);
	//use mmap
	img_ptr=mmap(NULL, sbuf.st_size, PROT_READ, MAP_PRIVATE,fsfd, 0);
	assert(img_ptr!=MAP_FAILED);
	
	sb=(struct superblock *)(img_ptr+BSIZE);
	//Check all Inode structures
	if(checkInodes(img_ptr,sb)==1){return 1;}
	if(checkRootInode()==1){return 1;}
	return 0;
}


int checkInodes()
{	
	int i=0,j;
	struct dinode *dip=(struct dinode*)(img_ptr+2*BSIZE);
//	
	dip=dip+1;//root inode starts from 1 as in #define ROOTINo 1
	for(i=0;i<sb->ninodes;i++)
	{
		//checkInodeAddresses(dip);
		if(dip->type>=4)
		{
			fprintf(stderr,"ERROR: bad inode.\n");
			return 1; 
		}
		//printf("type=%hu major=%hu minor=%hu nlink=%hu size=%u ",dip->type,dip->major,dip->minor,dip->nlink,dip->size);
		for( j=0;j<sizeof(dip->addrs)/sizeof(uint);j++)
		{
			//printf("%u ",dip->addrs[j]);
		}
		//printf("\n");
		dip++;
	}
	return 0;
}	

int checkRootInode()
{
	int i=0,j;
	struct dinode *dip=(struct dinode*)(img_ptr+2*BSIZE);
	dip=dip+1;//root inode starts from 1 as in #define ROOTINo 1
	
	char buf[512];
	
	
	struct dirent *de=(struct dirent*)(img_ptr+(dip->addrs[0])*BSIZE);
	/*
	while(strlen(de->name)!=0&&de<(struct dirent*)(img_ptr+(dip->addrs[0]+1)*BSIZE))
	{
		printf("%s %d\n",de->name,de->inum);
		de++;	
	}
	*/
	//printf("%d %d",de->inum,(de+1)->inum);
	if(de->inum!=(de+1)->inum||de->inum!=1)
	{
		fprintf(stderr,"ERROR: root directory does not exist.\n");
		return 1;
	}
	return 0;
}

void
rsect(uint sec, void *buf)
{
  if(lseek(fsfd, sec * 512L, 0) != sec * 512L){
    perror("lseek");
    exit(1);
  }
  if(read(fsfd, buf, 512) != 512){
    perror("read");
    exit(1);
  }
}


























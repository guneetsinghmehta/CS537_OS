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
int checkRootInode();
void rsect(uint sec, void *buf);
int checkAddresses(struct dinode*);

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
	if(checkRootInode()==1){return 1;}
	if(checkInodes(img_ptr,sb)==1){return 1;}
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

int checkDataBlock(int index)
{
//returns 1 if used and 0 else
	//index is the page number
	//REad the character charNumber=index%8
	//Shift =index-8(index/8);Shift=7-Shift; 
	//And shifted index char with 1 
	if(index==0){return 0;}
	
	char buf[512];
	int bitblocks = index/(512*8) ;
	int blockIndex = sb->ninodes / IPB + 3 + bitblocks;
	rsect(blockIndex,buf);
	
	int charNumber=index/8;
	int shift=index-8*(shift/8);
	shift=7-shift;
	char c=buf[charNumber];
	int res=(int)c;
	res=res>>shift;
	res=res%2;
	res=(res!=0);
	return res;
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
		if(dip->type==1)
		{//directory check
			struct dirent *de=(struct dirent*)(img_ptr+(dip->addrs[0])*BSIZE);	
			if(strcmp(de->name,".")!=0||strcmp((de+1)->name,"..")!=0)
			{
				//ERROR: directory not properly formatted.
				fprintf(stderr,"ERROR: directory not properly formatted.\n");
				return 1;
			}
			
			if(strcmp(de->name,"..")==0&&(i!=de->inum))
			{
				fprintf(stderr,"ERROR: directory not properly formatted.\n");
				return 1;
			}
		}
		if(checkAddresses(dip)==1)
		{
			fprintf(stderr,"ERROR: bad address in inode.\n");
			return 1;
		}
		//printf("type=%hu major=%hu minor=%hu nlink=%hu size=%u ",dip->type,dip->major,dip->minor,dip->nlink,dip->size);
		for( j=0;j<sizeof(dip->addrs)/sizeof(uint);j++)
		{
			//printf("-%u %d-",dip->addrs[j],checkDataBlock(dip->addrs[j]));
		}
		//printf("\n");
		dip++;
	}
	return 0;
}	

int checkAddresses(struct dinode *dip)
{
//return 1 if for error and 0 if no
	int i,j;
	for(i=0;i<=11&&dip->addrs[i]!=0;i++)
	{
		if(checkDataBlock(dip->addrs[i])==0)
		{
			return 1;
		//	printf("%d-%d ",dip->addrs[i],checkDataBlock(dip->addrs[i]));//return value of CheckDataBlock is 1 if used 0 if not
		}
	}
	//checking for indirect pointers
	if(checkDataBlock(dip->addrs[12])==1)
	{
		uint pointers[1024];
		//void rsect(uint sec, void *buf)
		rsect(dip->addrs[12],pointers);
		for(j=0;j<1024&&(pointers[j]!=0);j++)
		{
			//printf("%d ",pointers[j]);
			if(checkDataBlock(pointers[j])==0)
			{
				return 1;
			}
		}
	}
	return 0;
}

int checkRootInode()
{
	int i=0,j;
	struct dinode *dip=(struct dinode*)(img_ptr+2*BSIZE);
	dip=dip+1;//root inode starts from 1 as in #define ROOTINo 1
	

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




























#include<stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<unistd.h>
#include<assert.h>
#include <sys/mman.h>

#define BSIZE (512)
#define NDIRECT 12
#define NINDIRECT (BSIZE / sizeof(uint))
#define MAXFILE (NDIRECT + NINDIRECT)

// Block 0 is unused.
// Block 1 is super block.
// Inodes start at block 2.
struct superblock {
  uint size;         // Size of file system image (blocks)
  uint nblocks;      // Number of data blocks
  uint ninodes;      // Number of inodes.
};

struct dinode {
  short type;           // File type
  short major;          // Major device number (T_DEV only)
  short minor;          // Minor device number (T_DEV only)
  short nlink;          // Number of links to inode in file system
  uint size;            // Size of file (bytes)
  uint addrs[NDIRECT+1];   // Data block addresses
};


int main(int argc, char *argv[])
{
	int f1=open(argv[1],O_RDONLY);
	if(f1<0)//checks if file present or not
	{fprintf(stderr,"image not found");return 1; }
	
	int rc;
	struct stat sbuf;
	rc=fstat(f1,&sbuf);
	assert(rc==0);
	//use mmap
	void *img_ptr=mmap(NULL, sbuf.st_size, PROT_READ, MAP_PRIVATE,
                  f1, 0);
	assert(img_ptr!=MAP_FAILED);
	
	char buf[BSIZE];
	

	struct superblock *sb;
	sb=(struct superblock *)(img_ptr+BSIZE);
	
	printf("%d %d %d ",sb->size, sb->nblocks,sb->ninodes);
	
	
	int i=0;
	struct dinode *dip=(struct dinode*)(img_ptr+2*BSIZE);
	for(i=0;i<sb->ninodes;i++)
	{
		printf("%d %d \n",i,dip->type);
		dip++;
	}
	
	return 0;
}


















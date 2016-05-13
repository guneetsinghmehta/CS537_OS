#include<stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include
#define BLOCK_SIZE (512)
// Block 0 is unused.
// Block 1 is super block.
// Inodes start at block 2.
struct superblock {
  uint size;         // Size of file system image (blocks)
  uint nblocks;      // Number of data blocks
  uint ninodes;      // Number of inodes.
};

int main(int argc, char *argv[])
{
	int f1=open(argv[1],O_RDONLY);
	printf("%d %s",f1,argv[1]);
	if(f1<0)//checks if file present or not
	{fprintf(stderr,"image not found");return 1; }
	
	int rc;
	char buf[BSIZE];
	
	rc=read(f1,buf,BSIZE);
	assert(rc==BSIZE);
	
	struct superblock *sb;
	sb=(struct superblock *)buf;
	
	printf("%d %d %d ",sb->size, sb->nblocks,sb->ninodes);
	
	return 0;
}

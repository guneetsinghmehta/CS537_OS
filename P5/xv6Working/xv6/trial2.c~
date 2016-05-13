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

void *img_ptr;
struct superblock *sb;
int fsfd;

//Structures
int *dbMap,*dbReference,*iMap,*iReference;
//functions
int checkEntryByNumber(struct dinode*,int );
int checkMismatch(int);
int checkInodes();
int checkRootInode();
void rsect(uint sec, void *buf);
int checkAddresses(struct dinode*);
int checkEntry(struct dinode*, char*);


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
		
	dbMap=(int*)malloc(sizeof(int)*sb->nblocks);
	dbReference=(int*)malloc(sizeof(int)*sb->nblocks);
	iMap=(int*)malloc(sizeof(int)*sb->ninodes);
	iReference=(int*)malloc(sizeof(int)*sb->ninodes);
	//initialize structures
	int i;
	for(i=0;i<sb->nblocks;i++)
	{
		dbMap[i]=0;dbReference[i]=0;
		if(i<29&&i!=0){dbReference[i]=1;}
		if(i<sb->ninodes){iMap[i]=0;iReference[i]=0;}
	}
	
	if(checkInodes(img_ptr,sb)==1){return 1;}
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
	//shift=7-shift; //leads to flips
	char c=buf[charNumber];
	int res=(int)c;
	res=res>>shift;
	res=res%2;
	res=(res!=0);
	return res;
}

int checkInodes()
{	
	//printf("pages=%d IPB =%d ninodes=%d\n",sb->ninodes / IPB + 2,IPB,sb->ninodes); 
	int i=0,j,k;
	struct dinode *dip=(struct dinode*)(img_ptr+2*BSIZE);
	dip=dip+1;//root inode starts from 1 as in #define ROOTINo 1
	struct dinode *dipFirst=dip;
	//Filling dbMap
	uint pointers[sb->nblocks];
	for(i=0;i<sb->nblocks;i++)
	{
		dbMap[i]=checkDataBlock(i);
	}
	for(k=0;k<sb->ninodes;k++)
	{
		//checking valid inode type
		if(dip->type>=4)
		{
			fprintf(stderr,"ERROR: bad inode.\n");	
			return 1; 
		}
		if(dip->type==0){continue;}
		iMap[k]=1;//stores whether used or not
		for(i=0;i<=12&&dip->addrs[i]!=0;i++)
		{
			if(dip->addrs[i]>=sb->nblocks)
			{
				fprintf(stderr,"ERROR: bad address in inode.\n");	
				return 1; 
			}
			//printf("%d ",dip->addrs[i]);
			dbReference[dip->addrs[i]]++;
		}
		if(dip->addrs[12]!=0)
		{
		 	rsect(dip->addrs[12],pointers);
		 	for(j=0;j<sb->nblocks&&(pointers[j]!=0);j++)
			{
				dbReference[pointers[j]]++;
				if(pointers[j]>=sb->nblocks)
				{
					fprintf(stderr,"ERROR: bad address in inode.\n");	
					return 1; 
				}
				//printf("%d ",pointers[j]);
			}
		}
		//printf("\n");
		if(dip->type==1)
		{
			if(checkFormat(dip)==1){return 1;}
			//if(checkMismatch(k+1)==1){return 1;}//k+1 because kth entry is k+1 inode num
		}
		dip++;
	}
	
	for (i=0;i<sb->nblocks;i++)
	{
		if(dbMap[i]==1&&dbReference[i]==0)
		{fprintf(stderr,"ERROR: bitmap marks block in use but it is not in use.\n");return 1;} 
		if(dbReference[i]>1)
		{fprintf(stderr,"ERROR: address used more than once.\n");return 1;} 
		if(dbMap[i]==0&&dbReference[i]==1)
		{fprintf(stderr,"ERROR: address used by inode but marked free in bitmap.\n");return 1;} 
	}
	//printf("Original dbMap\n");
	for(i=0;i<sb->nblocks;i++)
	{
	  //printf("%d-%d-%d ",i,dbMap[i],dbReference[i]);
  	  printf(" %d-%d ",dbMap[i],dbReference[i]);
	  if(i%16==0){printf("\n");}
	}
	return 0;
}	

int checkMismatch(int childINum)
{
	//Return 0 if no mismatch and 1 if there is mismatch
	//Input - dinode of child.
	int parentINum;
	struct dinode *dipChild=(struct dinode*)(img_ptr+2*BSIZE);
	struct dinode *dipParent=(struct dinode*)(img_ptr+2*BSIZE);
	int i;
	for(i=0;i<childINum;i++)
	{dipChild++;}
	char parentStr[100]=".";
	parentINum=checkEntry(dipChild,parentStr);
	
	printf("parent=%d child=%d",parentINum,childINum);
	
	for(i=0;i<parentINum;i++)
	{dipParent++;}
	int ans=checkEntryByNumber(dipParent,childINum);

	if(ans==0)
	{
		fprintf(stderr,"ERROR: parent directory mismatch.\n");	
		return 1; 
	}
	return 0;
}

int checkFormat(struct dinode *dip)
{
	//Returns 0 if correct format and 1 if not	
	//Check for directory
	char parent[100]=".";
	char self[100]="..";
	//printf("directory %d %d\n",checkEntry(dip,self),checkEntry(dip,parent));
	if(checkEntry(dip,self)==0||checkEntry(dip,parent)==0)
	{
		fprintf(stderr,"ERROR: directory not properly formatted.\n");	
		return 1; 
	}
	return 0;
}

int checkRootInode()
{
//returns 1 if error and 0 if no error
	int i=0,j;
	struct dinode *dip=(struct dinode*)(img_ptr+2*BSIZE);
	dip=dip+1;//root inode starts from 1 as in #define ROOTINo 1
	struct dirent *de=(struct dirent*)(img_ptr+(dip->addrs[0])*BSIZE);

	//if(de->inum!=(de+1)->inum||de->inum!=1)
	char self[100]=".";
	char parent[100]="..";

	if(checkEntry(dip,self)!=1||checkEntry(dip,parent)!=1)
	{
		fprintf(stderr,"ERROR: root directory does not exist.\n");
		return 1;
	}

	return 0;
}

int checkEntry(struct dinode *dip,char *entry)
{
//Returns inum if the entry is present in dip and 0 if not - checks all the direct and indirect data pages
	struct dirent *de;
	int i,j;
	uint pointers[sb->nblocks];
	for(i=0;i<12&&dip->addrs[i]!=0;i++)
	{
		//direct pointers
		de=(struct dirent*)(img_ptr+(dip->addrs[i])*BSIZE);
		while(de->inum!=0&&de<(struct dirent*)(img_ptr+(dip->addrs[i]+1)*BSIZE))
		{
			//printf("%d %s\n",de->inum,de->name);
			if(strcmp(de->name,entry)==0){return de->inum;}
			de++;
		}
	}
	if(dip->addrs[12]!=0)
	{
		//indirect pointers
		rsect(dip->addrs[12],pointers);
		for(j=0;j<sb->nblocks&&(pointers[j]!=0);j++)
		{
			de=(struct dirent*)(img_ptr+(pointers[j])*BSIZE);
			while(de->inum!=0&&de<(struct dirent*)(img_ptr+(pointers[j]+1)*BSIZE))
			{
				//printf("%d %s\n",de->inum,de->name);
				if(strcmp(de->name,entry)==0){return de->inum;}
				de++;
			}
		}
	}
	return 0;
}

int checkEntryByNumber(struct dinode *dip,int entry)
{
//Returns inum if the entry is present in dip and 0 if not - checks all the direct and indirect data pages
	struct dirent *de;
	int i,j;
	uint pointers[sb->nblocks];
	for(i=0;i<12&&dip->addrs[i]!=0;i++)
	{
		//direct pointers
		de=(struct dirent*)(img_ptr+(dip->addrs[i])*BSIZE);
		while(de->inum!=0&&de<(struct dirent*)(img_ptr+(dip->addrs[i]+1)*BSIZE))
		{
			//printf("%d %s\n",de->inum,de->name);
			if(de->inum==entry){return de->inum;}
			de++;
		}
	}
	if(dip->addrs[12]!=0)
	{
		//indirect pointers
		rsect(dip->addrs[12],pointers);
		for(j=0;j<sb->nblocks&&(pointers[j]!=0);j++)
		{
			de=(struct dirent*)(img_ptr+(pointers[j])*BSIZE);
			while(de->inum!=0&&de<(struct dirent*)(img_ptr+(pointers[j]+1)*BSIZE))
			{
				//printf("%d %s\n",de->inum,de->name);
				if(de->inum==entry){return de->inum;}
				de++;
			}
		}
	}
	return 0;
}

























#include "types.h"
#include "defs.h"
#include "param.h"
#include "mmu.h"
#include "proc.h"
#include "x86.h"
#include "syscall.h"
#include "sysfunc.h"

//count of number of shared pages  
int getco()
{
    int count=0;
    int k=0;
    for(k=0;k<4;k++)
    {
        if(proc->bitmap[k]==-1)
        {
            count++;
        }
    }
    return count;
}
// User code makes a system call with INT T_SYSCALL.
// System call number in %eax.
// Arguments on the stack, from the user call to the C
// library system call function. The saved user %esp points
// to a saved program counter, and then the first argument.

// Fetch the int at addr from process p.
int
fetchint(struct proc *p, uint addr, int *ip)
{
  int count=getco();
  if((addr >= p->sz || addr+4 > p->sz) && (addr < USERTOP-count*PGSIZE)) 
    return -1;
  if(p->pid> 2 && ((addr<PGSIZE)))// || ((addr >= p->sz || addr+4 > p->sz) || (addr < USERTOP-count*PGSIZE))))
        return -1;
  *ip = *(int*)(addr);
  return 0;
}

// Fetch the nul-terminated string at addr from process p.
// Doesn't actually copy the string - just sets *pp to point at it.
// Returns length of string, not including nul.
int
fetchstr(struct proc *p, uint addr, char **pp)
{
  char *s, *ep;

  int count=getco();
  if(p->pid> 2 && addr<PGSIZE) //check for page 0 invalid
      return -1;
  if(addr >= p->sz && addr<USERTOP-count*PGSIZE)
    return -1;

  if(addr > USERTOP-count*PGSIZE) //accessing the shared memory
  {
   
  *pp = (char*)addr;
  ep = (char*)PGROUNDUP(addr+1);
//  cprintf("count=%d, addr-%d, ep=%d\n",count,addr,ep);
  for(s = *pp; s < ep; s++)
    if(*s == 0)
      return s - *pp;
  }
  else
  {
      *pp = (char*)addr;
  ep = (char*)p->sz;
  for(s = *pp; s < ep; s++)
    if(*s == 0)
      return s - *pp;
}
  return -1;
}

// Fetch the nth 32-bit system call argument.
int
argint(int n, int *ip)
{
  int count=getco();
  if((n >= proc->sz||n+4>=proc->sz) && (n<USERTOP-count*PGSIZE))
    return -1;
  return fetchint(proc, proc->tf->esp + 4 + 4*n, ip);
}
//Understand this better
// Fetch the nth word-sized system call argument as a pointer
// to a block of memory of size n bytes.  Check that the pointer
// lies within the process address space.
int argptr(int n, char **pp, int size)
{
  int i;
// cprintf("n=%d,size=%d\n",n,size); 
  if(argint(n, &i) < 0)
    return -1;
  if(i>=0&&i<PGSIZE)
   	return -1;
  
  int count=getco();
//  if(((uint)i >= proc->sz || ((((uint)i+size > proc->sz)) && (((uint)i <= USERTOP-count*PGSIZE) || ((uint)i+size < USERTOP-count*PGSIZE))) ||((uint)i+size>USERTOP)))
  
  if((((uint)i+size>=proc->sz) && ((uint)i<USERTOP-count*PGSIZE) ) || (uint)i+size>USERTOP)
      return -1;
/*
  {
      cprintf("addr=%d,proc sz=%d,shared=%d\n",i+size,proc->sz,USERTOP-count*PGSIZE);
      return -1;}
      */
    /*
  if((uint)i >= USERTOP-count*PGSIZE || (uint)i+size > USERTOP-count*PGSIZE)
    return -1;
*/
      *pp = (char*)i;
      return 0;

}

  /*int a,b,c,d,check;
  a=PGSIZE;b=proc->sz;
  c=USERTOP-count*PGSIZE;d=USERTOP;
  check=(uint)i+size;

  cprintf("check=%d size=%d a=%d b=%d c=%d d=%d\n",check,size,a,b,c,d);
  
  if((check>=a&&check<=b)||(check>=c&&check<=d))
  
  { 
       cprintf("return 0\n");
       return 0;
  }
  else{
      cprintf("return -1\n");
      return -1;
  }*/
// Fetch the nth word-sized system call argument as a string pointer.
// Check that the pointer is valid and the string is nul-terminated.
// (There is no shared writable memory, so the string can't change
// between this check and being used by the kernel.)
int
argstr(int n, char **pp)
{
  int addr;
  if(argint(n, &addr) < 0)
    return -1;
  return fetchstr(proc, addr, pp);
}

// syscall function declarations moved to sysfunc.h so compiler
// can catch definitions that don't match

// array of function pointers to handlers for all the syscalls
static int (*syscalls[])(void) = {
[SYS_chdir]   sys_chdir,
[SYS_close]   sys_close,
[SYS_dup]     sys_dup,
[SYS_exec]    sys_exec,
[SYS_exit]    sys_exit,
[SYS_fork]    sys_fork,
[SYS_fstat]   sys_fstat,
[SYS_getpid]  sys_getpid,
[SYS_kill]    sys_kill,
[SYS_link]    sys_link,
[SYS_mkdir]   sys_mkdir,
[SYS_mknod]   sys_mknod,
[SYS_open]    sys_open,
[SYS_pipe]    sys_pipe,
[SYS_read]    sys_read,
[SYS_sbrk]    sys_sbrk,
[SYS_sleep]   sys_sleep,
[SYS_unlink]  sys_unlink,
[SYS_wait]    sys_wait,
[SYS_write]   sys_write,
[SYS_uptime]  sys_uptime,
[SYS_shmem_count]  sys_shmem_count,
[SYS_shmem_access]  sys_shmem_access,
};

// Called on a syscall trap. Checks that the syscall number (passed via eax)
// is valid and then calls the appropriate handler for the syscall.
void
syscall(void)
{
  int num;
  
  num = proc->tf->eax;
  if(num > 0 && num < NELEM(syscalls) && syscalls[num] != NULL) {
    proc->tf->eax = syscalls[num]();
  } else {
    cprintf("%d %s: unknown sys call %d\n",
            proc->pid, proc->name, num);
    proc->tf->eax = -1;
  }
}

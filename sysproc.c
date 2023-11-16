#include "param.h"
#include "types.h"
#include "defs.h"
#include "x86.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "date.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  myproc()->sz = myproc()->sz + n; //modify size 
  //but don't allocate the space
  // if(growproc(n) < 0)
  //   return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

int
sys_settickets(void)
{
  int n;

  if(argint(0, &n) < 0)
    return -1;
  
  myproc()->tickets = n;
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

int
sys_yield(void)
{
  yield();
  return 0;
}

int sys_shutdown(void)
{
  shutdown();
  return 0;
}

/*returns the last-level page table entry for pid at virtual address address,
or 0 if there is no such page table entry*/
int sys_getpagetableentry(void){ //TODO: Complete Function

  //populate variables
  int pid;
  int address;
  if(argint(0, &pid) < 0)
    return -1;
  if(argint(1, &address) < 0)
    return -1;
  return get_pagetable_entry(pid, address);
}
 
int sys_isphysicalpagefree(void){
  //populate variables
  int ppn;
  if(argint(0, &ppn) < 0) 
    return -1;

  return is_physicalpage_free(ppn);
}

int sys_dumppagetable(void){
  //populate variables
  int pid;
  if(argint(0, &pid) < 0)
    return -1;

  return dump_ptable(pid);
}
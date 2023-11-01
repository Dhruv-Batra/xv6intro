#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"



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
  if(growproc(n) < 0)
    return -1;
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
  pde_t *pgdir;

  //get page dir
  acquire(&ptable.lock);
  for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){ //proc.c
      if(p->pid == pid){
        pgdir = p->pgdir;
      }
  }
  release(&ptable.lock);

  //get page table entry, xv6 has 2 level page table;
  return walkpgdir(pgdir, address, 0);
}

int sys_isphysicalpagefree(void){
  //populate variables
  int ppn;
  if(argint(0, &ppn) < 0)
    return -1;

  //iterate through free list 
  struct run *freeTable = freelist;
  while (freeTable != NULL){
    if (ppn == V2P(freeTable) >> PTXSHIFT){ //vm.c
      return 1;
    }
    freeTable = freeTable->next;
  }

  return 0;
}

int dumppagetable(void){
  //populate variables
  int pid;
  if(argint(0, &pid) < 0)
    return -1;

  //get page dir
  int sz; //size of page table in process
  acquire(&ptable.lock);
  for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){ //proc.c
      if(p->pid == pid){
        pgdir = p->pgdir;
        sz = p->sz;
      }
  }

  pte_t *pte;
  cprintf(1, "START PAGE TABLE (pid %d)\n", pid);
  int present = 0;
  int writable = 0;
  int user_mode = 0;
  int page_size = 0;
  int j = 0;
   for(i = PGSIZE; i < sz; i += PGSIZE){ //vm.c
    //error handling
    if((pte = walkpgdir(pgdir, (void *) i, 0)) == 0) //vm.c
      panic("copyuvm: pte should exist"); //vm.c
    //print virtual memory address
    pte* = walkpgdir(pgdir, i, 0);

    flags = PTE_FLAGS(*pte); //vm.c

    present = PTE_P & flags;
    writable = PTE_W & flags;
    user_mode = PTE_U & flags;
    if (!present) { 
      continue;
    }
    if (writable && user_mode){
      cprintf(1, "%d P U W %x\n",j,PTE_ADDR(uint pte));
    }
    else if (user_mode){
      cprintf(1, "%d P U - %x\n",j,PTE_ADDR(uint pte));
    }
    else if (writable){
      cprintf(1, "%d P - W %x\n",j,PTE_ADDR(uint pte));
    }
    else{
      cprintf(1, "%d P - - %x\n",j,PTE_ADDR(uint pte));
    }
    j++;
  }
  cprintf(1, "END PAGE TABLE\n");
  release(&ptable.lock);
}
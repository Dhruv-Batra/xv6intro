#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "getprocessesinfo.h"

extern struct {
  struct spinlock lock;
  struct proc proc[NPROC];
} ptable;

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

int 
sys_getprocessesinfo(void){

  struct proc *pt;
  int num_proc_counter = 0;

  struct processes_info  *p;

  if(argptr(0, (void*)&p, sizeof(*p)) < 0)
    return -1;

  acquire(&ptable.lock);
    for(pt = ptable.proc; pt < &ptable.proc[NPROC]; pt++){
      if(pt->state == UNUSED){
        continue;
      }
      p->pids[num_proc_counter] = pt->pid;
      p->times_scheduled[num_proc_counter] = pt->times_scheduled;
      p->tickets[num_proc_counter] = pt->tickets;

      num_proc_counter++;
    }
  
    p->num_processes = num_proc_counter;
  release(&ptable.lock);
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

// int sys_getprocessesinfo(void){
//   struct processes_info* proc_inst;

//   if(argptr(0, (void*)&proc_inst, sizeof(*proc_inst)) < 0)
//     return -1;
  
//   return getprocessesinfo(proc_inst);
            
// }
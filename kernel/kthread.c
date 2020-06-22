//--------------------------------------------------------------------
//
//  4190.307: Operating Systems (Spring 2020)
//
//  PA#6: Kernel Threads
//
//  June 2, 2020
//
//  Jin-Soo Kim
//  Systems Software and Architecture Laboratory
//  Department of Computer Science and Engineering
//  Seoul National University
//
//--------------------------------------------------------------------

#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "spinlock.h"
#include "sleeplock.h"
#include "proc.h"
#include "defs.h"

// #define SNU 1

#ifdef SNU

// Function for running thread.
void run_thread(void)
{
  // Still holding p->lock from scheduler.
  release(&myproc()->lock);

  // Run argument function.
  myproc()->fn((void*)myproc()->arg);
}

int 
kthread_create(const char *name, int prio, void (*fn)(void *), void *arg)
{
  struct proc *t;

  // Find UNUSED proc and allocate it.
  for (t = proc; t < &proc[NPROC]; t++)
  {
    acquire(&t->lock);
    if (t->state == UNUSED)
    {
      goto found;
    }
    else
    {
      release(&t->lock);
    }
  }
  return 0;

found:
  t->pid = allocpid();
  t->pagetable = kernel_pagetable;

  // Apply arguments to the thread.
  safestrcpy(t->name, name, sizeof(t->name));
  t->fn = fn;
  t->arg = (uint64)arg;

  // Set up context to run thread.
  t->context.ra = (uint64)run_thread;
  t->context.sp = t->kstack + PGSIZE;

  t->state = RUNNABLE;

  release(&t->lock);

  return t->pid;
}

void 
kthread_exit(void)
{
  struct proc* p = myproc();

  acquire(&p->lock);
  p->tf = 0;
  if (p->pagetable)
    proc_freepagetable(p->pagetable, p->sz);
  p->pagetable = 0;
  p->sz = 0;
  p->pid = 0;
  p->parent = 0;
  p->name[0] = 0;
  p->chan = 0;
  p->killed = 0;
  p->xstate = 0;
  p->state = UNUSED;
  release(&p->lock);
}

void
kthread_yield(void)
{
  // Same as yield in proc.c
  struct proc *t = myproc();
  acquire(&t->lock);
  t->state = RUNNABLE;
  sched();
  release(&t->lock);
}

void
kthread_set_prio(int newprio)
{



  return;
}

int
kthread_get_prio(void)
{




  return 0;
}
#endif


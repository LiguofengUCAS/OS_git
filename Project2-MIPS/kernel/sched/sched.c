#include "lock.h"
#include "time.h"
#include "stdio.h"
#include "sched.h"
#include "queue.h"
#include "screen.h"

pcb_t pcb[NUM_MAX_TASK];

/* current running task PCB */
pcb_t *current_running;

/* global process id */
pid_t process_id = 1;

/* kernel stack ^_^ */
#define NUM_KERNEL_STACK 20

uint64_t stack_base = 0xffffffffa0f00000;

static uint64_t kernel_stack[NUM_KERNEL_STACK];
static int kernel_stack_count;

static uint64_t user_stack[NUM_KERNEL_STACK];
static int user_stack_count;

void init_stack()
{
    
}

uint64_t new_kernel_stack()
{
    
}

uint64_t new_user_stack()
{
     
}

static void free_kernel_stack(uint64_t stack_addr)
{
    //free((void*)stack_addr);
}

static void free_user_stack(uint64_t stack_addr)
{
    //free((void*)stack_addr);
}

/* Process Control Block */
void set_pcb(pid_t pid, pcb_t *pcb, task_info_t *task_info)
{
    pcb->pid         = pid;
    //pcb->name        = task_info->name;  //考虑怎样给字符串正确赋值？
    pcb->type        = task_info->type;
    pcb->status      = TASK_READY;
    
    stack_base += 0x800000;
    pcb->user_context.regs[29] = stack_base;//$29

    pcb->user_context.regs[31] = task_info->entry_point;
    queue_push(&ready_queue,pcb);
}

/* ready queue to run */
queue_t ready_queue ;

/* block queue to wait */
queue_t block_queue ;

static void check_sleeping()
{
    
}

void scheduler(void)
{
    // TODO schedule
    // Modify the current_running pointer.
    pcb_t *next_running;
    //判空，如果为空则current_running指向pcb[0]
    if(queue_is_empty(&ready_queue))
        next_running = &pcb[0];
    else
        next_running = (pcb_t*)queue_dequeue(&ready_queue);
 
    if(current_running->status != TASK_BLOCKED)//current_running->status = TASK_RUNNING
    {
        current_running->status = TASK_READY;
        if(current_running->pid != 0)
        {
            queue_push(&ready_queue, current_running);
        }
    }

    current_running = next_running;
    current_running->status = TASK_RUNNING;
    return ;
}

void do_sleep(uint32_t sleep_time)
{
    
}

void do_exit(void)
{
    
}

void do_block(queue_t *queue)
{
    // block the current_running task into the queue
    current_running->status = TASK_BLOCKED;
    queue_push(queue, (void *)current_running);
    do_scheduler();
}

void do_unblock_one(queue_t *queue)
{
     // unblock the head task from the queue
    pcb_t *item = (pcb_t *)queue_dequeue(queue);
    item->status = TASK_READY;
    queue_push(&ready_queue, item);
}

void do_unblock_all(queue_t *queue)
{
    // unblock all task in the queue
    pcb_t *item;
    while(!queue_is_empty(queue))
    {
        item = (pcb_t *)queue_dequeue(queue);
        item->status = TASK_READY;
        queue_push(&ready_queue, item);
    }
}

int do_spawn(task_info_t *task)
{
     
}

int do_kill(pid_t pid)
{
    
}

int do_waitpid(pid_t pid)
{
    
}

// process show
void do_process_show()
{

     
}

pid_t do_getpid()
{
    return current_running->pid;
}
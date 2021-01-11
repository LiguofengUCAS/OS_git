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
pid_t pcb_array_p = 1;

/* kernel stack ^_^ */
#define NUM_KERNEL_STACK 20

uint64_t stack_base = 0xffffffffa0f00000;
uint64_t stack_top  = 0xffffffffa2000000;

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
/*void set_pcb(pid_t pid, pcb_t *pcb, task_info_t *task_info)
{
    int i;
    pcb->pid         = pid;
    //pcb->name        = task_info->name;  //while 逐位赋值
    for(i=0; task_info->name[i]!='\0'; i++)
        pcb->name[i] = task_info->name[i];
    pcb->name[i] = '\0';

    pcb->type        = task_info->type;
    pcb->status      = TASK_READY;

    pcb->lock_top = 0;
    
    stack_base += 0x800000;
    //pcb->user_stack_top = stack_base;
    pcb->user_context.regs[29] = stack_base;//pcb->user_stack_top;//$29
    pcb->user_context.regs[31] = task_info->entry_point;

    queue_push(&ready_queue,pcb);
    //pcb->my_queue = &ready_queue;
}*/

/* ready queue to run */
queue_t ready_queue ;

/* block queue to wait */
queue_t block_queue ;

/* exit pcb queue to be reused */
queue_t exit_pcb_queue  ;

/* exit stack queue to be reused */
queue_t exit_kernel_stack_queue ;
queue_t exit_user_stack_queue ;

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
    {
        next_running = (pcb_t*)queue_dequeue(&ready_queue);
        next_running->my_queue = NULL;
    }
 
    if(current_running->status == TASK_RUNNING)//current_running->status = TASK_RUNNING
    {
        current_running->status = TASK_READY;
        if(current_running->pid != 0)
        {
            queue_push(&ready_queue, current_running);
            //current_running->my_queue = &ready_queue;
        }
    }
    current_running->cursor_x = screen_cursor_x;
    current_running->cursor_y = screen_cursor_y;

    current_running = next_running;
    current_running->status = TASK_RUNNING;

    screen_cursor_x = current_running->cursor_x;
    screen_cursor_y = current_running->cursor_y;
    return ;
}

pcb_t *find_pcb(uint16_t pid)
{
    int pcb_p;
    for(pcb_p = 0; pcb_p < pcb_array_p; pcb_p++)
    {
        if(pcb[pcb_p].pid == pid)
            return &pcb[pcb_p];
    }
    return NULL;
}

void do_sleep(uint32_t sleep_time)
{
    
}

void do_block(queue_t *queue)
{
    
    // block the current_running task into the queue
    current_running->status = TASK_BLOCKED;
    queue_push(queue, (void *)current_running);
    // current_running->my_queue = queue;
    do_scheduler();
    
}

void do_unblock_one(queue_t *queue)
{
    
    // unblock the head task from the queue
    if(!queue_is_empty(queue))
    {
        pcb_t *item = (pcb_t *)queue_dequeue(queue);
        item->status = TASK_READY;
        queue_push(&ready_queue, item);
    }
}

void do_unblock_all(queue_t *queue)
{
    
    // unblock all task in the block_queue
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
    pcb_t *new_pcb;
    int i;

    if(queue_is_empty(&exit_pcb_queue))
        new_pcb = &pcb[pcb_array_p++];
    else
        new_pcb = queue_dequeue(&exit_pcb_queue);
    
    //basic info
    new_pcb->pid = process_id++;
    new_pcb->status = TASK_READY;
    new_pcb->type = task->type;
    new_pcb->lock_top = 0;
    for(i = 0; task->name[i] != '\0'; i++)
        new_pcb->name[i] = task->name[i];
    new_pcb->name[i] = '\0';
    
    queue_init(&new_pcb->wait_queue);
    
    //stack
    if(queue_is_empty(&exit_kernel_stack_queue))
    { 
        stack_base += 0x10000;
        stack_top -= 0x10000;
        //$29 sp
        new_pcb->kernel_stack_top = stack_base;
        new_pcb->kernel_context.regs[29] = stack_base;
        new_pcb->user_stack_top = stack_top;
        new_pcb->user_context.regs[29] = stack_top;
    }
    else
    {
        stack_t *kernel_sp = queue_dequeue(&exit_kernel_stack_queue);
        //stack_t *user_sp   = queue_dequeue(&exit_user_stack_queue);
        new_pcb->kernel_stack_top = kernel_sp->stack_top;
        new_pcb->kernel_context.regs[29] = kernel_sp->stack_top;
        //new_pcb->user_stack_top = user_sp->stack_base;
        //p new_pcb->user_context.regs[29] = user_sp->stack_base;
    }
    
    new_pcb->kernel_context.regs[31] = task->entry_point;
    new_pcb->user_context.regs[31] = task->entry_point;

    queue_push(&ready_queue, new_pcb);

    return 0;
}

int do_kill(pid_t pid)
{
    int i;
    pcb_t *dying = find_pcb(pid);

    dying->status = TASK_EXITED;
    //remove from the task queue
    if(dying->my_queue != NULL)
    {
        queue_remove(dying->my_queue, dying);
    }
    //release lock
    for(i = 0; i < dying->lock_top; i++)
    {
        do_mutex_lock_release(dying->lock[i]);
    }
    //release wait queue
    do_unblock_all(&dying->wait_queue);
    //release stack
    stack_t kernel_sp, user_sp;
    kernel_sp.stack_top = dying->kernel_stack_top;
    kernel_sp.stack_base = kernel_sp.stack_top - 0x10000;
    //user_sp->stack_top = dying->user_stack_top;
    //user_sp->stack_base = dying->user_stack_top + 0x10000;

    queue_push(&exit_kernel_stack_queue, &kernel_sp);
    //queue_push(&exit_user_stack_queue, &user_sp);
    //release pcb
    queue_push(&exit_pcb_queue, dying);

    do_scheduler();
}

void do_exit(void)
{
    int i;
    pcb_t *exiting = current_running;

    exiting->status = TASK_EXITED;
    //remove from the task queue
    if(exiting->my_queue != NULL)
    {
        queue_remove(exiting->my_queue, exiting);
    }
    //release lock
    int lock_num = exiting->lock_top;
    for(i = 0; i < lock_num; i++)
    {
        do_mutex_lock_release(exiting->lock[i]);
    }
    //release wait queue
    do_unblock_all(&exiting->wait_queue);
    //release stack
    stack_t kernel_sp, user_sp;
    kernel_sp.stack_top = exiting->kernel_stack_top;
    kernel_sp.stack_base = kernel_sp.stack_top - 0x10000;
    //user_sp->stack_top = exiting->user_stack_top;
    //user_sp->stack_base = user_sp->stack_top + 0x10000;

    queue_push(&exit_kernel_stack_queue, &kernel_sp);
    //queue_push(&exit_user_stack_queue, &user_sp);
    //release pcb
    queue_push(&exit_pcb_queue, exiting);
    
    //do_cheduler
    do_scheduler();
}

int do_waitpid(pid_t pid)
{
    pcb_t *tmp;
    tmp = find_pcb(pid);
    if(tmp != NULL && tmp != current_running)
        do_block(&tmp->wait_queue);
    
}

// process
void do_process_show()
{
    int i, j;
    pcb_t *tmp;
    char string[20];
    
    kprintf("[PROCESS TABLE]\n");

    for (i = 0; i < 5; i++)
    {
        tmp = &pcb[i];
        status_string(tmp->status, &string);
        kprintf("PID : %d NAME: ", tmp->pid);
        
        for(j = 0; tmp->name[j] != '\0'; j++)
            kprintf("%c", tmp->name[j]);

        kprintf(" STATUS : ");
        for(j = 0; string[j] != '\0'; j++)
            kprintf("%c", string[j]);
        kprintf("\n");
    }
}

pid_t do_getpid()
{
    return current_running->pid;
}

void status_string(int status, char string[20])
{
    char block[]   = "TASK_BLOCKED";
    char running[] = "TASK_RUNNING";
    char ready[]   = "TASK_READY"  ;
    char exit[]    = "TASK_EXITED" ;

    int j;
    if(status == TASK_BLOCKED)
    {
        for(j = 0; block[j] != '\0'; j++)
            string[j] = block[j];
        string[j] = '\0';
    }
    else if(status == TASK_RUNNING)
    {
        for(j = 0; running[j] != '\0'; j++)
            string[j] = running[j];
        string[j] = '\0';
    }
    else if(status == TASK_READY)
    {
        for(j = 0; ready[j] != '\0'; j++)
            string[j] = ready[j];
        string[j] = '\0';
    }
    else if(status == TASK_EXITED)
    {
        for(j = 0; exit[j] != '\0'; j++)
            string[j] = exit[j];
        string[j] = '\0';
    }
}
#include "lock.h"
#include "sched.h"
#include "syscall.h"

int mutex_lock_id = 0;

void spin_lock_init(spin_lock_t *lock)
{
    lock->status = UNLOCKED;
}

void spin_lock_acquire(spin_lock_t *lock)
{
    while (LOCKED == lock->status)
    {
    };
    lock->status = LOCKED;
}

void spin_lock_release(spin_lock_t *lock)
{
    lock->status = UNLOCKED;
}

void do_mutex_lock_init(mutex_lock_t *lock)
{
    lock->status = UNLOCKED;
    lock->id = mutex_lock_id++;
}

void do_mutex_lock_acquire(mutex_lock_t *lock)
{
    if(lock->status == LOCKED)
    {
        do_block(&block_queue);
    }
    else
    {
        current_running->lock[current_running->lock_top++] = lock;
    }
    lock->status = LOCKED;
}

void do_mutex_lock_release(mutex_lock_t *lock)
{
    if(!queue_is_empty(&block_queue))
    {
        do_unblock_one(&block_queue);
        lock->status = UNLOCKED;
    }
    else
        lock->status = UNLOCKED;

    current_running->lock[find_lock(lock)] = NULL;
    
    current_running->lock_top--;
    
}

int find_lock(mutex_lock_t *lock)
{
    int i = 0;
    while(1)
    {
        if(current_running->lock[i] == lock || i > NUM_MAX_LOCK)
        {
            break;
        }
        else
            i++;
    }

    return i;
}

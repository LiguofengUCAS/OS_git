/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * * * * *
 *            Copyright (C) 2018 Institute of Computing Technology, CAS
 *               Author : Han Shukai (email : hanshukai@ict.ac.cn)
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * * * * *
 *                  The shell acts as a task running in user mode. 
 *       The main function is to make system calls through the user's output.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * * * * *
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this 
 * software and associated documentation files (the "Software"), to deal in the Software 
 * without restriction, including without limitation the rights to use, copy, modify, 
 * merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit 
 * persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * * * * */

#include "screen.h"
#include "stdio.h"
//#include "syscall.h"
#include "test.h"
#include "sched.h"

#ifdef P3_TEST

struct task_info task1 = {"task1", (uint64_t)&ready_to_exit_task, USER_PROCESS};
struct task_info task2 = {"task2", (uint64_t)&wait_lock_task, USER_PROCESS};
struct task_info task3 = {"task3", (uint64_t)&wait_exit_task, USER_PROCESS};

struct task_info task4 = {"task4", (uint64_t)&semaphore_add_task1, USER_PROCESS};
struct task_info task5 = {"task5", (uint64_t)&semaphore_add_task2, USER_PROCESS};
struct task_info task6 = {"task6", (uint64_t)&semaphore_add_task3, USER_PROCESS};

struct task_info task7 = {"task7", (uint64_t)&producer_task, USER_PROCESS};
struct task_info task8 = {"task8", (uint64_t)&consumer_task1, USER_PROCESS};
struct task_info task9 = {"task9", (uint64_t)&consumer_task2, USER_PROCESS};

struct task_info task10 = {"task10", (uint64_t)&barrier_task1, USER_PROCESS};
struct task_info task11 = {"task11", (uint64_t)&barrier_task2, USER_PROCESS};
struct task_info task12 = {"task12", (uint64_t)&barrier_task3, USER_PROCESS};

struct task_info task13 = {"SunQuan", (uint64_t)&SunQuan, USER_PROCESS};
struct task_info task14 = {"LiuBei", (uint64_t)&LiuBei, USER_PROCESS};
struct task_info task15 = {"CaoCao", (uint64_t)&CaoCao, USER_PROCESS};

#ifdef P4_TEST
struct task_info task16 = {"mem_test1", (uint64_t)&rw_task1, USER_PROCESS};
struct task_info task17 = {"plan", (uint64_t)&drawing_task1, USER_PROCESS};
#endif

#ifdef P5_TEST
struct task_info task18 = {"mac_send", (uint64_t)&mac_send_task, USER_PROCESS};
struct task_info task19 = {"mac_recv", (uint64_t)&mac_recv_task, USER_PROCESS};
#endif

#ifdef P6_TEST

struct task_info task19 = {"fs_test", (uint64_t)&test_fs, USER_PROCESS};
#endif
struct task_info task16 = {"multcore", (uint64_t)&test_multicore, USER_PROCESS};
static struct task_info *test_tasks[NUM_MAX_TASK] = {
    &task1,
    &task2,
    &task3,
    &task4,
    &task5,
    &task6,
    &task7,
    &task8,
    &task9,
    &task10,
    &task11,
    &task12,
    &task13,
    &task14,
    &task15,
};

#endif

int itoa(char *ch)
{
    return (*ch) - 48;
}

/* clear screen */
static void vt100_clear()
{
    // \033[2J
    kprintf("%c[2J", 27);
}

static char read_uart_ch(void)
{
    char ch = 0;
    unsigned char *read_port = (unsigned char *)(0xffffffffbfe00000 + 0x00);
    unsigned char *stat_port = (unsigned char *)(0xffffffffbfe00000 + 0x05);
    
    ch = (*stat_port & 0x01) ? *read_port : 0 ;
    
    return ch;
}

void execute(uint32_t argc, char argv[][10])
{
    if(argc == 1)
    {
        if(!strcmp(argv[0], "ps"))
        {
            do_process_show();
            //kprintf("Process_show done.\n");
        }
        else if(!strcmp(argv[0], "clear"))
        {   
            init_screen();
            screen_move_cursor(0, 15);
            kprintf("-------------------- COMMAND --------------------\n");
        }
        else
            kprintf("Unknown command!\n");
    }
    else if(argc == 2)
    {
        int pid = itoa((char *)argv[1]);
        if(!strcmp(argv[0], "exec"))
        {
            kprintf("exec process[%d].\n", pid);
            do_spawn(test_tasks[pid-1]);
            //kprintf("exec process[%d] done.\n", pid);
        }
        else if(!strcmp(argv[0], "kill"))
        {
            kprintf("kill process pid = %d.\n", pid);
            do_kill(pid);
            //kprintf("kill process %d done.\n", pid);
        }
        else
            kprintf("Unknown command!\n");
    }
    else if(argc != 0)
        kprintf("Unknown command!\n");
}

void test_shell()
{
    ////////////
    char cmd[20];
    uint32_t i = 0, argc, j, k;
    char argv[3][10];

    char ch;
    int temp_x;
    int temp_y;
    /* terminal */
    screen_move_cursor(0, 15);
    kprintf("-------------------- COMMAND --------------------\n");
    kprintf(">root@StandPowerOS: ");
    
    //ch = read_uart_ch();
    
    while (1)
    {
        // read command from UART port
        //kprintf("testing read_uart_ch\n");
        ch = read_uart_ch();

        // TODO solve command
        if(ch == 0 || (i == 0 && (ch == 0x7f || ch == 8))) //ASCII  空字符或退格符或DEL
        {
            if(i==0)
                do_scheduler();
                
            continue;
        }
        //screen_move_cursor(temp_x, temp_y);
        kprintf("%c", ch);

        if(ch != '\r')
        {
            if(ch == 8 || ch == 0x7f) // backspace
            {   
                i--;
            }
            else 
                cmd[i++] = ch;
            continue;
        }
        else
        {
            cmd[i++] = ' ';
            cmd[i] = '\0';

            // argc: number of arguments, argv[]: arguments
            j = 0;
            //跳过命令行前端的空格
            if(cmd[j] == ' ')
            {   
                while(cmd[j] == ' ' && j < i)
                    j++;
            }
            //分段读取关键字
            for(argc = k = 0; j < i; j++)
            {
                if(cmd[j] == ' ')
                {
                    argv[argc][k] = '\0';
                    argc++, k = 0;
                    while(cmd[j] == ' ' && j < i)
                        j++;
                }
                argv[argc][k++] = cmd[j];
            }

            execute(argc, argv);

            i = 0;
            kprintf(">root@StandPowerOS: ");
        }
        
        do_scheduler();
    }
}

/*
**未知原因造成的光标混乱  
*/

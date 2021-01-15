#include "sched.h"
#include "screen.h"
#include "stdio.h"
#include "syscall.h"
#include "time.h"

#include "test4.h"

#define RW_TIMES 3
int atoi(char *input)
{
	int base = 10;
    if ((input[0] == '0' && input[1] == 'x') ||
        (input[0] == '0' && input[1] == 'X')) {
        base = 16;
        input += 2;
    }
    long ret = 0;
    while (*input != '\0' && *input != '\r') {
        if ('0' <= *input && *input <= '9') {
            ret = ret * base + (*input - '0');
        } else if (base == 16) {
            if ('a' <= *input && *input <= 'f'){
                ret = ret * base + (*input - 'a' + 10);
            } else if ('A' <= *input && *input <= 'F') {
                ret = ret * base + (*input - 'A' + 10);
            } else {
                return 0;
            }
        } else {
            return 0;
        }
        ++input;
    }
    return ret;
}

int rand()

{

    int current_time = get_timer();

    return current_time % 100000;

}
// void rw_task1(char *argv[])
void rw_task1()
{
	char* argv[] = {
		"rw",
		"rw",
		"0x10000",
		"0x20000",
		"0x30000",
		"0x10000",
		"0x20000",
		"0x30000"
	};
	int mem1, mem2 = 0;
	int curs = 0;
	int memory[RW_TIMES * 2];

	int i = 0;

	// srand((uint32_t)get_ticks());
	for (i = 0; i < RW_TIMES; i++)
	{
		screen_move_cursor(1, curs + i);
		mem1 = atoi(argv[i + 2]);

		memory[i] = mem2 = 100;//rand();
		*(int *)mem1 = mem2;
		kprintf("Write: 0x%x,%d", mem1, mem2);
	}
	curs = RW_TIMES;
	for (i = 0; i < RW_TIMES; i++)
	{
		screen_move_cursor(1, curs + i);
		mem1 = atoi(argv[RW_TIMES + i + 2]);

		memory[i + RW_TIMES] = *(int *)mem1;
		if (memory[i + RW_TIMES] == memory[i])
			kprintf("Read succeed: 0x%x,%d", mem1, memory[i + RW_TIMES]);
		else
			kprintf("Read error: 0x%x,%d", mem1, memory[i + RW_TIMES]);
	}
	do_exit();
}

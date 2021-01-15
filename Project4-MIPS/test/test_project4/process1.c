#include "sched.h"
#include "screen.h"
#include "stdio.h"
#include "syscall.h"
#include "test4.h"

static char blank[] = {"                   "};
static char plane1[] = {"    ___         _  "};
static char plane2[] = {"| __\\_\\______/_| "};
static char plane3[] = {"<[___\\_\\_______| "};
static char plane4[] = {"|  o'o             "};

void drawing_task1(void)
{
    int i = 22, j = 10;
    while (1)
    {
        for (i = 60; i > 0; i--)
        {
            screen_move_cursor(i, j + 0);
            kprintf("%s", plane1);
            screen_move_cursor(i, j + 1);
            kprintf("%s", plane2);
            screen_move_cursor(i, j + 2);
            kprintf("%s", plane3);
            screen_move_cursor(i, j + 3);
            kprintf("%s", plane4);
        }
        do_scheduler();
        screen_move_cursor(1, j + 0);
        kprintf("%s", blank);
        screen_move_cursor(1, j + 1);
        kprintf("%s", blank);
        screen_move_cursor(1, j + 2);
        kprintf("%s", blank);
        screen_move_cursor(1, j + 3);
        kprintf("%s", blank);
    }
}

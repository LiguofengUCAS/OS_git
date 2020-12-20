void __attribute__((section(".entry_function"))) _start(void)
{
    void(*printch) (char c) = (void*)0xffffffff8f0d5570;
    void(*printstr)(char *string) = (void*)0xffffffff8f0d5534;
    //printstr = (void*)0xffffffff8f0d5534;
    (*printstr)("Hello OS!\n");

    volatile char *flag = (char*)0xffffffffbfe00005;
    char *input = (char*)0xffffffffbfe00000;

    while(1)
    {
        if((*flag) & 0x01 == 1)
            printch(*input);
    }
    return;
}

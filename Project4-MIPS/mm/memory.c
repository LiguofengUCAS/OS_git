#include "mm.h"

void init_page_table()
{
}
void do_TLB_Refill()
{
}

void do_page_fault()
{
}
void init_TLB(void)
{
    int i;
    uint64_t VPN2 = 0;                                  //虚页号，你想实现虚拟地址转换的地址的起始虚页号
    uint64_t PFN = 0x10000;                             //物理页号（这里我抹去了最低一位，在填cp0lo寄存器时直接写了 物理页号的起点是0.5G处，因为最低0.5G留给unmapped区
    set_cp0_pagemask(0);                                //设置物理页大小 全0则为4KB
    for(i = 0; i < 64; i++)
    {
        set_cp0_entryhi(VPN2 << 13);                    //虚页信息填入cp0hi 具体看讲义或者手册
        set_cp0_entrylo0(PFN << 7 | 0b0010111);         //偶数实页 最低为默认是0 所以PFN里没有这一位
        set_cp0_entrylo1(PFN << 7 | 0b1010111);         //奇数实页
        set_cp0_index(i);                               //填入板子的64项TLB中的第几项，类似数组的索引
        tlbwi_operation();                              //根据上述5个寄存器内容去填tlb
        VPN2 += 1;
        PFN += 1;
    }
}
void physical_frame_initial(void)
{
}

//你能给init_TLB的每个操作写一下注释吗，我回去弄清楚做了一件什么事情
//变量代表什么？
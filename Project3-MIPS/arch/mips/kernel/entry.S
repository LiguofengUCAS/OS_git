
#include "asm.h"

.equ    KERNEL, 0
.equ    USER,   312
.equ    NESTED_COUNT, 624

 
LEAF(asm_start)
    mtc0    zero, CP0_STATUS
    mtc0    zero, CP0_WATCHLO
    mtc0    zero, CP0_WATCHHI

    mfc0    t0, CP0_CONFIG
    and     t0, ~0x7
    ori     t0, 0x2
    mtc0    t0, CP0_CONFIG
    
    jr  ra
END(asm_start)

.macro SAVE_CONTEXT offset
    .set    noat
    ld      k0, current_running
	daddi	k0, k0, \offset
    sd      AT, OFFSET_REG1(k0)  
    sd      v0, OFFSET_REG2(k0)
    sd      v1, OFFSET_REG3(k0)
	sd      a0, OFFSET_REG4(k0)
	sd      a1, OFFSET_REG5(k0)
	sd      a2, OFFSET_REG6(k0)
	sd      a3, OFFSET_REG7(k0)
	sd      t0, OFFSET_REG8(k0)
	sd      t1, OFFSET_REG9(k0)
	sd      t2, OFFSET_REG10(k0)
	sd      t3, OFFSET_REG11(k0)
	sd      t4, OFFSET_REG12(k0)
	sd      t5, OFFSET_REG13(k0)
	sd      t6, OFFSET_REG14(k0)
	sd      t7, OFFSET_REG15(k0)
	sd      s0, OFFSET_REG16(k0)
	sd      s1, OFFSET_REG17(k0)
	sd      s2, OFFSET_REG18(k0)
	sd      s3, OFFSET_REG19(k0)
	sd      s4, OFFSET_REG20(k0)
	sd      s5, OFFSET_REG21(k0)
	sd      s6, OFFSET_REG22(k0)
	sd      s7, OFFSET_REG23(k0)
	sd      t8, OFFSET_REG24(k0)
	sd      t9, OFFSET_REG25(k0)
	sd      gp, OFFSET_REG28(k0)
	sd      sp, OFFSET_REG29(k0)
    sd      fp, OFFSET_REG30(k0)
	sd      ra, OFFSET_REG31(k0)
	mfc0	k1, CP0_STATUS
	nop
	sw		k1, OFFSET_STATUS(k0)
	mfhi	k1
	sd		k1, OFFSET_HI(k0)
	mflo	k1
	sd		k1, OFFSET_LO(k0)
	dmfc0	k1, CP0_BADVADDR
	nop
	sd		k1, OFFSET_BADVADDR(k0)
	mfc0	k1, CP0_CAUSE
	nop
	sw		k1, OFFSET_CAUSE(k0)
	dmfc0	k1, CP0_EPC
	nop
	sd		k1, OFFSET_EPC(k0)
    .set    at
.endm

.macro RESTORE_CONTEXT offset
    .set    noat
    ld      k0, current_running
	daddi	k0, k0, \offset
    ld      AT, OFFSET_REG1(k0)
    ld      v0, OFFSET_REG2(k0)
    ld      v1, OFFSET_REG3(k0)
	ld      a0, OFFSET_REG4(k0)
	ld      a1, OFFSET_REG5(k0)
	ld      a2, OFFSET_REG6(k0)
	ld      a3, OFFSET_REG7(k0)
	ld      t0, OFFSET_REG8(k0)
	ld      t1, OFFSET_REG9(k0)
	ld      t2, OFFSET_REG10(k0)
	ld      t3, OFFSET_REG11(k0)
	ld      t4, OFFSET_REG12(k0)
	ld      t5, OFFSET_REG13(k0)
	ld      t6, OFFSET_REG14(k0)
	ld      t7, OFFSET_REG15(k0)
	ld      s0, OFFSET_REG16(k0)
	ld      s1, OFFSET_REG17(k0)
	ld      s2, OFFSET_REG18(k0)
	ld      s3, OFFSET_REG19(k0)
	ld      s4, OFFSET_REG20(k0)
	ld      s5, OFFSET_REG21(k0)
	ld      s6, OFFSET_REG22(k0)
	ld      s7, OFFSET_REG23(k0)
	ld      t8, OFFSET_REG24(k0)
	ld      t9, OFFSET_REG25(k0)
	ld      gp, OFFSET_REG28(k0)
	ld      sp, OFFSET_REG29(k0)
    ld      fp, OFFSET_REG30(k0)
	ld      ra, OFFSET_REG31(k0)
	lw		k1, OFFSET_STATUS(k0)
	#ori		k1, 0x2
	#xori	k1, 0x2
	mtc0	k1, CP0_STATUS
	ld		k1, OFFSET_HI(k0)
	mthi	k1
	ld		k1, OFFSET_LO(k0)
	mtlo	k1
	ld		k1, OFFSET_BADVADDR(k0)
	dmtc0	k1, CP0_BADVADDR
	lw		k1, OFFSET_CAUSE(k0)
	mtc0	k1, CP0_CAUSE
	ld		k1, OFFSET_EPC(k0)
	dmtc0	k1, CP0_EPC
   .set    at
.endm

# function do_scheduler
NESTED(do_scheduler, 0, ra)
     SAVE_CONTEXT(KERNEL)
	 jal scheduler
	 RESTORE_CONTEXT(KERNEL)
	 jr ra

END(do_scheduler)

LEAF(reset_timer)
    
END(reset_timer)

LEAF(set_cp0_status)
    
END(set_cp0_status)

LEAF(set_cp0_cause)
   
END(set_cp0_cause)

LEAF(get_cp0_status)
    
END(get_cp0_status)

LEAF(get_cp0_cause)
    
END(get_cp0_cause)

LEAF(get_cp0_count)
	 
END(get_cp0_count)

LEAF(get_cp0_compare)
	 
END(get_cp0_compare)

 
.global exception_handler_begin
.global exception_handler_end
.global TLBexception_handler_begin
.global TLBexception_handler_end

NESTED(TLBexception_handler_entry, 0, sp)
TLBexception_handler_begin:
	 
 
TLBexception_handler_end:
END(TLBexception_handler_entry)

NESTED(exception_handler_entry, 0, sp)   
exception_handler_begin:
    
exception_handler_end:
END(exception_handler_entry)

NESTED(handle_int, 0, sp)
    
    
END(handle_int)

NESTED(handle_syscall, 0, sp)
    
END(handle_syscall)

NESTED(handle_tlb, 0, sp)
   
END(handle_tlb)

NESTED(handle_other, 0, sp)
    
   
END(handle_other)

LEAF(exception_handler_exit)
  
END(exception_handler_exit)

LEAF(set_cp0_entryhi)
   
END(set_cp0_entryhi)


LEAF(get_cp0_index)
    
END(get_cp0_index)

LEAF(set_cp0_index)
    
END(set_cp0_index)

LEAF(get_cp0_badvaddr)
    
END(get_cp0_badvaddr)

LEAF(get_cp0_entrylo0)
    
END(get_cp0_entrylo0)

LEAF(set_cp0_entrylo0)
    
END(set_cp0_entrylo0)

LEAF(get_cp0_entrylo1)
    
END(get_cp0_entrylo1)

LEAF(set_cp0_entrylo1)
   
END(set_cp0_entrylo1)

LEAF(set_cp0_pagemask)
    
END(set_cp0_pagemask)

LEAF(tlbwr_operation)
   
END(tlbwr_operation)

LEAF(tlbwi_operation)
    
END(tlbwi_operation)

LEAF(tlbp_operation)
    
END(tlbp_operation)



LEAF(get_cp0_config)
    
END(get_cp0_config)

LEAF(get_cpu_id)
    
END(get_cpu_id)


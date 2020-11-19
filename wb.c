#include "debug_rv32i_pp_ip.h"
#include "rv32i_pp_ip.h"
#ifndef __SYNTHESIS__
#ifdef DEBUG_FETCH
#include <stdio.h>
#endif
#ifdef DEBUG_DISASSEMBLE
#include "disassemble.h"
#endif
#ifdef DEBUG_EMULATE
#include "emulate.h"
#endif
#endif

void wb(
  mem_stage_t wb_in,
  int *reg_file,
  unsigned int *nbi,
  bit_t *exit_cond){
#pragma HLS INLINE off
  if (!wb_in.cancel){
    if (!wb_in.has_no_dest)
      reg_file[wb_in.rd] = wb_in.result;
    *exit_cond = wb_in.is_ret && wb_in.result == 0;
#ifndef __SYNTHESIS__
#ifdef DEBUG_FETCH
    printf("%04d: %08x      ", (unsigned int)wb_in.pc,
                               (unsigned int)wb_in.instruction);
#ifndef DEBUG_DISASSEMBLE
    printf("\n");
#endif
#endif
#ifdef DEBUG_DISASSEMBLE
    disassemble(wb_in.pc,
                wb_in.instruction,
                wb_in.d_i);
#endif
#ifdef DEBUG_EMULATE
    emulate(reg_file,
            wb_in.d_i,
            wb_in.next_pc);
#endif
#endif
    *nbi = *nbi + 1;
  }
  else *exit_cond = 0;
}

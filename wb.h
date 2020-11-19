#ifndef __WB
#define __WB

#include "rv32i_pp_ip.h"

void wb(
  mem_stage_t wb_in,
  int *reg_file,
  unsigned int *nbi,
  bit_t *exit_cond);

#endif

#ifndef __MEM
#define __MEM
#include "rv32i_pp_ip.h"

void mem_access(
  execute_stage_t mem_in,
  char *data_mem_0,
  char *data_mem_1,
  char *data_mem_2,
  char *data_mem_3,
  mem_stage_t *mem_out);

#endif

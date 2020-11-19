#ifndef __NEW_CYCLE
#define __NEW_CYCLE

#include "rv32i_pp_ip.h"

void new_cycle(
  fetch_decode_stage_t fetch_out,
  execute_stage_t execute_out,
  mem_stage_t mem_out,
  fetch_decode_stage_t *execute_in,
  execute_stage_t *mem_in,
  mem_stage_t *wb_in);

#endif

#ifndef __FETCH
#define __FETCH

#include "rv32i_pp_ip.h"

void fetch(
  fetch_decode_stage_t *fetch_out,
  instruction_t *code_mem,
  instruction_t *instruction,
  bit_t execute_set_pc,
  code_address_t target_pc);

#endif

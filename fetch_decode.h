#ifndef __FETCH_DECODE
#define __FETCH_DECODE

#include "rv32i_pp_ip.h"

void fetch_decode(
  fetch_decode_stage_t *fetch_out,
  unsigned int *code_mem,
  bit_t execute_set_pc,
  code_address_t target_pc);

#endif

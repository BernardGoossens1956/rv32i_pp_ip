#include "rv32i_pp_ip.h"

void fetch(
  fetch_decode_stage_t *fetch_out,
  instruction_t *code_mem,
  instruction_t *instruction,
  bit_t execute_set_pc,
  code_address_t target_pc){
#pragma HLS INLINE
  fetch_out->pc = (execute_set_pc)?
                   target_pc:
                   fetch_out->pc + 4;
  *instruction = code_mem[fetch_out->pc >> 2];
}

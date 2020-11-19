#include "debug_rv32i_pp_ip.h"
#include "rv32i_pp_ip.h"
#include "fetch.h"
#include "decode.h"

void fetch_decode(
  fetch_decode_stage_t *fetch_out,
  unsigned int *code_mem,
  bit_t execute_set_pc,
  code_address_t target_pc){
#pragma HLS INLINE off
  instruction_t instruction;
  fetch(fetch_out,
        code_mem,
        &instruction,
        execute_set_pc,
        target_pc);
  decode(instruction,
         &(fetch_out->decoded_instruction));
#ifndef __SYNTHESIS__
#ifdef DEBUG_DISASSEMBLE
  fetch_out->instruction = instruction;
#endif
#endif
}

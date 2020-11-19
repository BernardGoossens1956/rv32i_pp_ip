#ifndef __EXECUTE
#define __EXECUTE

#include "rv32i_pp_ip.h"

void execute(
  int *reg_file,
  fetch_decode_stage_t execute_in,
  bit_t mem_cancel,
  bit_t wb_cancel,
  bit_t mem_has_no_dest,
  bit_t wb_has_no_dest,
  reg_num_t mem_rd,
  reg_num_t wb_rd,
  int mem_result,
  int wb_result,
  fetch_decode_stage_t *fetch_out,
  execute_stage_t *execute_out,
  bit_t *execute_set_pc,
  code_address_t *target_pc);

#endif

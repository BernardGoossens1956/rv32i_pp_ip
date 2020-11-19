#ifndef __COMPUTE
#define __COMPUTE

#include "rv32i_pp_ip.h"

int read_reg(
  int *reg_file,
  reg_num_t rs);
bit_t compute_branch_result(
  int rv1,
  int rv2,
  func3_t func3);
int compute_op_result(
  int rv1,
  int rv2,
  decoded_instruction_t d_i);
int compute_result(
  int rv1,
  int op_result,
  fetch_decode_stage_t fetch_decode);
code_address_t compute_next_pc(
  fetch_decode_stage_t fetch_decode,
  int rv1,
  bit_t cond,
  bit_t *execute_wb_set_pc);

#endif

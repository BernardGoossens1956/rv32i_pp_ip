#include "debug_rv32i_pp_ip.h"
#include "rv32i_pp_ip.h"
#include "compute.h"

int by_pass(
  bit_t cancel,
  reg_num_t rd,
  int mem_result,
  int wb_result,
  reg_num_t rs){
#pragma HLS INLINE
  bit_t by_pass_rv_from_mem_in;
  by_pass_rv_from_mem_in = !cancel && rs!=0 && rs == rd;
  if (by_pass_rv_from_mem_in) return mem_result;
  else return wb_result;
}
void get_source(
  int r1,
  int r2,
  fetch_decode_stage_t execute_in,
  bit_t mem_cancel,
  bit_t wb_cancel,
  bit_t mem_has_no_dest,
  bit_t wb_has_no_dest,
  reg_num_t mem_rd,
  reg_num_t wb_rd,
  int mem_result,
  int wb_result,
  int *rv1,
  int *rv2){
#pragma HLS INLINE
  bit_t c1, c2, c3, c4, c5, c6;
  bit_t no_by_pass_rs1, no_by_pass_rs2;
  reg_num_t rs1, rs2;
  bit_t cancel;
  cancel = execute_in.cancel;
  c1 = !cancel && execute_in.decoded_instruction.is_jump;
  c2 = !cancel && execute_in.decoded_instruction.is_branch;
  rs1 = execute_in.decoded_instruction.rs1;
  rs2 = execute_in.decoded_instruction.rs2;
  c3 = (mem_cancel      ||
        mem_has_no_dest ||
        rs1==0          ||
        rs1!=mem_rd);
  c4 = (wb_cancel       ||
        wb_has_no_dest  ||
        rs1==0          ||
        rs1!=wb_rd);
  no_by_pass_rs1 = c3 && c4;
  c5 = (mem_cancel      ||
        mem_has_no_dest ||
        rs2==0          ||
        rs2!=mem_rd);
  c6 = (wb_cancel       ||
        wb_has_no_dest  ||
        rs2==0          ||
        rs2!=wb_rd);
  no_by_pass_rs2 = c5 && c6;
  *rv1 = (no_by_pass_rs1)?r1:
          by_pass(mem_cancel,
                  mem_rd,
                  mem_result, wb_result,
                  rs1);
  *rv2 = (no_by_pass_rs2)?r2:
          by_pass(mem_cancel,
                  mem_rd,
                  mem_result, wb_result,
                  rs2);
}

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
  code_address_t *target_pc){
#pragma HLS INLINE off
  int r1, r2, rv1, rv2, rs, op_result, result;
  reg_num_t rs1, rs2;
  bit_t bcond, set_pc, c1, c2, cancel;

  rs1 = execute_in.decoded_instruction.rs1;
  rs2 = execute_in.decoded_instruction.rs2;
  r1 = read_reg(reg_file, rs1);
  r2 = read_reg(reg_file, rs2);
  get_source(r1, r2, execute_in,
             mem_cancel,      wb_cancel,
             mem_has_no_dest, wb_has_no_dest,
             mem_rd,          wb_rd,
             mem_result,      wb_result,
             &rv1, &rv2);

  cancel = execute_in.cancel;
  execute_out->cancel = cancel;
  c1 = !cancel && execute_in.decoded_instruction.is_jump;
  c2 = !cancel && execute_in.decoded_instruction.is_branch;

  bcond = compute_branch_result(
          rv1,
          rv2,
          execute_in.decoded_instruction.func3);

  fetch_out->cancel = c1 || (c2 && bcond);

  rs = (execute_in.decoded_instruction.type == R_TYPE)?
        rv2:
   (int)execute_in.decoded_instruction.imm;
  op_result = compute_op_result(
    rv1,
    rs,
    execute_in.decoded_instruction);
  result = compute_result(
    rv1,
    op_result,
    execute_in);

  *target_pc = compute_next_pc(
    execute_in,
    rv1,
    bcond,
    &set_pc);

  *execute_set_pc = set_pc && !cancel;

  execute_out->rd          =
    execute_in.decoded_instruction.rd;
  execute_out->func3       =
    execute_in.decoded_instruction.func3;
  execute_out->is_branch   =
    execute_in.decoded_instruction.is_branch;
  execute_out->is_load     =
    execute_in.decoded_instruction.is_load;
  execute_out->is_store    =
    execute_in.decoded_instruction.is_store;
  execute_out->is_ret      =
    execute_in.decoded_instruction.is_ret;
  execute_out->has_no_dest =
    execute_in.decoded_instruction.has_no_dest;
  execute_out->result      =
   (execute_in.decoded_instruction.is_ret)?
    *target_pc:
    result;
  execute_out->rv2 = rv2;
#ifndef __SYNTHESIS__
#ifdef DEBUG_DISASSEMBLE
  execute_out->instruction =
    execute_in.instruction;
  execute_out->pc          =
    execute_in.pc;
  execute_out->d_i         =
    execute_in.decoded_instruction;
#endif
#ifdef DEBUG_EMULATE
#ifndef DEBUG_DISASSEMBLE
  execute_out->d_i = execute_in.decoded_instruction;
#endif
  execute_out->next_pc = *target_pc;
#endif
#endif
}

#include "ap_cint.h"
#include "rv32i_pp_ip.h"

int read_reg(
  int *reg_file,
  reg_num_t rs){
#pragma HLS INLINE
  return reg_file[rs];
}
bit_t compute_branch_result(
  int rv1,
  int rv2,
  func3_t func3){
#pragma HLS INLINE
  bit_t result;
  switch(func3){
    case BEQ : result = (rv1 == rv2);
               break;
    case BNE : result = (rv1 != rv2);
               break;
    case 2   :
    case 3   : result = 0;
               break;
    case BLT : result = (rv1 <  rv2);
               break;
    case BGE : result = (rv1 >= rv2);
               break;
    case BLTU: result =
               ((unsigned int)rv1 <  (unsigned int)rv2);
               break;
    case BGEU: result =
               ((unsigned int)rv1 >= (unsigned int)rv2);
               break;
  }
  return result;
}
int compute_op_result(
  int rv1,
  int rv2,
  decoded_instruction_t d_i){
#pragma HLS INLINE
  bit_t f7_6   = d_i.func7>>5;
  bit_t r_type = d_i.type == R_TYPE;
  uint5 shift;
  int result;
  if (r_type)
    shift = rv2;
  else//I_TYPE
    shift = ((uint5)d_i.inst_24_21)<<1 |
             (uint5)d_i.inst_20;
  switch(d_i.func3){
    case ADD : if (r_type && f7_6) result = rv1 - rv2;//SUB
               else result = rv1 + rv2;
               break;
    case SLL : result = rv1 << shift;
               break;
    case SLT : result = rv1 < rv2;
               break;
    case SLTU: result = (unsigned int)rv1 < (unsigned int)rv2;
               break;
    case XOR : result = rv1 ^ rv2;
               break;
    case SRL : if (f7_6) result = rv1 >> shift;//SRA
               else result = (unsigned int)rv1 >> shift;
               break;
    case OR  : result = rv1 | rv2;
               break;
    case AND : result = rv1 & rv2;
               break;
  }
  return result;
}
int compute_result(
  int rv1,
  int op_result,
  fetch_decode_stage_t fetch_decode){
#pragma HLS INLINE
  code_address_t pc4 = fetch_decode.pc + 4;
  int result;
  switch(fetch_decode.decoded_instruction.type){
    case R_TYPE:
      result = op_result;
      break;
    case I_TYPE:
      if (fetch_decode.decoded_instruction.is_jalr)
        result = (unsigned int)pc4;
      else if (fetch_decode.decoded_instruction.is_load)
        result = rv1 + (int)fetch_decode.decoded_instruction.imm;
      else if (fetch_decode.decoded_instruction.is_op_imm)
        result = op_result;
      else
        result = 0;//(d_i.opcode == SYSTEM)
      break;
    case S_TYPE:
      result = rv1 + (int)fetch_decode.decoded_instruction.imm;
      break;
    case B_TYPE:
      result = 0;
      break;
    case U_TYPE:
      if (fetch_decode.decoded_instruction.is_lui)
        result = (unsigned int)
                 ((u_uimmediate_t)
                  (fetch_decode.decoded_instruction.imm<<12));
      else//d_i.opcode == AUIPC
        result = (int)fetch_decode.pc +
                 (int)
                 ((u_uimmediate_t)
                  (fetch_decode.decoded_instruction.imm<<12));
      break;
    case J_TYPE:
      result = (unsigned int)pc4;
      break;
    default:
      result = 0;
      break;
  }
  return result;
}
code_address_t compute_next_pc(
  fetch_decode_stage_t fetch_decode,
  int rv1,
  bit_t cond,
  bit_t *execute_set_pc){
#pragma HLS INLINE
  code_address_t next_pc;
  code_address_t pc4 = fetch_decode.pc + 4;
  code_address_t j_b_target_pc = (fetch_decode.pc +
           (code_address_t)
           (((int)fetch_decode.decoded_instruction.imm<<1)));
  code_address_t i_target_pc = ((rv1 +
             (int)fetch_decode.decoded_instruction.imm)&0xfffffffe);
  bit_t set_pc;
  switch(fetch_decode.decoded_instruction.type){
    case R_TYPE:
      set_pc = 0;
      next_pc = pc4;
      break;
    case I_TYPE:
      set_pc = (fetch_decode.decoded_instruction.is_jalr);
      next_pc = (set_pc)?i_target_pc:pc4;
      break;
    case S_TYPE:
      set_pc = 0;
      next_pc = pc4;
      break;
    case B_TYPE:
      set_pc = cond;
      next_pc = (cond)?j_b_target_pc:pc4;
      break;
    case U_TYPE:
      set_pc = 0;
      next_pc = pc4;
      break;
    case J_TYPE:
      set_pc = 1;
      next_pc = j_b_target_pc;
      break;
    default:
      set_pc = 0;
      next_pc = pc4;
      break;
  }
  *execute_set_pc = set_pc;
  return next_pc;
}

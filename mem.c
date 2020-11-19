#include "ap_cint.h"
#include "debug_rv32i_pp_ip.h"
#include "rv32i_pp_ip.h"

int mem_load(
  char *data_mem_0,
  char *data_mem_1,
  char *data_mem_2,
  char *data_mem_3,
  data_address_t address,
  func3_t msize){
#pragma HLS INLINE
  uint2          a01 =  address;
  bit_t          a1  = (address >> 1);
  data_address_t a2  = (address >> 2)&(DATA_MEM_SIZE/4 - 1);
  int            result;
  char           b, b0, b1, b2, b3;
  unsigned char  ub, ub0, ub1, ub2, ub3;
  short          h, h0, h1;
  unsigned short uh, uh0, uh1;
  int            w, ib, ih;
  unsigned int   iub, iuh;
  b0 = *(data_mem_0 + a2);
  ub0 = b0;
  b1 = *(data_mem_1 + a2);
  ub1 = b1;
  h0 = ((uint16)ub1 <<  8) |  (uint16)ub0;
  uh0 = h0;
  b2 = *(data_mem_2 + a2);
  ub2 = b2;
  b3 = *(data_mem_3 + a2);
  ub3 = b3;
  h1 = ((uint16)ub3 <<  8) |  (uint16)ub2;
  uh1 = h1;
  switch(a01){
    case 0b00: b = b0; break;
    case 0b01: b = b1; break;
    case 0b10: b = b2; break;
    case 0b11: b = b3; break;
  }
  ub = b;
  ib = (int)b;
  iub = (unsigned int)ub;
  h = (a1)?h1:h0;
  uh = h;
  ih = (int)h;
  iuh = (unsigned int)uh;
  w = (uh1 << 16) | uh0;
  switch(msize){
    case LB:
      result = ib;  break;
    case LH:
      result = ih;  break;
    case LW:
      result = w;   break;
    case 3:
      result = 0;   break;
    case LBU:
      result = iub; break;
    case LHU:
      result = iuh; break;
    case 6:
    case 7:
      result = 0;   break;
  }
  return result;
}
void mem_store(
  char *data_mem_0,
  char *data_mem_1,
  char *data_mem_2,
  char *data_mem_3,
  data_address_t address,
  int rv2,
  uint2 msize){
#pragma HLS INLINE
  uint2 a01 =  address;
  bit_t a1  = (address >> 1);
  data_address_t a2 = (address >> 2)&(DATA_MEM_SIZE/4 - 1);
  char  rv2_0, rv2_1, rv2_2, rv2_3;
  rv2_0 = rv2;
  rv2_1 = rv2>>8;
  rv2_2 = rv2>>16;
  rv2_3 = rv2>>24;
  switch(msize){
    case SB:
      switch(a01){
        case 0b00:
          *(data_mem_0 + a2) = rv2_0;
          break;
        case 0b01:
          *(data_mem_1 + a2) = rv2_0;
          break;
        case 0b10:
          *(data_mem_2 + a2) = rv2_0;
          break;
        case 0b11:
          *(data_mem_3 + a2) = rv2_0;
          break;
      }
      break;
    case SH:
      if (a1 == 0){
        *(data_mem_0 + a2) = rv2_0;
        *(data_mem_1 + a2) = rv2_1;
      }
      else{
        *(data_mem_2 + a2) = rv2_0;
        *(data_mem_3 + a2) = rv2_1;
      }
      break;
    case SW:
      *(data_mem_0 + a2) = rv2_0;
      *(data_mem_1 + a2) = rv2_1;
      *(data_mem_2 + a2) = rv2_2;
      *(data_mem_3 + a2) = rv2_3;
      break;
    case 3:
      break;
  }
}
void mem_access(
  execute_stage_t mem_in,
  char *data_mem_0,
  char *data_mem_1,
  char *data_mem_2,
  char *data_mem_3,
  mem_stage_t *mem_out){
#pragma HLS INLINE off
  data_address_t address = mem_in.result;
  mem_out->cancel = mem_in.cancel;
  if (!mem_in.cancel){
    mem_out->result = (mem_in.is_load)?
      mem_load(data_mem_0,
               data_mem_1,
               data_mem_2,
               data_mem_3,
               address,
               mem_in.func3):
      mem_in.result;
    if (mem_in.is_store)
      mem_store(data_mem_0,
                data_mem_1,
                data_mem_2,
                data_mem_3,
                address,
                mem_in.rv2,
         (uint2)mem_in.func3);
    mem_out->rd = mem_in.rd;
    mem_out->is_ret = mem_in.is_ret;
    mem_out->has_no_dest = mem_in.has_no_dest;
#ifndef __SYNTHESIS__
#ifdef DEBUG_DISASSEMBLE
    mem_out->instruction = mem_in.instruction;
    mem_out->pc = mem_in.pc;
    mem_out->d_i = mem_in.d_i;
#endif
#ifdef DEBUG_EMULATE
#ifndef DEBUG_DISASSEMBLE
    mem_out->d_i = mem_in.d_i;
#endif
    mem_out->next_pc = mem_in.next_pc;
#endif
#endif
  }
}

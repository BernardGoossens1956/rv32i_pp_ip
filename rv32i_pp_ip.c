#include "ap_cint.h"
#include "rv32i_pp_ip.h"
#include "new_cycle.h"
#include "fetch_decode.h"
#include "execute.h"
#include "mem.h"
#include "wb.h"

void init_reg_file(int *reg_file){
#pragma HLS INLINE off
  uint6 i;
  for (i = 0; i < NB_REGISTER; i++)
    reg_file[i] = 0;
}
void rv32i_pp_ip(
  unsigned int  pc,
  unsigned int  code_mem[CODE_MEM_SIZE/sizeof(instruction_t)],
  char          data_mem_0[DATA_MEM_SIZE/sizeof(int)],
  char          data_mem_1[DATA_MEM_SIZE/sizeof(int)],
  char          data_mem_2[DATA_MEM_SIZE/sizeof(int)],
  char          data_mem_3[DATA_MEM_SIZE/sizeof(int)],
  unsigned int *nb_instruction,
  unsigned int *nb_cycle){
#pragma HLS INTERFACE s_axilite port=pc
#pragma HLS INTERFACE s_axilite port=code_mem
#pragma HLS INTERFACE s_axilite port=data_mem_0
#pragma HLS INTERFACE s_axilite port=data_mem_1
#pragma HLS INTERFACE s_axilite port=data_mem_2
#pragma HLS INTERFACE s_axilite port=data_mem_3
#pragma HLS INTERFACE s_axilite port=nb_instruction
#pragma HLS INTERFACE s_axilite port=nb_cycle
#pragma HLS INTERFACE ap_ctrl_none port=return
  int                  reg_file[NB_REGISTER];
#pragma HLS ARRAY_PARTITION variable=reg_file dim=1 complete
  code_address_t       target_pc;
  fetch_decode_stage_t fetch_out;
  fetch_decode_stage_t execute_in;
  execute_stage_t      execute_out;
  execute_stage_t      mem_in;
  mem_stage_t          mem_out;
  mem_stage_t          wb_in;
  unsigned int         nbi;
  unsigned int         nbc;
  bit_t                exit_cond;
  bit_t                execute_set_pc;
  init_reg_file(reg_file);
  fetch_out.cancel   = 1;
  execute_out.cancel = 1;
  mem_out.cancel     = 1;
  execute_out.rd     = 0;
  mem_out.rd         = 0;
  execute_set_pc     = 1;
  target_pc          = pc;
  fetch_out.pc       = pc-4;
  nbi                = 0;
  nbc                = 0;
  exit_cond          = 0;
  while(!exit_cond){
#pragma HLS PIPELINE II=3
    new_cycle(fetch_out, execute_out, mem_out,
              &execute_in, &mem_in, &wb_in);
    fetch_decode(&fetch_out,
                 code_mem,
                 execute_set_pc,
                 target_pc);
    execute(reg_file,
            execute_in,
            mem_in.cancel,
            wb_in.cancel,
            mem_in.has_no_dest,
            wb_in.has_no_dest,
            mem_in.rd,
            wb_in.rd,
            mem_in.result,
            wb_in.result,
           &fetch_out,
           &execute_out,
           &execute_set_pc,
           &target_pc);
    mem_access(mem_in,
               data_mem_0,
               data_mem_1,
               data_mem_2,
               data_mem_3,
               &mem_out);
    wb(wb_in, reg_file, &nbi, &exit_cond);
    nbc = nbc + 1;
  };
  *nb_instruction = nbi;
  *nb_cycle = nbc;
}

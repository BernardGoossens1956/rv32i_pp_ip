#include <stdio.h>
#include "rv32i_pp_ip.h"
#include "print.h"

//#include "test_branch_dump.h"
//#include "test_jal_jalr_dump.h"
//#include "test_load_store_dump.h"
//#include "test_lui_auipc_dump.h"
//#include "test_mem_dump.h"
//#include "test_op_dump.h"
//#include "test_op_imm_dump.h"
#include "test_sum_dump.h"

char data_mem_0[DATA_MEM_SIZE/4];
char data_mem_1[DATA_MEM_SIZE/4];
char data_mem_2[DATA_MEM_SIZE/4];
char data_mem_3[DATA_MEM_SIZE/4];

int main() {
  unsigned int i;
  unsigned int pc;
  unsigned int nbi;
  unsigned int nbc;
  unsigned char b0, b1, b2, b3;
  int w;
  pc = 0;
  rv32i_pp_ip(pc,
              code_mem,
              data_mem_0,
              data_mem_1,
              data_mem_2,
              data_mem_3,
             &nbi,
             &nbc);
  printf("\n%d fetched and decoded instructions\
 in %d cycles (ipc = %2.2f)\n", nbi, nbc, ((float)nbi)/nbc);
  printf("\nregister file dump\n");
  for (i=1; i<NB_REGISTER; i++){
    b0 = data_mem_0[i];
    b1 = data_mem_1[i];
    b2 = data_mem_2[i];
    b3 = data_mem_3[i];
    w = (b3<<24) | (b2<<16) | (b1<<8) | b0;
    print_reg_name(i);
    printf(" ");
  #ifdef SYMB_REG
    if (i!=26 && i!=27) printf(" ");
  #else
    if (i<10) printf(" ");
  #endif
    printf("= %16d (%8x)\n", w, (unsigned int)w);
  }
  printf("\ndata memory dump (non null words)\n");
  for (i=NB_REGISTER; i<DATA_MEM_SIZE/4; i++){
    b0 = data_mem_0[i];
    b1 = data_mem_1[i];
    b2 = data_mem_2[i];
    b3 = data_mem_3[i];
    w = (b3<<24) | (b2<<16) | (b1<<8) | b0;
    if (w != 0)
      printf("m[%4d] = %16d (%8x)\n", i, w, (unsigned int)w);
  }
  return 0;
}

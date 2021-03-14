/*
 * @Descripttion : 
 * @Version      : 
 * @Autor        : one30: one30@m.scnu.edu.cn(email)
 * @Date         : 2021-03-11 11:24:37
 * @LastEditTime : 2021-03-14 13:48:25
 * @FilePath     : /src/neon_test.c
 */
#include <stdio.h>
#include <stdlib.h>
//#include "x86intrin.h"
#include <stdint.h>

#include "Neon.h"

// void print64bin (const uint64_t n) {
//   for (int i = 1; i <= 64; i++) {
//     printf("%lu",(n>>(64-i)) & 1);
//   }  
// }

// void print128bin (const uint64x2_t v) {
//   uint64_t out[2];
//   vst1q_u64((uint64_t*)out,v);
//   for (int i = 0; i <= 1; i++) {
//     print64bin(out[i]);
//   }
//   puts("");
// }

void print64bin (uint64_t n) {
  for (int i = 1; i <= 64; i++) {
    printf("%lu",(n>>(64-i)) & 1);
  }  
}

void print128bin (uint64x2_t v) {
  uint64_t out[2];
  vst1q_u64((uint64_t*)out,v);
  for (int i = 0; i <= 1; i++) {
    print64bin(out[i]);
  }
  puts("");
}


void check_ortho () {
  
  DATATYPE buffer[128];

  for (uint64_t i = 0, a = -1; i < 64; i++) {
    buffer[i] = (uint64x2_t){ -1,  a };
    // a -= (uint64_t)1 << (63-i);
  }
  // for (uint64_t i = 0, a = -1; i < 64; i++) {
  //   buffer[i+64] = (uint64x2_t){ a,  0 };
  //   a -= (uint64_t)1 << (63-i);
  // }

  for (int i = 0; i < 128; i++)
    print128bin(buffer[i]);

  real_ortho_128x128(buffer);

  printf("\n\n\n");
  for (int i = 0; i < 10; i++)
    puts("*******************************************************************************");
  printf("\n\n\n");
  
  for (int i = 0; i < 128; i++)
    print128bin(buffer[i]); 

}


void neon_sample_test() {
    DATATYPE buffer[128];
    for (uint64_t i = 0, a = -1; i < 4; i++) {
        buffer[i] = (uint64x2_t){ -1,  a };
    }
    uint64_t out[2];
    for (uint64_t i = 0, a = -1; i < 4; i++) {
        vst1q_u64((uint64_t*)out,buffer[i]);
        printf("%x %x", out[0],out[1]);
        printf("\n");
    }
}

void real_ortho_test()
{
  //  uint64_t m[4];
  //  for(int i=0; i<4; i++)
  //  {
  //    m[i] = 0x0123456789abcdef;
  //    printf("%16x",m[i]);
  //    printf("%16lu",m[i]);
  //    printf("\t");
  //  }
  //  printf("\n");
   uint64x2_t m[4];
   int i = -1;
   printf("%d\t",i);
   printf("%x\t",i);
   uint64_t n[2] = {0x0123456789abcdef,0x0123456789abcdef};
   uint64_t tmp[2];
   for(int i=0; i<4; i++)
   {	
	  m[i] = vld1q_u64((uint64_t*)n);
    
   	printf("%d\t",m[i]);
   	printf("%x\t",m[i]);
   }
}




int main() {

  
  neon_sample_test();
  real_ortho_test();
 // check_ortho();
  return 0;
  
}

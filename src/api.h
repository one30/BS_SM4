/*
 * @Descripttion : 
 * @Version      : 
 * @Autor        : one30: one30@m.scnu.edu.cn(email)
 * @Date         : 2021-03-14 13:34:33
 * @LastEditTime : 2021-03-14 14:50:00
 * @FilePath     : /src/api.h
 */
#include "Neon.h"

void sm4_bs(DATATYPE plain[128], DATATYPE key[32][32], DATATYPE cipher[128]);
void SM4__(DATATYPE plain[128],DATATYPE BS_RK_256[32][32],DATATYPE cipher[128]);
void S_box(int round,DATATYPE buf[36][32]);
void Sm4_BoolFun(bits in, bit_t *out0, bit_t *out1, bit_t *out2, bit_t *out3, bit_t *out4, bit_t *out5, bit_t *out6, bit_t *out7);
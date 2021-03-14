/*
 * @Descripttion : 
 * @Version      : 
 * @Autor        : one30: one30@m.scnu.edu.cn(email)
 * @Date         : 2021-03-14 13:42:08
 * @LastEditTime : 2021-03-14 14:49:00
 * @FilePath     : /src/sm4_bs_neon.c
 */
#include "api.h"

void sm4_bs(DATATYPE plain[128], DATATYPE key[32][32], DATATYPE cipher[128]) {
  
  real_ortho_128x128(plain);
  
  SM4__(plain,key,cipher);

  real_ortho_128x128(cipher);
//   for (int i = 0; i < 128; i++)
//     cipher[i] = _mm256_shuffle_epi8(cipher[i],
//                                     _mm256_set_epi8(8,9,10,11,12,13,14,15,0,1,2,3,4,5,6,7,
//                                                     8,9,10,11,12,13,14,15,0,1,2,3,4,5,6,7));

}

void SM4__(DATATYPE plain[128],DATATYPE BS_RK_256[32][32],DATATYPE cipher[128])
{
    int i = 0;
    DATATYPE buf[36][32];
    DATATYPE N_temp[128];
    DATATYPE temp[36][32];

    for(int j = 0; j < 4; j++)
    {
        for(int k = 0; k < 32; k++)
        {
            buf[j][k] = plain[32*j+k];//load data
        }     
    }
        
    while(i < 32)//32轮迭代计算
    {
        for(int j = 0; j < 32; j++)//4道32bit数据操作:
        {
            buf[4+i][j]= buf[i+1][j] ^ buf[i+2][j] ^ buf[i+3][j] ^ BS_RK_256[i][j];
        }

        S_box(i,buf);//bingo256 合成置换T的非线性变换
        
        //printf("\tafter shift\n");
        for(int j = 0; j < 32; j++)//bingo256 4道32bit数据操作:合成置换T的线性变换L
        {
            temp[4+i][j]= buf[4+i][j] ^ buf[4+i][(j+2)%32] ^ buf[4+i][(j+10)%32] ^ buf[4+i][(j+18)%32] ^ buf[4+i][(j+24)%32];
        }
        for(int j = 0; j < 32; j++)//4道32bit数据操作
        {
            buf[4+i][j]= temp[i+4][j] ^ buf[i][j];
        }        
        i++;
    }

    for(int j = 0; j < 4; j++)//反序计算
    {
        for(int k = 0; k < 32; k++)
        {

            N[32*j+k] = buf[35-j][k];
        }
    }
}

void S_box(int round,DATATYPE buf[36][32])
{
    bits sm4;

    for(int i = 0; i<4; i++)
    {
        sm4.b7 = buf[round+4][i*8];
        sm4.b6 = buf[round+4][i*8+1];
        sm4.b5 = buf[round+4][i*8+2];
        sm4.b4 = buf[round+4][i*8+3];
        sm4.b3 = buf[round+4][i*8+4];
        sm4.b2 = buf[round+4][i*8+5];
        sm4.b1 = buf[round+4][i*8+6];
        sm4.b0 = buf[round+4][i*8+7];

        Sm4_BoolFun(sm4,&buf[round+4][i*8+7],&buf[round+4][i*8+6],&buf[round+4][i*8+5],&buf[round+4][i*8+4],
            &buf[round+4][i*8+3],&buf[round+4][i*8+2],&buf[round+4][i*8+1],&buf[round+4][i*8]);

    }
}

void Sm4_BoolFun(bits in, bit_t *out0, bit_t *out1, bit_t *out2, bit_t *out3, bit_t *out4, bit_t *out5, bit_t *out6, bit_t *out7){
        bit_t y_t[21], t_t[8], t_m[46], y_m[18], t_b[30];
  	    y_t[18] = in.b2 ^in.b6;
		t_t[ 0] = in.b3 ^in.b4;
		t_t[ 1] = in.b2 ^in.b7;
		t_t[ 2] = in.b7 ^y_t[18];
		t_t[ 3] = in.b1 ^t_t[ 1];
		t_t[ 4] = in.b6 ^in.b7;
		t_t[ 5] = in.b0 ^y_t[18];
		t_t[ 6] = in.b3 ^in.b6;
		y_t[10] = in.b1 ^y_t[18];
		y_t[ 0] = in.b5 ^~ y_t[10];
		y_t[ 1] = t_t[ 0] ^t_t[ 3];
		y_t[ 2] = in.b0 ^t_t[ 0];
		y_t[ 4] = in.b0 ^t_t[ 3];
		y_t[ 3] = in.b3 ^y_t[ 4];
		y_t[ 5] = in.b5 ^t_t[ 5];
		y_t[ 6] = in.b0 ^~ in.b1;
		y_t[ 7] = t_t[ 0] ^~ y_t[10];
		y_t[ 8] = t_t[ 0] ^t_t[ 5];
		y_t[ 9] = in.b3;
		y_t[11] = t_t[ 0] ^t_t[ 4];
		y_t[12] = in.b5 ^t_t[ 4];
		y_t[13] = in.b5 ^~ y_t[ 1];
		y_t[14] = in.b4 ^~ t_t[ 2];
		y_t[15] = in.b1 ^~ t_t[ 6];
		y_t[16] = in.b0 ^~ t_t[ 2];
		y_t[17] = t_t[ 0] ^~ t_t[ 2];
		y_t[19] = in.b5 ^~ y_t[14];
		y_t[20] = in.b0 ^t_t[ 1];

    //The shared non-linear middle part for AES, AES^-1, and SM4
  	t_m[ 0] = y_t[ 3] ^	 y_t[12];
		t_m[ 1] = y_t[ 9] &	 y_t[ 5];
		t_m[ 2] = y_t[17] &	 y_t[ 6];
		t_m[ 3] = y_t[10] ^	 t_m[ 1];
		t_m[ 4] = y_t[14] &	 y_t[ 0];
		t_m[ 5] = t_m[ 4] ^	 t_m[ 1];
		t_m[ 6] = y_t[ 3] &	 y_t[12];
		t_m[ 7] = y_t[16] &	 y_t[ 7];
		t_m[ 8] = t_m[ 0] ^	 t_m[ 6];
		t_m[ 9] = y_t[15] &	 y_t[13];
		t_m[10] = t_m[ 9] ^	 t_m[ 6];
		t_m[11] = y_t[ 1] &	 y_t[11];
		t_m[12] = y_t[ 4] &	 y_t[20];
		t_m[13] = t_m[12] ^	 t_m[11];
		t_m[14] = y_t[ 2] &	 y_t[ 8];
		t_m[15] = t_m[14] ^	 t_m[11];
		t_m[16] = t_m[ 3] ^	 t_m[ 2];
		t_m[17] = t_m[ 5] ^	 y_t[18];
		t_m[18] = t_m[ 8] ^	 t_m[ 7];
		t_m[19] = t_m[10] ^	 t_m[15];
		t_m[20] = t_m[16] ^	 t_m[13];
		t_m[21] = t_m[17] ^	 t_m[15];
		t_m[22] = t_m[18] ^	 t_m[13];
		t_m[23] = t_m[19] ^	 y_t[19];
		t_m[24] = t_m[22] ^	 t_m[23];
		t_m[25] = t_m[22] &	 t_m[20];
		t_m[26] = t_m[21] ^	 t_m[25];
		t_m[27] = t_m[20] ^	 t_m[21];
		t_m[28] = t_m[23] ^	 t_m[25];
		t_m[29] = t_m[28] &	 t_m[27];
		t_m[30] = t_m[26] &	 t_m[24];
		t_m[31] = t_m[20] &	 t_m[23];
		t_m[32] = t_m[27] &	 t_m[31];
		t_m[33] = t_m[27] ^	 t_m[25];
		t_m[34] = t_m[21] &	 t_m[22];
		t_m[35] = t_m[24] &	 t_m[34];
		t_m[36] = t_m[24] ^	 t_m[25];
		t_m[37] = t_m[21] ^	 t_m[29];
		t_m[38] = t_m[32] ^	 t_m[33];
		t_m[39] = t_m[23] ^	 t_m[30];
		t_m[40] = t_m[35] ^	 t_m[36];
		t_m[41] = t_m[38] ^	 t_m[40];
		t_m[42] = t_m[37] ^	 t_m[39];
		t_m[43] = t_m[37] ^	 t_m[38];
		t_m[44] = t_m[39] ^	 t_m[40];
		t_m[45] = t_m[42] ^	 t_m[41];
		y_m[ 0] = t_m[38] &	 y_t[ 7];
		y_m[ 1] = t_m[37] &	 y_t[13];
		y_m[ 2] = t_m[42] &	 y_t[11];
		y_m[ 3] = t_m[45] &	 y_t[20];
		y_m[ 4] = t_m[41] &	 y_t[ 8];
		y_m[ 5] = t_m[44] &	 y_t[ 9];
		y_m[ 6] = t_m[40] &	 y_t[17];
		y_m[ 7] = t_m[39] &	 y_t[14];
		y_m[ 8] = t_m[43] &	 y_t[ 3];
		y_m[ 9] = t_m[38] &	 y_t[16];
		y_m[10] = t_m[37] &	 y_t[15];
		y_m[11] = t_m[42] &	 y_t[ 1];
		y_m[12] = t_m[45] &	 y_t[ 4];
		y_m[13] = t_m[41] &	 y_t[ 2];
		y_m[14] = t_m[44] &	 y_t[ 5];
		y_m[15] = t_m[40] &	 y_t[ 6];
		y_m[16] = t_m[39] &	 y_t[ 0];
		y_m[17] = t_m[43] &	 y_t[12];

  //bottom(outer) linear layer for sm4
  	t_b[ 0] = y_m[ 4] ^	 y_m[ 7];
		t_b[ 1] = y_m[13] ^	 y_m[15];
		t_b[ 2] = y_m[ 2] ^	 y_m[16];
		t_b[ 3] = y_m[ 6] ^	 t_b[ 0];
		t_b[ 4] = y_m[12] ^	 t_b[ 1];
		t_b[ 5] = y_m[ 9] ^	 y_m[10];
		t_b[ 6] = y_m[11] ^	 t_b[ 2];
		t_b[ 7] = y_m[ 1] ^	 t_b[ 4];
		t_b[ 8] = y_m[ 0] ^	 y_m[17];
		t_b[ 9] = y_m[ 3] ^	 y_m[17];
		t_b[10] = y_m[ 8] ^	 t_b[ 3];
		t_b[11] = t_b[ 2] ^	 t_b[ 5];
		t_b[12] = y_m[14] ^	 t_b[ 6];
		t_b[13] = t_b[ 7] ^	 t_b[ 9];
		t_b[14] = y_m[ 0] ^	 y_m[ 6];
		t_b[15] = y_m[ 7] ^	 y_m[16];
		t_b[16] = y_m[ 5] ^	 y_m[13];
		t_b[17] = y_m[ 3] ^	 y_m[15];
		t_b[18] = y_m[10] ^	 y_m[12];
		t_b[19] = y_m[ 9] ^	 t_b[ 1];
		t_b[20] = y_m[ 4] ^	 t_b[ 4];
		t_b[21] = y_m[14] ^	 t_b[ 3];
		t_b[22] = y_m[16] ^	 t_b[ 5];
		t_b[23] = t_b[ 7] ^	 t_b[14];
		t_b[24] = t_b[ 8] ^	 t_b[11];
		t_b[25] = t_b[ 0] ^	 t_b[12];
		t_b[26] = t_b[17] ^	 t_b[ 3];
		t_b[27] = t_b[18] ^	 t_b[10];
		t_b[28] = t_b[19] ^	 t_b[ 6];
		t_b[29] = t_b[ 8] ^	 t_b[10];
		*out0 = t_b[11] ^~ t_b[13];
		*out1 = t_b[15] ^~ t_b[23];
		*out2 = t_b[20] ^	 t_b[24];
		*out3 = t_b[16] ^	 t_b[25];
		*out4 = t_b[26] ^~ t_b[22];
		*out5 = t_b[21] ^	 t_b[13];
		*out6 = t_b[27] ^~ t_b[12];
		*out7 = t_b[28] ^~ t_b[29];
}
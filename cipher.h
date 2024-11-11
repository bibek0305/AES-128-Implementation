#include<stdio.h>
#include<stdint.h>
#include"keyexpand.h"

//substitute each byte using s box.
void subbytes(uint8_t S[4][4])
{
    for(int i=0;i<4;i++)
    {
        for(int j=0;j<4;j++)
        {
            uint8_t x=S[i][j]&0x0f;
            uint8_t y=S[i][j]>>4;
            S[i][j]=s_box[y][x];
        }
    }
}

// function for rotation of single byte.
void single_byte_rot(uint8_t x[])
{
    uint8_t a=x[0];
    for(int i=0;i<3;i++)
    {
        x[i]=x[i+1];
    }
    x[3]=a;
}

// cyclically shifts the last three rows in the state.
void shiftrows(uint8_t S[4][4])
{
    for(int i=1;i<4;i++)
    {
        for(int k=0;k<i;k++) // k for how many times single_byte_rot for the rows.
        {
            single_byte_rot(S[i]);
        }
    }
}

//multiplication by 2 and 3 in GF(2^8) using the irreducible polynomial x^8+x^4+x^3+x+1.
uint8_t mult_by_2(uint8_t a)
{
    uint16_t x;
    x=a<<1;
    uint8_t y= x & 0xff;
    uint8_t z=x>>8;
    return (z*0x1b)^y;
}
uint8_t mult_by_3(uint8_t a)
{
    uint16_t x;
    x=a<<1;
    uint8_t y= (x^((uint16_t)a)) & 0xff;
    uint8_t z=x>>8;
    return (z*0x1b)^y;
}

// column by column multiplication of the state matrix with a fixed matrix.
void mixcolumns(uint8_t S[][4])
{
    uint8_t temp[4][4];
    for(int k=0;k<4;k++)
    {
        temp[0][k]= mult_by_2(S[0][k]) ^ mult_by_3(S[1][k]) ^ S[2][k] ^ S[3][k];
        temp[1][k]= S[0][k] ^ mult_by_2(S[1][k]) ^ mult_by_3(S[2][k]) ^ S[3][k];
        temp[2][k]= S[0][k] ^ S[1][k] ^ mult_by_2(S[2][k]) ^ mult_by_3(S[3][k]);
        temp[3][k]= mult_by_3(S[0][k]) ^ S[1][k] ^ S[2][k] ^ mult_by_2(S[3][k]);

        for(int i=0;i<4;i++)
        {
            S[i][k] = temp [i][k];
        }
        
    }
}

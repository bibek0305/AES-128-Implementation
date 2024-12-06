#include<stdio.h>
#include<stdint.h>
#include"cipher.h"
#include"decipher.h"

void  AES_ENC(uint8_t m[])
{
    uint8_t k[16]= {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};//128_bit key
    uint8_t state[4][4];
    word w[4][11];// To store 11 round keys.


    key_expand(k,w);

    //convert the input into a state matrix
    for(int i=0;i<4;i++)
    {
        for(int j=0;j<4;j++)
        {
            state[i][j]=m[i+4*j];
        }
    }

    for (int r=0;r<9;r++)//r:round no.
    {
        for(int i=0;i<4;i++)//i-th keyword of r-th round also i-th column of the state matrix
        {
            for(int k=0;k<4;k++)//k:row index for an element in state matrix also the k-th byte in i-th keyword
            {
                state[k][i] ^= w[i][r].s[k];
            } 
            
        }
       
        subbytes(state);
        shiftrows(state);
        mixcolumns(state);  
    }

    for(int i=0;i<4;i++)
    {
        for(int k=0;k<4;k++)
        {
            state[k][i] ^= w[i][9].s[k];
        }       
        
    }
    subbytes(state);
    shiftrows(state);

    for(int i=0;i<4;i++)
    {
        for(int k=0;k<4;k++)
        {
            state[k][i] ^= w[i][10].s[k];
        } 
    }
   
    for(int i=0;i<4;i++)
    {
        for(int j=0;j<4;j++)
        {
            m[i+4*j] = state[i][j];
        }
    }
}




void AES_DEC(uint8_t c[])
{
    uint8_t k[16]= {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};//128_bit key
    uint8_t state[4][4];
    word w[4][11];

    key_expand(k,w);

    for(int i=0;i<4;i++)
    {
        for(int j=0;j<4;j++)
        {
            state[i][j]=c[i+4*j];
        }
    }

    //inverse of the add round key for round 10...
    for (int j=0;j<4;j++)
    {
        for(int k=0;k<4;k++)
        {
            state[k][j] ^= w[j][10].s[k]; 
        }
    }

    //shift row, subbyte, add roundkey, mixcolumn operations upto round 1...
    for(int r = 9;r>=1;r--)
    {
        inv_subbytes(state);
        inv_shiftrow(state);
        
        for (int j=0;j<4;j++)
        {
            for(int k=0;k<4;k++)
            {
                state[k][j] ^= w[j][r].s[k]; 
            }
        }
        inv_mixcolumns(state);       
    }
    // shiftrow, subbyte, addroundkey operations at the last round...
        inv_shiftrow(state);
        inv_subbytes(state);
        for (int j=0;j<4;j++)
        {
            for(int k=0;k<4;k++)
            {
                state[k][j] ^= w[j][0].s[k]; 
            }
        }

    for(int i=0;i<4;i++)
    {
        for(int j=0;j<4;j++)
        {
            c[i+4*j] = state[i][j];
        }
    }
}
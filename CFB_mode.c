#include<stdio.h>
#include<stdint.h>
#include"AES.h"

void padding(uint8_t buffer[16], int m)
{
    uint8_t a=16-m;
    for(int i=m;i<16;i++)
    {
        buffer[i] = a;
    }
}

int main()
{
    uint8_t buffer[16],msg[16],temp[16],
    IV[16] = {0x1A, 0x2B, 0x3C, 0x4D, 0x5E, 0x6F, 0x7A, 0x8B,0x9C, 0xAD, 0xBE, 0xCF, 0xD1, 0xE2, 0xF3, 0x04};
    FILE *fptr, *fptr1, *fptr2;

    fptr = fopen("newfile3.pdf","rb");
    fptr1 = fopen("enc_cfb","wb");

    if(fptr == NULL)
    {
        printf("Error in opening file.");
        return 1;
    }
    
    if(fptr1 == NULL)
    {
        printf("Error in opening file.");
        return 1;
    }
    fwrite(IV,sizeof(uint8_t),16,fptr1);
    for(int i=0;i<16;i++)
    {
        temp[i] = IV[i];
    }

    while(1)
    {
        int n = fread(buffer,sizeof(uint8_t),16,fptr);
        if(n<16)
        {
            padding(buffer,n);
            AES_ENC(temp);
            for(int i =0;i<16;i++)
            {
                temp[i] = buffer[i] ^ temp[i];
            }       
            fwrite(temp,sizeof(uint8_t),16,fptr1);
            break;
        }
        AES_ENC(temp);
        for(int i =0;i<16;i++)
        {
            temp[i] = buffer[i] ^ temp[i];
        }
        fwrite(temp,sizeof(uint8_t),16,fptr1);
    }
    fclose(fptr);
    fclose(fptr1);


    //decryption......
    fptr1=fopen("enc_cfb","rb");
    fptr2=fopen("newfile4_cfb.pdf","wb");

    if(fptr1 == NULL)
    {
        printf("Error in opening file.");
        return 1;
    }
    if(fptr2 == NULL)
    {
        printf("Error in opening file.");
        return 1;
    }
    fread(buffer,sizeof(uint8_t),16,fptr1);
    for(int i =0;i<16;i++)
    {
        temp[i]= buffer[i];
    }
    int flag =0;
    while(1)
    {
        int k=fread(buffer,1,16,fptr1);
        if(flag!=0)
        {
            if(k==0)
            {
                int p = (int)msg[15];
                int a = 16-p;
                fwrite(msg,1,a,fptr2);
                break;
            }
            else
            {
                fwrite(msg, sizeof(uint8_t), 16,fptr2);
            }
        }
        AES_ENC(temp);
        for(int i=0;i<16;i++)
        {
            msg[i]=temp[i] ^ buffer[i];
        }
        for(int i =0;i<16;i++)
        {
            temp[i]= buffer[i];
        }
        flag =1;
    }
    fclose(fptr1);
    fclose(fptr2);
    return 0;
}
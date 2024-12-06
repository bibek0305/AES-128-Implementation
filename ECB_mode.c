#include<stdio.h>
#include<stdint.h>
#include"AES.h"

// Function to add padding to the buffer
void padding(uint8_t buffer[], int m)
{
    uint8_t a=16-m;  // Calculate the padding value based on the number of missing bytes
    for (int i=m;i<16;i++) 
    {
        buffer[i]= a;  // Fill the rest of the buffer with the padding value
    }
}

int main()
{
    uint8_t buffer[16],temp[16];  
    FILE *fptr, *fptr1, *fptr2;  


    fptr = fopen("newfile3.pdf","rb");
    fptr1 = fopen("newfile2","wb");

    // Check if the input file failed to open
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


    while(1)
    {
        // Read 16 bytes from the input file
        int n = fread(buffer, sizeof(uint8_t), 16, fptr);
        
        if(n < 16)
        {
            padding(buffer, n);  // Apply padding to the buffer
            AES_ENC(buffer);  // Encrypt the padded buffer
            fwrite(buffer, sizeof(uint8_t), 16, fptr1);  // Write the encrypted data to the output file
            break;  // Exit the loop after the final block
        }

        AES_ENC(buffer);
        fwrite(buffer, sizeof(uint8_t), 16, fptr1);
    }


    fclose(fptr);
    fclose(fptr1);

    // Decryption process starts here...

    // Open the encrypted file for reading
    fptr1 = fopen("newfile2", "rb");
    if(fptr1 == NULL)
    {
        printf("Error in opening file.");
        return 1;
    }

    // Open the output file to write the decrypted content
    fptr2 = fopen("newfile3_ECB.pdf", "wb");
    if(fptr2 == NULL)
    {
        printf("Error in opening file.");
        return 1;
    }

    int flag = 0;  
    while(1)
    {
        int k = fread(buffer, sizeof(uint8_t), 16, fptr1);

        if(flag != 0)
        {
            // If no more data, handle padding removal
            if(k == 0)
            {
                int p = (int)temp[15];  // Get the padding value
                int a = 16 - p;  // Calculate the number of valid bytes
                fwrite(temp, 1, a, fptr2);  // Write the valid decrypted bytes
                break;  // Exit the loop after the final block
            }
            else
            {
                fwrite(temp, sizeof(uint8_t), 16, fptr2);  // Write the previously decrypted block
            }
        }

        // Decrypt the 16-byte block
        AES_DEC(buffer);
        // Store the decrypted data into temp
        for(int i = 0; i < 16; i++)
        {
            temp[i] = buffer[i];
        }

        flag = 1;  // Set the flag to indicate the first block is processed
    }

    
    fclose(fptr1);
    fclose(fptr2);

    return 0;  
}

/*
Command : gcc <file.c> -lcrypto
The libcrypto shared library should be present in the current working directory
openssl library should be installed
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <openssl/aes.h>

#define BLOCK_SIZE 16

/*Global variables*/

unsigned char GeneratedKey[16];	  /*Array to store key*/
unsigned char Message[128] = {0}; /*Array to store message*/

/*Function declarations*/

void GenerateKey(void);
unsigned char *ReadKey(void);
int Get_Number_Of_Blocks(unsigned char *Message);
void XOR_With_Key(unsigned char *Message, unsigned char *Key, int NumberOfBlocks);
void Encrypt_As_Blocks(unsigned char *Message, int NumberOfBlocks, unsigned char *Key);
void Decrypt_As_Blocks(unsigned char *Message, int NumberOfBlocks, unsigned char *Key);

int main()
{
	/*Variable to store the number of 16 byte blocks of the message*/
	int NumberOfBlocks = 0;

	/*Get message from CLI*/
	printf("Enter the message:\n");
	fgets(Message, 128, stdin); /* get message as keyboard input*/
	printf("\n");

	NumberOfBlocks = Get_Number_Of_Blocks(Message); /*Stores number of 16 byte blocks present in message.Last block might contain padding*/

	printf("\nOriginal message in Hex:\n");
	for (int i = 0; i < strlen((const char *)Message); i++)
	{
		printf("%02x ", Message[i]); /*Print atleast 2 digits of the hex value*/
	}
	printf("\n");

	unsigned char *KeyPtr; /*Pointer to array containing  random generated key*/
	GenerateKey();	       /*Calling function that calls python script to generate key*/
	KeyPtr = ReadKey();    /*Reads key from file*/

	/*XOR with key*/
	XOR_With_Key(Message, KeyPtr, NumberOfBlocks);
	printf("\nAfter XOR:\n");
	for (int i = 0; i < NumberOfBlocks * BLOCK_SIZE; i++)
	{
		printf("%02x ", Message[i]);
	}
	printf("\n");

	/*Encrypt message using AES*/
	Encrypt_As_Blocks(Message, NumberOfBlocks, KeyPtr);
	printf("\nAfter Encryption->HEX:\n");
	for (int i = 0; i < NumberOfBlocks * BLOCK_SIZE; i++)
	{
		printf("%02x ", Message[i]);
	}
	printf("\n");

	printf("\nAfter Encryption->ASCII:\n");
	for (int i = 0; i < NumberOfBlocks * BLOCK_SIZE; i++)
	{
		printf("%c", Message[i]);
	}
	printf("\n");

	/*Decrypt message using AES*/
	Decrypt_As_Blocks(Message, NumberOfBlocks, KeyPtr);

	/*Display the message after decryption as hex values*/
	printf("\nAfter Decryption->HEX:\n");
	for (int i = 0; i < strlen((const char *)Message); i++)
	{
		printf("%02x ", Message[i]);
	}
	printf("\n");

	/*Display the message after decryption in ASCII values*/
	printf("\nAfter Decryption->ASCII:\n");
	for (int i = 0; i < strlen((const char *)Message); i++)
	{
		printf("%c", Message[i]);
	}
	printf("\n");

	/*XOR each 16 byte block with the key*/
	XOR_With_Key(Message, KeyPtr, NumberOfBlocks);

	/*Display the message after XOR operation as hex values*/
	printf("\nAfter XOR operation->HEX:\n");
	for (int i = 0; i < strlen((const char *)Message); i++)
	{
		printf("%02x ", Message[i]);
	}
	printf("\n");

	/*Display the message after XOR operation as ASCII values*/
	printf("\nFinal decrypted Message:\n");
	for (int i = 0; i < strlen((const char *)Message); i++)
	{
		printf("%c", Message[i]);
	}
	printf("\n");
	return 0;
}

/*Function to cal python script that generates random key */
void GenerateKey()
{
	printf("\nGenerating Key: \n");
	system("python key_generator.py"); // Displays key in terminal,writes key to a file
}

/*Function to read key from file*/
unsigned char *ReadKey(void)
{
	FILE *file;
	file = fopen("/home/amalr/Desktop/PythonGit/pythonassignments/Q6/key.txt", "r");
	fgets(GeneratedKey, 128, file);
	printf("GeneratedKey :\n");
	for (int i = 0; i < 16; i++)
	{
		printf("%02x ", GeneratedKey[i]);
	}
	printf("\n");
	printf("size of key = %ld \n", sizeof(GeneratedKey));
	return GeneratedKey;
}

/*Function to calculate the number of 128 byte blocks in the message*/
int Get_Number_Of_Blocks(unsigned char *Message)
{
	/*Number of characters in message*/
	int NumberOfCharacters = strlen((const char *)Message);

	/*Number of 16byte blocks*/
	int NumberOfBlocks = NumberOfCharacters / BLOCK_SIZE;

	if ((NumberOfCharacters % 16) != 0)
		NumberOfBlocks = ((NumberOfCharacters / BLOCK_SIZE) + 1);

	/*Display number of characters and 128 bit blocks*/
	printf("NumberOfCharacters = %d\n", NumberOfCharacters);
	printf("NumberOfBlocks = %d\n", NumberOfBlocks);
	return NumberOfBlocks;
}

/*Function to XOR each 16 byte block in the array passed as argument with the 16 byte key*/
void XOR_With_Key(unsigned char *Message, unsigned char *Key, int NumberOfBlocks)
{
	int i, j, k = 0;
	unsigned char BLOCK[16] = {0};
	// unsigned char XOR_BLOCK[16] = {0};
	printf("\nXORing with Key:\n");
	for (i = 0; i < NumberOfBlocks; i++)
	{
		for (j = 0; j < BLOCK_SIZE; j++)
		{
			BLOCK[j] = *(Message + (i * BLOCK_SIZE) + j);
		}
		/*XOR with key*/
		for (k = 0; k < BLOCK_SIZE; k++)
		{
			// XOR_BLOCK[k] = BLOCK[k] ^ *(Key + k);
			BLOCK[k] ^= *(Key + k);
			*(Message + (i * BLOCK_SIZE) + k) = BLOCK[k];
		}
	}
}

/*Function to encrypt each 16byte block of the array passed as argument using AES algorithm*/
void Encrypt_As_Blocks(unsigned char *Message, int NumberOfBlocks, unsigned char *Key)
{

	unsigned char UNENCRYPTED_BLOCK[16] = {0}; /*16byte long block to store elements of unencypted message*/
	unsigned char ENCRYPTED_BLOCK[16] = {0};   /*16byte long block to store elements of unencypted message*/
	AES_KEY aesKey;				   /*Set key*/
	unsigned char iv[] = "InitializationVe";   /* 128-bit Initialisation Variable*/
	/*Set encryption key*/
	if (AES_set_encrypt_key(GeneratedKey, 128, &aesKey) < 0)
	{
		fprintf(stderr, "Could not set encryption key.\n");
		exit(1);
	}
	/*Considering each block,encrypt it,copy to final array*/
	for (int i = 0; i < NumberOfBlocks; i++)
	{
		for (int j = 0; j < BLOCK_SIZE; j++)
		{
			UNENCRYPTED_BLOCK[j] = *(Message + (i * BLOCK_SIZE) + j); /*Copying elements of message to the 16 byteblock*/
		}

		AES_encrypt(UNENCRYPTED_BLOCK, ENCRYPTED_BLOCK, &aesKey); /*Encrypting the message using openssl implementation of AES*/

		/*Copy encrypted block to final message*/
		for (int k = 0; k < BLOCK_SIZE; k++)
		{
			*(Message + (i * BLOCK_SIZE) + k) = ENCRYPTED_BLOCK[k];
		}
	}
}

/*Function to decrypt each 16byte block of the array passed as argument using AES algorithm*/
void Decrypt_As_Blocks(unsigned char *Message, int NumberOfBlocks, unsigned char *Key)
{

	unsigned char ENCRYPTED_BLOCK[16] = {0}; /*Array to store a 128 bit block of original message*/
	unsigned char DECRYPTED_BLOCK[16] = {0}; /*Array to store the output of decryption function*/
	AES_KEY aesKey;				 /*Set key*/
	unsigned char iv[] = "InitializationVe"; /* 128-bit Initialisation Variable*/
						 /*Set decryption key*/
	if (AES_set_decrypt_key(GeneratedKey, 128, &aesKey) < -1)
	{
		fprintf(stderr, "Could not set decryption key.\n");
		exit(1);
	}
	/*Considering each block,decrypt it,copy to final array*/
	for (int i = 0; i < NumberOfBlocks; i++)
	{
		for (int j = 0; j < BLOCK_SIZE; j++)
		{
			ENCRYPTED_BLOCK[j] = *(Message + (i * BLOCK_SIZE) + j);
		}

		AES_decrypt(ENCRYPTED_BLOCK, DECRYPTED_BLOCK, &aesKey); /*Decrypt message using openssl implementation of AES*/

		/*Copy decrypted block to final message*/
		for (int k = 0; k < BLOCK_SIZE; k++)
		{
			*(Message + (i * BLOCK_SIZE) + k) = DECRYPTED_BLOCK[k];
		}
	}
}

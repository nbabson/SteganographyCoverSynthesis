// Neil Babson
// Multimedia Security
// April 21, 2016
//

// Decryption
#include <stdio.h>
#include <string.h>
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>


int main(int argc, char* argv[])
{
   if (argc != 7)
   {
      printf("Usage: dec -i <ciphertext filename> -o <plaintext filename> -p <password>\n\n");
      return 1;
   }

   // Read ciphertext message from file
   unsigned char * cipher = NULL;
   long cipher_len;
   FILE * f_in = fopen(argv[2], "rb");

   if (f_in)
   {
      fseek(f_in, 0, SEEK_END);
      cipher_len = ftell(f_in);
      fseek(f_in, 0, SEEK_SET);
      cipher = malloc(cipher_len);
      if (cipher)
	 fread(cipher, 1, cipher_len, f_in);
      fclose(f_in);
   }

   // Display ciphertext message
   //printf("\n%s\n", cipher);

   int success;
   int passlen = strlen(argv[6]);
   char* pass = malloc(sizeof(char) * (passlen + 1));
   unsigned char* salt = NULL;
   int saltlen = 0;
   int iter = 10000;
   int keylen = 100;
   unsigned char key[keylen + 1];

   unsigned char* message = malloc(sizeof(char) * (cipher_len));
   int message_len = cipher_len;

   strcpy(pass, argv[6]);
   // Derive encryption key from password
   success = PKCS5_PBKDF2_HMAC_SHA1(pass, passlen, salt, saltlen, iter, keylen, key);

   if (!success)
   {
      printf("Unable to generate encryption key.\n\n");
      return 1;
   }

   // Display key
   //int i;
   //printf("Key: ");
   //for (i = 0; i < keylen; ++i)
   //   printf("%x", key[i]);
   //printf("\n");

   // Decrypt
   EVP_CIPHER_CTX *ctx;
   static const unsigned char iv[] = {
      //0x99,0xaa,0x3e,0x68,0xed,0x81,0x73,0xa0,0xee,0xd0,0x66,0x84};
      0x99,0xaa,0x3e,0x68,0xed,0x81,0x73,0xa0,0xee,0xd0,0x66,0x84};

   // Create and initialize context
   ctx = EVP_CIPHER_CTX_new();
   // Set decryption program with type and mode
   EVP_DecryptInit_ex(ctx, EVP_aes_128_gcm(), NULL, NULL, NULL);
   // Initialize key and IV
   EVP_DecryptInit_ex(ctx, NULL, NULL, key, iv);
   // Decrypt plaintext
   EVP_DecryptUpdate(ctx, message, &message_len, cipher, cipher_len); 

   // Set expected tag value.
   //EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_TAG, 16, tag);

   // Finalize teh decryption
   success = EVP_DecryptFinal_ex(ctx, message, &message_len); 

   /* Clean up */

   EVP_CIPHER_CTX_free(ctx);
   /* Remove all digests and ciphers */
   EVP_cleanup();
   /* if next step omitted, a small leak may be left when making use of the BIO (low level API) for e.g. base64 transformations */
   CRYPTO_cleanup_all_ex_data();
   /* Remove error strings */
   ERR_free_strings();

   // Write ciphertext to file
   FILE * f_out = fopen(argv[4], "wb");

   // Print plaintext message
   //printf("\n%s\n", message);

   if (f_out)
   {
      if (message)
         fputs((char*) message, f_out);
      fclose(f_out);
   }

   // Deallocate
   free(message);
   free(pass);
   free(cipher);

   return 0;
}

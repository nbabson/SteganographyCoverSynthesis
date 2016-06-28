// Neil Babson
// Multimedia Security
// April 21, 2016

// Encryption
#include <stdio.h>
#include <string.h>
#include <openssl/ssl.h>
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/err.h>


int main(int argc, char* argv[])
{
   if (argc != 7 || strcmp(argv[1], "-i") || strcmp(argv[3], "-o") || strcmp(argv[5], "-p"))
   {
      printf("Usage: enc -i <plaintext filename> -o <ciphertext filename> -p <password>\n\n");
      return 1;
   }


   // Read plaintext message from file
   unsigned char * message = NULL;
   long length;
   FILE * f_in = fopen(argv[2], "rb");

   if (f_in)
   {
      fseek(f_in, 0, SEEK_END);
      length = ftell(f_in);
      fseek(f_in, 0, SEEK_SET);
      message = malloc(length + 1);
      if (message)
	 fread(message, 1, length, f_in);
      fclose(f_in);
   }

   // Display plaintext message
   //printf("\n%s\n", message);
 
   int success;
   int passlen = strlen(argv[6]);
   char* pass = malloc(sizeof(char) * (passlen + 1));
   strcpy(pass, argv[6]);
   unsigned char* salt = NULL;
   int saltlen = 0;
   int iter = 10000;
   int keylen = 100;
   unsigned char key[keylen + 1];
   unsigned char* cipher = malloc(length);
   int cipher_len = length;
   unsigned char* tag = NULL;

   // Display password
   //printf("Password: %s\n", pass);

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


   // Encrypt
   EVP_CIPHER_CTX *ctx;
   static const unsigned char iv[] = {
      0x99,0xaa,0x3e,0x68,0xed,0x81,0x73,0xa0,0xee,0xd0,0x66,0x84};

   // Create and initialize context
   ctx = EVP_CIPHER_CTX_new();
   // Set encryption program with type and mode
   EVP_EncryptInit_ex(ctx, EVP_aes_128_gcm(), NULL, NULL, NULL);
   // Initialize key and IV
   EVP_EncryptInit_ex(ctx, NULL, NULL, key, iv);
   // Encrypt plaintext
   EVP_EncryptUpdate(ctx, cipher, &cipher_len, message, length); 
   // Finalize teh encryption
   EVP_EncryptFinal_ex(ctx, cipher, &cipher_len);


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

   // Print ciphertext
   //printf("\n%s\n", cipher);

   //printf("Length: %ld\n", length);
   if (f_out)
   {
      if (cipher)
         fwrite(cipher, length, 1, f_out); 
      fclose(f_out);
   }

   // Deallocate
   free(message);
   free(pass);
   free(cipher);
   free(tag);

   return 0;
}

// Neil Babson
// March 23, 2016
// Cellular Automota message embedder
// MultiMedia Security
// Final Project

// Embed encrypted message in CA rule
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <openssl/rand.h>

int verbose = 0;

int main(int argc, char* argv[])
{
   if (argc < 5)
   {
      printf("Usage: embed <message> <cover rule file> <states> <neighborhood> [<CA padding file>]\n");
      return 1;
   }
   FILE* f_in;
   unsigned i;
   long message_len;
   int states = atoi(argv[3]);
   int neighborhood = atoi(argv[4]);

   if (states != 4) {
      printf("Only four state version has been implemented\n");
      return 1;
   }

   if (verbose)
   {
      printf("States: %d\n", states);
      printf("Neighborhood: %d\n", neighborhood);
   }

   int rule_len = pow(states, neighborhood);
   unsigned char* message = NULL; 
   unsigned padding[rule_len];
   int rule[rule_len];

   if (argc == 6)
   {
      f_in = fopen(argv[5], "r");
      if (f_in) {
         for (i = 0; i < rule_len; ++i)
	    padding[i] = getc(f_in) - 48;
         fclose(f_in);
      }
      else {
	 printf("Couldn't open CA padding file\n");
         return 1;
      }
   }
   if ((f_in = fopen(argv[1], "rb"))){
      fseek(f_in, 0, SEEK_END);
      message_len = ftell(f_in);
      fseek(f_in, 0, SEEK_SET);
      message = malloc(message_len);
      if (message)
	 fread(message, i, message_len, f_in);
      fclose(f_in);
   }
   else {
      printf("Couldn't open message file\n");
      return 1;
   }

   if (verbose)
   {
      printf("\n%s\n", message);
      printf("Message length: %ld\n", message_len);
      printf("Rule length: %0x\n", rule_len);
   }
   // If states = 4, add 8 for 2 bytes of message length
   if (message_len * 4 + 8 > rule_len) {
      printf("Message is too long to embed in rule\n");
      return 1;
   }


   // Encode message length
   rule[0] = (message_len & 1) + (message_len & 2) ;
   rule[1] = ((message_len & 0x4) >> 2) + ((message_len & 0x8) >> 2);
   rule[2] = ((message_len & 0x10) >> 4) + ((message_len & 0x20) >> 4);
   rule[3] = ((message_len & 0x40) >> 6) + ((message_len & 0x80) >> 6);
   rule[4] = ((message_len & 0x100) >> 8) + ((message_len & 0x200) >> 8) ;
   rule[5] = ((message_len & 0x400) >> 10) + ((message_len & 0x800) >> 10);
   rule[6] = ((message_len & 0x1000) >> 12) + ((message_len & 0x2000) >> 12);
   rule[7] = ((message_len & 0x4000) >> 14) + ((message_len & 0x8000) >> 14);

   int rule_index = 8;
   if (states == 4) {
      for (i = 0; i < message_len; ++i) {
	 if (verbose)
	    printf("Message: %c\n", message[i]);
	 rule[rule_index] = ((message[i] & 0x40) >> 6) + ((message[i] & 0x80) >> 6);
	 rule[rule_index+1] = ((message[i] & 0x10) >> 4) + ((message[i] & 0x20) >> 4);
         rule[rule_index+2] = ((message[i] & 0x4) >> 2)  + ((message[i] & 0x8) >> 2);
         rule[rule_index+3] = (message[i] & 1) + (message[i] & 2);
	 rule_index += 4;
      }
   }

   // If message fits embed twice
   if (rule_len - rule_index > message_len) {
      for (i = 0; i < message_len; ++i) {
	 if (verbose)
	    printf("Message: %c\n", message[i]);
	 rule[rule_index] = ((message[i] & 0x40) >> 6) + ((message[i] & 0x80) >> 6);
	 rule[rule_index+1] = ((message[i] & 0x10) >> 4) + ((message[i] & 0x20) >> 4);
	 rule[rule_index+2] = ((message[i] & 0x4) >> 2)  + ((message[i] & 0x8) >> 2);
	 rule[rule_index+3] = (message[i] & 1) + (message[i] & 2);
	 rule_index += 4;
      }
   }

   unsigned char buf; // = malloc(20*sizeof(unsigned char));
   // Pad rule
   if (argc == 6) {
      for (;rule_index < rule_len; ++rule_index)
	 rule[rule_index] = padding[rule_index];
   }
   // Pad random
   else {
      while (rule_index < rule_len) {
	 RAND_bytes(&buf, 1);
	 rule[rule_index++] = ((buf & 0x40) >> 6) + ((buf & 0x80) >> 6); 
	 if (rule_index == rule_len)
	    break;
	 rule[rule_index++] = ((buf & 0x10) >> 4) + ((buf & 0x20) >> 4);
	 if (rule_index == rule_len)
	    break;
         rule[rule_index++] = ((buf & 0x4) >> 2)  + ((buf & 0x8) >> 2);
	 if (rule_index == rule_len)
	    break;
         rule[rule_index++] = (buf & 1) + (buf & 2);
      }
   }

   FILE * f_out = fopen(argv[2], "w");
   for (i = 0; i < rule_len; ++i)
      fprintf(f_out, "%d", rule[i]);
   fclose(f_out);

   // Display
   if (verbose)
   {
      for (i = 0; i < 90; ++i)
	 printf("%0x ", rule[i]);
      printf("\n");

      for (i = 0; i < 90; ++i)
	 printf("%0x ", padding[i]);
      printf("\n");
   }

   return 0;
}


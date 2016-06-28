
// Neil Babson
// March 26, 2016
// Cellular Automota message extractor
// MultiMedia Security
// Final Project

// Extrract message from CA rule
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

int verbose = 0;

int main(int argc, char* argv[])
{
   if (argc != 5)
   {
      printf("Usage: extract <rule file> <states> <neighboirhood> <message file>\n");
      return 1;
   }

   FILE* f_in;
   FILE* f_out;
   int states = atoi(argv[2]);
   int neighborhood = atoi(argv[3]);
   int rule_len = pow(states, neighborhood);
   int rule[rule_len];
   int i, j, message_len = 0;
   char state;

   if (states != 4) {
      printf("Only four state version implemented\n");
      return 1;
   }

   f_in = fopen(argv[1], "r");
   if (f_in) {
      for (i = 0; i < rule_len; ++i) {
	 state = fgetc(f_in);
	 if (state == '_')
	    rule[i] = -1;
	 else
	    rule[i] = ((int) state) - 48;
	 if (verbose)
	    printf("%d ", rule[i]);
      }
   } else {
      printf("Couldn't open rule file\n");
      return 1;
   }

   for (i = 0; i < 8; ++i) {
      if (rule[i] < 0) {
	 printf("Message length corrupted. Aborting\n");
	 return 1;
      }
      message_len += rule[i] << (2*i);
      if (verbose)
         printf("Rule[%d] = %d\t\tmessage_len = %d\n", i, rule[i], message_len);
   }
   if (verbose)
      printf("Message length: %d\n", message_len);
   
   // Retrieve message
   int letter;
   char message[message_len + 1];
   int rule_index = 8;
   for (i = 0; i < message_len; ++i) {
      letter = 0;
      if (rule[rule_index] < 0 || rule[rule_index+1] < 0 ||
	    rule[rule_index+2] < 0 || rule[rule_index+3] < 0) 
	 message[i] = '_';
      else {
	 for (j = 0; j < 4; ++j) 
            letter += rule[j + rule_index] << (2*(3-j));
         message[i] = letter;
      }
      rule_index += 4;
   }

   // This might be unneeded, the message should have its own terminating character
   //message[i] = '\0';
   
   if (verbose)
      printf("First decryption: %s\n", message);

   // Fill blanks with duplicate of message
   if (rule_len - rule_index > message_len) {
      for (i = 0; i < message_len; ++i) {
	 letter = 0;
	 if (message[i] == '_') {
	    if (!(rule[rule_index] < 0 || rule[rule_index+1] < 0 ||
		  rule[rule_index+2] < 0 || rule[rule_index+3] < 0)) { 
	       for (j = 0; j < 4; ++j)
	          letter += rule[j + rule_index] << (2*(3-j));
	       message[i] = letter;
	    }
	 }
	 rule_index += 4;
      }
   }

   // Save message to file
   f_out = fopen(argv[4], "wb");
   if (f_out) {
      fwrite(message, message_len, 1, f_out);
   } else {
      printf("Couldn't open message save file\n");
      return 1;
   }
   
   return 0;
}




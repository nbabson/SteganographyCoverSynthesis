

// Neil Babson
// March 23, 2016
// Recover the message from a synthesized cellular automaton
// MultiMedia Security
// Final Project

// Embed encrypted message in CA rule and create .ppm file of CA
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
   if (argc < 3) {
      printf("Usage: recoverMessage <input .ppm file>  <message file>\n");
      return 1;
   }

   char syst[200] = "./extractRule ";

   strcat(syst, argv[1]);
   strcat(syst, " 4 7 temp.txt");
   if (system(syst)) {
      printf("Fatal error\n");
      return 1;
   }

   strcpy(syst, "./extract temp.txt 4 7 ");
   strcat(syst, argv[2]);
   if (system(syst)) {
      printf("Fatal error\n");
      return 1;
   }

   return 0;
}

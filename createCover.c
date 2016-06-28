
// Neil Babson
// March 23, 2016
// Synthesize Cellular Automota cover from message
// MultiMedia Security
// Final Project

// Embed encrypted message in CA rule and create .ppm file of CA
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
   if (argc < 3) {
      printf("Usage: createCover <message file> <output .ppm file> [<CA padding rule file>]\n");
      return 1;
   }

   char syst[200] = "./embed ";

   strcat(syst, argv[1]);
   strcat(syst, " out.txt 4 7 ");
   if (argc == 4)
      strcat(syst, argv[3]);
   if (system(syst)) {
      printf("Fatal error\n");
      return 1;
   }

   strcpy(syst, "./makeCA out.txt 4 7 ");
   strcat(syst, argv[2]);
   if (system(syst)) {
      printf("Fatal error\n");
      return 1;
   }

   return 0;
}

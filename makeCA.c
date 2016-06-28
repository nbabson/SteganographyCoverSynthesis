
// Neil Babson
// March 23, 2016
// Cellular Automota message embedder
// MultiMedia Security
// Final Project

// Create new CA from a rule
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <openssl/rand.h>

int baseNtoDec(int * num, int base, int length);
int mod(int a, int b);

const int WIDTH = 800;
const int GENERATIONS = 600;
int verbose = 0;

int main(int argc, char* argv[])
{
   if (argc != 5)
   {
      printf("Usage: makeCA <rule file> <states> <neighborhood> <output .ppm file>\n");
      return 1;
   }
   FILE* f_in;
   int i, header, count, states = atoi(argv[2]);
   int neighborhood = atoi(argv[3]);
   int rule_len = pow(states, neighborhood);
   int rule[rule_len];
   int visited[rule_len];
   int ca[WIDTH][GENERATIONS];
   char ans;
   char syst[100]; 


   if (verbose) {
      printf("States: %d\n", states);
      printf("Neighborhood: %d\n", neighborhood);
      printf("WIDTH: %d\n", WIDTH);
   }

   f_in = fopen(argv[1], "r");
   if (f_in) {
      for (i = 0; i < rule_len; ++i)
	 rule[i] = getc(f_in) - 48;
      fclose(f_in);
   }
   else {
      printf("Couldn't open CA rule file\n");
      return 1;
   }

   if (verbose)
   {
      printf("Beginning of rule: \n");
      for (i = 0; i < 100; ++i)
	 printf("%d", rule[i]);
      printf("\n");
   }

   do {
   // Repeat until length header is installed uncorrupted
      do {
	 // Initialize flags to check if rule has been implemented
	 for (i = 0; i < rule_len; ++i)
	    visited[i] = 0;
    
	 // Make random initial generation (four states)
	 i = 0;
	 unsigned char buf;
	 while (i < WIDTH) {
	    RAND_bytes(&buf, 1);
	    ca[i++][0] = ((buf & 0x40) >> 6) + ((buf & 0x80) >> 6); 
	    if (i == WIDTH)
	       break;
	    ca[i++][0] = ((buf & 0x10) >> 4) + ((buf & 0x20) >> 4);
	    if (i == WIDTH)
	       break;
	    ca[i++][0] = ((buf & 0x4) >> 2)  + ((buf & 0x8) >> 2);
	    if (i == WIDTH)
	       break;
	    ca[i++][0] = (buf & 1) + (buf & 2);
	 }

	 if (verbose) {
	    printf("First Generation\n");
	    for (i = 0; i < 400; ++i)
	       printf("%d ", ca[i][0]);
	    printf("\n");
	 }

	 // Make rest of CA
	 int appliedRule;
	 int ruleIndex[neighborhood];
	 int start = - (neighborhood / 2);
	 int finish = neighborhood / 2;
	 int j, k, n = rule_len - 1;
	 for (i = 1; i < GENERATIONS; ++i) {
	    for (j = 0; j < WIDTH; ++j) {
	       for (k = start, count = 0; k <= finish; ++k, ++count) {
		  ruleIndex[count] = ca[mod(j+k, WIDTH)][i-1];
	       }
	       //if (verbose)
	//	  printf("base4toDec %d\n" , baseNtoDec(ruleIndex, states, neighborhood)); 
	       appliedRule = n - baseNtoDec(ruleIndex, states, neighborhood);
	       ca[j][i] = rule[appliedRule];
	       visited[appliedRule] = 1;
	    }
	 }

	 // Save CA
	 FILE * f_out = fopen("ca.txt", "w");
	 if (f_out) {
	    for (i = 0; i < GENERATIONS; ++i)
	       for (j = 0; j < WIDTH; ++j)
		  fprintf(f_out, "%d", ca[j][i]);
	    fclose(f_out);
	 }
	 else {
	    printf("Couldn't open output file\n");
	    return 1;
	 }

         header = 0;
	 for (i = 0; i < 8; ++i) {
	    if (!visited[i]) {
	       header = 1;
	       if (verbose)
		  printf("Corrupted header.  Recreating CA.\n");
	    }
         }


      }
      while (header);

      // Check if all rules have been instantiated
      count = 0;
      for (i = 0; i < rule_len; ++i)
	 if (!visited[i])
	    ++count;
      printf("%d elements of the rule were never performed.\n", count);

      strcpy(syst, "./draw ca.txt "); 
      strcat(syst, argv[4]);
      system(syst);
      puts(syst);

      printf("Do you want to re-encode this rule? (y or n) ");
      scanf(" %c", &ans);
   }
   while (toupper(ans) != 'N');

   return 0;
}

// Turns out C's % operator isn't modulo, only remainder
int mod(int a, int b)
{
   int r = a % b;
   return r < 0 ? r + b : r;
}

int baseNtoDec(int * num, int base, int length)
{
   int total = 0;
   int place = 1;
   int i, n = length - 1;

   for (i = n; i >= 0; --i) {
      total += place * num[i];
      place *= base;
  //    if (verbose)
  //       printf("Total: %d Place: %d Base: %d num[i]: %d i: %d\n", total, place, base, num[i], i);
   }
   return total;
}



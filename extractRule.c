// Neil Babson
// March 28, 2016
// Cellular Automata rule extractor
// MultiMedia Security
// Final Project

// Extract a CA rule from the .ppm image file
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

int baseNtoDec(int * num, int base, int length);
int mod(int a, int b);

int verbose = 0;

int main(int argc, char* argv[])
{
   if (argc != 5) {
      printf("Usage: ./extractRule <.ppm file> <states> <neighborhood> <rule file>\n");
      return 1;
   }
   FILE* f_in;
   FILE* f_out;
   int states = atoi(argv[2]);
   int neighborhood = atoi(argv[3]);
   int rule_len = pow(states, neighborhood);
   int rule[rule_len];
   int i, j, k;
   char* image = NULL;
   long image_len;

   // Initialize rule instances to not found
   for (i = 0; i < rule_len; ++i)
      rule[i] = -1;

   if (states != 4) {
      printf("Only four state version has been implemented\n");
      return 1;
   }

   f_in = fopen(argv[1], "rb");
   if (f_in) {
      fseek(f_in, 0, SEEK_END);
      image_len = ftell(f_in);
      fseek(f_in, 0, SEEK_SET);
      image = malloc(image_len + 1);
      if (image)
	 fread(image, 1, image_len, f_in);
      fclose(f_in);
   }
   else {
      printf("Couldn't open image file\n");
      return 1;
   }

   char* word;
   word = strtok(image, " \n\t");
   if (verbose)
      printf("file header: %s\n", word); 
   if (!word || strcmp(word, "P3")) {
      printf("CA image file must have plain ppm type\n");
      return 1;
   }

   // File Header
   int width = atoi(strtok(NULL, " \n\t")) / 3;
   int height = atoi(strtok(NULL, " \n\t")) / 3;
   if (verbose) {
      printf("Width: %d		Height: %d\n", width, height);
   }
   strtok(NULL, " \n\t");  // maxval

   // Number of .ppm to ignore for 2nd and 3rd rows of each state
   int nPer2Line = width * 18;
   int genA[width];
   int genB[width];
   // Determine whether genA or genB is the parent
   int parentFlag = 1;
   // Get first generation
   for (i = 0; i < width; ++i) {
      // State number is the value of the first color div 50
      genA[i] = atoi(strtok(NULL, " \n\t")) / 50 ;
      for (j = 0; j < 8; ++j)
	 // Discard b and g colors and next two pixels
	 strtok(NULL, " \n\t");
   }
   // Discard next two lines of pixels
   for (i = 0; i < nPer2Line; ++i)
      strtok(NULL, " \n\t");

   int* parent;
   int* child;
   int count, appliedRule;
   int ruleIndex[neighborhood];
   int start = - (neighborhood / 2);
   int finish = neighborhood / 2;
   int n = rule_len - 1;
   for (i = 1; i  < height; ++i) {
      if (parentFlag > 0) {
	 parent = genA;
	 child = genB;
      }
      else {
	 parent = genB;
	 child = genA;
      }
      // Get next generation
      for (j = 0; j < width; ++j) {
	 // State number is the value of the first color div 50
	 *(child + j) = atoi(strtok(NULL, " \n\t")) / 50 ;
	 for (k = 0; k < 8; ++k)
	    // Discard b and g colors and next two pixels
	    strtok(NULL, " \n\t");
      }
      // Discard next two lines of pixels
      for (j = 0; j < nPer2Line; ++j)
	 strtok(NULL, " \n\t");

      for (j = 0; j  < width; ++j) {
	 for (k = start, count = 0; k <= finish; ++k, ++count) {
	    ruleIndex[count] = *(parent + mod(j+k, width));
	 }
	 if (verbose) {
	    printf("[");
	    for (k = 0; k < neighborhood; ++k)
	       printf("%d,", ruleIndex[k]);
	    printf("]\n");
	 }
	 appliedRule = n - baseNtoDec(ruleIndex, states, neighborhood);
	 if (verbose)
	    printf("appliedRule: %d\n", appliedRule); 
	 rule[appliedRule] = child[j];
      }
      parentFlag *= -1;
   }

   count = 0;
   for (i = 0; i < rule_len; ++i)
      if (rule[i] == -1)
	 ++count;
   printf("%d elements of the rule were never performed.\n", count);

   // Write the rule to file
   f_out = fopen(argv[4] ,"w");
   if (f_out) {
      for (i = 0; i < rule_len; ++i) {
	 if (rule[i] == -1)
	    fprintf(f_out, "_");
	 else
	    fprintf(f_out, "%d", rule[i]);
      }
      fclose(f_out);
   }
   else {
      printf("Couldn't open output file\n");
      return 1;
   }

   return 0;
}


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
   }
   return total;
}





      


   

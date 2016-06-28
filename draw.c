
#include <stdio.h>
#include <string.h>
#include <openssl/rand.h>

const int WIDTH = 800;
const int GENERATIONS = 600;
int main(int argc, char* argv[])
{
   int i, j, k, l, states = 4;
   if (argc != 3)
   {
      printf("Usage: draw <CA file> <PPM file>\n");
      return 1;
   }

   FILE * f_out = fopen(argv[2],"w");

   fputs("P3\n", f_out);
   fprintf(f_out, "%d %d\n", 3 * WIDTH, 3 * GENERATIONS);
   //fputs("1200 600\n", f_out);
   fputs("255\n", f_out);
   
   FILE * f_in = fopen(argv[1], "r");
   int colorNum = 3 * states;
   unsigned char colors[3*states];
   unsigned char newColor[1];
   // Set all colors randomly
   RAND_bytes(colors, colorNum);

   // Set range of r value for each color (50i -> 50(i+i) - 1)
   for (i = 0; i < states; ++i)
   {
      while ((int) *(colors+(3*i)) <  50*i || (int) *(colors+(3*i)) >= 50*(i+1))
      {
	 RAND_bytes(newColor, 1);
	 *(colors+(3*i)) = *newColor;
      }
   }

   // Set equal numbers of least bit of each color
   for (i = 0; i < colorNum; ++i)
   {
      if (i % 2 == 0)
	 colors[i] = colors[i] & ~1;
      else 
	 colors[i] = colors[i] | 1;
   }
   
   int  state;
   char charState;
   int layer[WIDTH];
   for (i = 0; i < GENERATIONS; ++i)
   {
      for (j = 0; j < WIDTH; ++j)
      {
	 fscanf(f_in, " %c", &charState);
	 state = (int) charState - 48;
	 //printf("%d ",state);
	 layer[j] = state;
	 for (k = 0; k < 3; ++k)
	    fprintf(f_out, "%d %d %d ", *(colors+state*3),*(colors+state*3+1),*(colors+state*3+2));


	 if (i % 3 == 2)
	    fprintf(f_out, "\n");
      }
      for (j = 0; j < 2; ++j)
	 for (l = 0; l < WIDTH; ++l)
	    for (k = 0; k < 3; ++k)
	       fprintf(f_out, "%d %d %d ", *(colors+layer[l]*3),*(colors+layer[l]*3+1),*(colors+layer[l]*3+2));
   }
   fclose(f_in);
   fclose(f_out);


   return 0;
}






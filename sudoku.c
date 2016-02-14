#include<stdio.h>

int main()
{
  char c;
  char *inp_line = '\0'; 

  while((c = getchar()) != EOF)
  {
    ungetc(c, stdin); /* push the character back into the stream */
    gets(inp_line);
    printf(" >>> %s\n", inp_line);
  }

  return 0;
}

/*** FUNCTION DEFINTIIONS ***/

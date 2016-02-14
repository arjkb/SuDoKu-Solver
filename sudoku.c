#include<stdio.h>
#include<string.h>
#include<ctype.h>

#define TRUE 1
#define FALSE 0

int valid(char *s);
int illegal_char_error(char *s);

int main()
{
  char c;
  char *inp_line;

  while((c = getchar()) != EOF)
  {
    ungetc(c, stdin); /* push the character back into the stream */
    gets(inp_line);
    printf("%s\n", inp_line);

    if(!valid(inp_line))
    {
      printf("Error\n");
    }
    printf("\n");
  }
  return 0;
}

/*** FUNCTION DEFINTIIONS ***/
int valid(char *s)
{
  if((strlen(s) != 81) || (illegal_char_error(s)))
  {
    return FALSE;
  }
  else
  {
    return TRUE;
  }
}

int illegal_char_error(char *s)
{
  /*
   * Function that checks if the board contains an illegal character, such as
   * a non-number
   */
  int i;
  for(i = 0; i < strlen(s); i++)  
  {
    if((!isdigit(s[i])) && (s[i] != '.'))
    {
      return TRUE; /* There is an illegal character */
    }
  }
  return FALSE; /* There is no illegal character */
}

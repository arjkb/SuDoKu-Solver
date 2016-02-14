#include<stdio.h>
#include<string.h>
#include<ctype.h>

#define TRUE 1
#define FALSE 0

#define VALID 1
#define INVALID 0

int valid(char *s);
int illegal_char_error(char *s);
int validateRow(int *row);

int writeToBoard(char *s);

int board[9][9];

int main()
{
  char c;
  char *inp_line;

  while((c = getchar()) != EOF)
  {
    ungetc(c, stdin); /* push the character back into the stream */
    gets(inp_line);
    printf("%s\n", inp_line);

    if((!valid(inp_line)))
    {
      printf("Error\n");
    }
    else if(!writeToBoard(inp_line))
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

int writeToBoard(char *s)
{
  /* Writes the input line to a 9 x 9 int matrix representing a SuDoKu board
   *
   * Returns true if the write to the board finished successfully
   */

  int i, j, k;

  k = 0;
  for(i = 0; i < 9; i++)
  {
    for(j = 0; j < 9; j++)
    {
      board[i][j] = (s[k] == '.')?0:s[k];
      k++;
    }
    
    if(!validateRow(board[i]))
    {
      return INVALID;
    }
  }

  return TRUE;
}

int validateRow(int *row)
{
  /* Returns true if the row is valid, false otherwise
   */
  const int MAX_COUNT = 9;
  int i, j;
  int temp;

  for(i = 0; i < MAX_COUNT; i++)
  {
    if(row[i] == 0) continue;

    temp = row[i];
    for(j = 0; j < MAX_COUNT; j++)
    {
      if((i != j) && (temp == row[j]))
      {
        return INVALID;
      }
    }
  }
  return VALID;
}

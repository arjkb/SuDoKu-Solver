#include<stdio.h>
#include<string.h>
#include<ctype.h>

#define TRUE 1
#define FALSE 0

#define VALID 1
#define INVALID 0

#define ROWS 9
#define COLS 9

int valid(const char *s);
int illegal_char_error(const char *s);

int validate_row(const int *row);
int validate_columns(int board[ROWS][COLS]);

int validate_boxes(int board[ROWS][COLS]);
int validate_3x3(int board[ROWS][COLS], const int big_row, const int big_col);

int toDigit(char);

int writeToBoard(int board[ROWS][COLS], const char *s);

void print_board(int board[ROWS][COLS], const char *message);

int solve(int board[ROWS][COLS]);
int initial_sweep(int board[ROWS][COLS]);
int exist_row(const int *row, const int element, const int current_index);

/*
int ifexist_col(const int board[ROWS][COLS], const int element);
int ifexist_3x3(const int element, const int row_start, const int col_start);
*/

const int MAX_COUNT = 9;

int main()
{
  int board[ROWS][COLS];
  int solvecount =  0;

  char c;
  char inp_line[82];

  while((c = getchar()) != EOF)
  {
    ungetc(c, stdin); /* push the character back into the stream */
    gets(inp_line);
    printf("%s\n", inp_line);

    if((!valid(inp_line)))
    {
      printf("Error\n");
    }
    else if(!writeToBoard(board, inp_line))
    {
      printf("Error\n");
    }
    else
    {
      /* solve the SuDoKu */
      print_board(board, "Board:"); 

       solvecount = solve(board);

       if( solvecount > 0 ) 
       {
         print_board(board, "Solved: ");
       }

    }
    
    printf("\n");
  }
  return 0;
}

/*** FUNCTION DEFINTIIONS ***/

int solve(int board[ROWS][COLS])
{
  return initial_sweep(board);
}

int initial_sweep(int board[ROWS][COLS])
{
  /* Checks for any obvious squares */
  int possibility_count = 0; /* keeps track of no of diff 
                              possible values in a square */
  int val, candidate;
  int changed;
  int tot_change = 0;

  int i, j;

  do  
  {
    changed = 0;

    for(i = 0; i < ROWS; i++)  
    {
      for(j = 0; j < COLS; j++)
      {
        if(board[i][j] == 0)
        {
          /* Attempt filling only if the board is empty */
        
          possibility_count = 0;

          for(val = 1; val <= MAX_COUNT; val++)
          {

            if(!exist_row(board[i], val, j))
            {
              candidate = val;
              possibility_count++;
            }
          } 

          if(possibility_count == 1)
          {
            printf(" FILLING IN %d at (%d, %d)", candidate, i, j);
            board[i][j] = candidate;
            changed = 1;
            tot_change++;
          }
        }
      } 
    }
  } while( changed );
  return tot_change;
}

int exist_row(const int *row, const int element, const int current_index)
{
  /* Returns true if element exists in the row
   */
  int i;
  for(i = 0; i < MAX_COUNT; i++)
  {
    if((element == row[i]) && (i != current_index))
    {
      return 1;
    }
  }
  return 0;
}

int valid(const char *s)
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

int illegal_char_error(const char *s)
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

int writeToBoard(int board[][COLS], const char *s)
{
  /* Writes the input line to a 9 x 9 int matrix representing a SuDoKu board
   *
   * Returns true if the write to the board finished successfully
   */

  int i, j, k;

  k = 0;
  for(i = 0; i < ROWS; i++)
  {
    for(j = 0; j < COLS; j++)
    {
      board[i][j] = (s[k] == '.')?(0):(toDigit(s[k]));
      k++;
    }
    
    if(!validate_row(board[i]))
    {
      return INVALID;
    }
  }

  if(!validate_columns(board))  { return INVALID; }
  if(!validate_boxes(board))  { return INVALID; }

  return TRUE;
}

int validate_row(const int *row)
{
  /* Returns true if the row is valid, false otherwise
   */
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

int validate_columns(int board[ROWS][COLS]) 
{
  const int MAX_COUNT = 9;
  int i, j, k;
  int temp;

  for(i = 0; i < ROWS; i++)
  {
    for(j = 0; j < COLS; j++)
    {
      if(board[j][i] == 0) continue;

      temp = board[j][i];
      for(k = 0; k < MAX_COUNT; k++)
      {
        if((k != j) && (board[k][i] == temp))
        {
          return INVALID;
        }
      }      
    }
  }
  return VALID;
}

int validate_boxes(int board[ROWS][COLS]) {
  int i, j;
  for(i = 0; i < 3; i++)
  {
    for(j = 0; j < 3; j++)
    {
      if(validate_3x3(board, i, j) == INVALID)
      {
        return INVALID;
      }
    }
  }
  return VALID;
}

int validate_3x3(int board[ROWS][COLS], const int big_row, const int big_col)
{
  int m, n;
  int i, j;
  int row_start = 0, col_start = 0;

  int temp;

  switch(big_row)
  {
    case 0: row_start = 0; break;
    case 1: row_start = 3; break;
    case 2: row_start = 6; break;
    default: return INVALID;
  }

  switch(big_col)
  {
    case 0: col_start = 0; break;
    case 1: col_start = 3; break;
    case 2: col_start = 6; break;
    default: return INVALID;
  }

  for(m = row_start; m < (row_start + 3); m++)
  {
    for(n = col_start; n < (col_start + 3); n++)
    {
      if(board[m][n] == 0) continue;

      temp = board[m][n]; 
      for(i = row_start; i < (row_start + 3); i++)
      {
        for(j = col_start; j < (col_start + 3);j++)
        {
          if((temp == board[i][j]) && (m != i) && (n != j))
          {
            return INVALID;
          }
        }
      }
    }
  } /* end of outermost for-loop */

  return VALID;
}

void print_board(int board[ROWS][COLS], const char *message)  {
  int i, j;

  printf(" >>> %s\n", message);
  for(i = 0; i < ROWS; i++)
  {
    for(j = 0; j < COLS; j++)
    {
      if((j == 3) || (j == 6))
      {
        printf(" ");
      }
      printf("%d", board[i][j]);
    }
    printf("\n");
    if((i == 2) || (i == 5))
    {
      printf("\n");
    }
  }
}

int toDigit(char c)
{
  return c - '0';
}

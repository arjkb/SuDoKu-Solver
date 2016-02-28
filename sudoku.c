/*
 * Program to solve a sudoku puzzle.
 *
 * Arjun Krishna Babu
 * (Fedora 24 running on Intel i5)
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<math.h>

#define DEBUG

#define TRUE 1
#define FALSE 0

#define VALID 1
#define INVALID 0

#define ROWS 9
#define COLS 9

#define ON 2
#define OFF 3

#define SUCCESS 1
#define FAIL  0

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
int exist_col(int board[ROWS][COLS], const int element, const int curr_row, const int curr_col);
int exist_3x3(int board[ROWS][COLS], const int element, const int curr_row, const int curr_col);

int initial_sweep_bitwise(int board[ROWS][COLS]);

void get_fillable_square(int board[ROWS][COLS], int *r, int *c);
int fill(int board[ROWS][COLS], int candidate[ROWS][COLS]);

#ifdef OLD_FILL
int fill(int board[ROWS][COLS], int candidate[ROWS][COLS], const int curr_row, const int curr_col);
void getNextEmptySquare(int board[ROWS][COLS], const int curr_row, const int curr_col, int *next_row, int *next_col);
#endif

int count_empty_squares(int B[ROWS][COLS]);
void getPossibleValues(const int candidates, int *values, int *size_of_values);

void setbit(int *, const int);
void clearbit(int *, const int);
void clear_all_bits(int *bitpattern);
int checkbit(int, const int);
int getBitval(const int);
int is_single_bit_on(const int n);
int get_single_set_position(const int bitpattern);

void initializeCandidates();
void determineCandidates(int board[ROWS][COLS], int candidates[ROWS][COLS]);

void printCandidates(const int *array, const int LEN, const int r, const int c);
void printLinear(int array[ROWS][COLS]);

void copyMatrix(int source[ROWS][COLS], int dest[ROWS][COLS]);

void turn_bit_row(const int OPCODE, int candy[ROWS][COLS], const int R, const int bit);
void turn_bit_col(const int OPCODE, int candy[ROWS][COLS], const int C, const int bit);

void turnoff_bit_box(const int ROW, const int COL, const int bit);

/*
int ifexist_col(const int board[ROWS][COLS], const int element);
int ifexist_3x3(const int element, const int row_start, const int col_start);
*/

const int MAX_COUNT = 9;

/* Keeps track of candidates for a square (as bit patterns) */
int candidates[ROWS][COLS];

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
/*        print_board(board, "Partially Filled in Board: "); */

        printLinear(board);
      }
    }
    
    printf("\n");
  }
  return 0;
}

/*** FUNCTION DEFINTIIONS ***/

int solve(int board[ROWS][COLS])
{
  /* return initial_sweep(board); */

  int i = 0, j = 0;

  int r_isb = -2;
  int r_fill = -2;

#ifdef DEBUG
  printf("\n DEBUG: INSIDE solve()");
#endif

/*  getNextEmptySquare(board, 0, 0, &i, &j); */

  r_isb = initial_sweep_bitwise(board);
  r_fill = fill(board, candidates);

#ifdef OLD_FILL
  r_fill = fill(board, candidates, i, j);
#endif

#ifdef DEBUG
  printf("\n r_isb: %d", r_isb);
  printf("\n r_fill: %d", r_fill);
#endif

  if(r_fill == SUCCESS) 
  {
    printf("\n SOLVED!");
  }

  return (r_isb && r_fill);  
}

void initializeCandidates(int board[ROWS][COLS])  
{
  int SET_1_9 = 1022;
  int i, j;

#ifdef DEBUG
  printf("\n DEBUG: inside initializecandidates()");
#endif

  for(i = 0; i < ROWS; i++)
  {
    for(j = 0; j < COLS; j++)
    {
      if(board[i][j] == 0)
      {
        candidates[i][j] = SET_1_9;
      }
      else
      {
        candidates[i][j] = 0;
      }
    }
  }

}

void copyMatrix(int source[ROWS][COLS], int dest[ROWS][COLS])
{
  int i, j;
  for(i = 0; i < ROWS; i++)
  {
    for(j = 0; j < ROWS; j++)
    {
      dest[i][j] = source[i][j];
    }
  }
}

/** Backtracker Funtion Definitions **/

int fill(int board[ROWS][COLS], int candy[ROWS][COLS])
{
  /* Fills the board */

  /* copy board to B, and candy to C
   * get the next fillable square
   *
   * Add the candidates of that square into VAL
   *
   * Add an element of VAL into the square
   * call fill()
   */

  /* BASE CASES
   *
   * When candidates of an empty  square cannot be determined 
   * (since it got cancelled away due to previous erroneus entries)
   *  return FAIL
   *
   * When there are no more empty squares
   *  copy B back to board
   *  return SUCCESS
   */ 
  
  /* 2D arrays to store local copies of board and candy */
  int B[ROWS][COLS];
  int C[ROWS][COLS];

  int val[9] = {0};
  int size = 0;
  int k = 0;

  int fillable_row, fillable_col;

  int empty_count = 1;

  int ret_val = FAIL;

#ifdef DEBUG
    printf("\n Inside fill()");
#endif


  /* Copy data in to B and C */
  copyMatrix(board, B);
  copyMatrix(candy, C);

  determineCandidates(B, C); 

  empty_count = count_empty_squares(B);

#ifdef DEBUG
    printf("\n Empty Count: %d", empty_count);
#endif

  if(empty_count) 
  {
    /* If there are still empty squares... */

    get_fillable_square(B, &fillable_row, &fillable_col);
    getPossibleValues(C[fillable_row][fillable_col], val, &size);

#ifdef DEBUG
    printCandidates(val, size, fillable_row, fillable_col);        
#endif
    while( k < size )
    {
      copyMatrix(candy, C);
      #ifdef DEBUG
        printCandidates(val, size, fillable_row, fillable_col);        
        printf("\n Filling in %d at (%d, %d): ", val[k], fillable_row, fillable_col);
      #endif

      B[fillable_row][fillable_col] = val[k++];
#ifdef DEBUG
      printLinear(B);
#endif
      determineCandidates(B, C);

      ret_val = fill(B, C);

      if( ret_val == SUCCESS )  
      {
        #ifdef DEBUG
          printf("\n Got back SUCCESS! (%d, %d)", fillable_row, fillable_col);
        #endif

        copyMatrix(B, board);
        copyMatrix(C, candy);
        return SUCCESS;
      }
    } /* end of while() */

    B[fillable_row][fillable_col] = 0;
#ifdef DEBUG
    printf("\n Returning FAIL! (%d, %d)", fillable_row, fillable_col);
#endif

    return FAIL;
  }
    
  if( ret_val == SUCCESS || empty_count == 0 )
  {
    /* No more empty squares */
    /* BASE CASE OF SUCCESFULL SOLUTION! */
    copyMatrix(B, board);
#ifdef DEBUG
    printf("\n Returning SUCCESS! (%d, %d)", fillable_row, fillable_col);
#endif
    return SUCCESS;
  }

} /*** END OF FILL() ***/


int count_empty_squares(int B[ROWS][COLS])
{
  /* returns the number of squares that are empty.
   * Does this by counting the number of squares with a 0 filled in
   */

  int i, j;
  int count = 0;

  for(i = 0; i < ROWS; i++)
  {
    for(j = 0; j < COLS; j++)
    {
      if(B[i][j] == 0)  count++;
    }
  }
  return count;
}

void getPossibleValues(int candidates, int values[9],  int *size)
{
  int i = 0, b;

  *size = 0;

#ifdef DEBUG
  printf("\n Candidate in GPV: %d\n", candidates);
#endif

  for(b = 0; b < 10; b++)
  {
    if(checkbit(candidates, b))
    {
      values[i] = b;
      i++;
    }
  }

  *size = i;

#ifdef DEBUG
  printf("\n Size in GPV: %d", *size);
#endif
}

void get_fillable_square(int board[ROWS][COLS], int *r, int *c)
{
  int i, j;

  *r = -1;
  *c = -1;

  for(i = 0; i < ROWS; i++)
  {
    for(j = 0; j < COLS; j++)
    {
      if(board[i][j] == 0)
      {
        *r = i;
        *c = j;
        return;
      }
    }
  }
  
  printf("\n POTENTIAL ERROR: Didn't find an empty square! (This is a point you aren't supposed to reach!)");
}

#ifdef OLD_FILL
void getNextEmptySquare(int board[ROWS][COLS], 
                        const int curr_row, const int curr_col, 
                        int *next_row, int *next_col) 
{
  int i = curr_row, j = curr_col + 1;

  *next_row = -1;
  *next_col = -1;

  for(i = curr_row; i < ROWS; i++)
  {
    for(j = 0; j < COLS; j++)
    {
#ifdef DEBUG
      if( i == 0 && j == 5 )  
      {
        printf("\n BOO: %d", board[i][j]);
      }
#endif      

      if(board[i][j] == 0)  
      {
        *next_row = i;
        *next_col = j;
        return;
      }
    }
  }

#ifdef DEBUG
  printf("\n Reached end of getNES\n");
#endif
}
#endif


void printCandidates(const int *array, const int LEN, 
                      const int r, const int c)
{
  int i;
  printf("\n Candidates at (%d, %d): ", r, c);
  for(i = 0; i < LEN; i++)  
  {
    printf(" %d", array[i]);
  }
}

void printLinear(int array[ROWS][COLS])
{
  int i, j;
  for(i = 0; i < ROWS; i++)
  {
    for(j = 0; j < COLS; j++)
    {
      printf("%d", array[i][j]);
    }
  }
}

void determineCandidates(int board[ROWS][COLS], int cand[ROWS][COLS]) 
{
  int i, j, val;

  initializeCandidates(board);

  /* Go row-wise */
  for(i = 0; i < ROWS; i++)
  {
    for(j = 0; j < COLS; j++)
    {
      if(board[i][j] != 0) continue;

      for(val = 1; val <= MAX_COUNT; val++)
      {
        if(exist_row(board[i], val, j))
        {
          /* clear the bits for impossible candidates 
           * if value exists in the same row 
           */
          clearbit(&cand[i][j], val);
#ifdef DEBUG
          if((i == 0) && (j == 2) && (val == 7))
          {
            printf("\n Cleared %d at (%d, %d)!!", val, i, j);
          }
#endif          
        }
      
        if(exist_col(board, val, i, j))
        {
          /* clear the bits for impossible candidates if value exists in the same column 
           */
          clearbit(&cand[i][j], val);
#ifdef DEBUG
          if( i == 0 && j == 2 && val == 7 )
          {
            printf("\n Cleared %d at (%d, %d)!!", val, i, j);
          }
#endif          
        }

        if(exist_3x3(board, val, i, j))
        {
          clearbit(&cand[i][j], val);
#ifdef DEBUG
          if( i == 0 && j == 2 && val == 7 )
          {
            printf("\n Cleared %d at (%d, %d)!!", val, i, j);
          }
#endif          
        }
      }
    }
  }
}

int initial_sweep_bitwise(int board[ROWS][COLS])
{
  int i, j, count, tot_change;

  count = 0;
  tot_change = 0;


#ifdef DEBUG
  printf("\n DEBUG: inside initial_sweep_bitwise() INTIALIZING");
#endif
  
  initializeCandidates(board);

#ifdef DEBUG
  printf("\n INITIALIZED\n");
#endif

/*  determineCandidates(board); */

  do  
  {
    determineCandidates(board, candidates);
    count = 0;
    for(i = 0; i < ROWS; i++)
    {
      for(j = 0; j < COLS; j++)
      {
        if((board[i][j] == 0) && is_single_bit_on(candidates[i][j])) 
        {
          /* Fill the number if only a single bit is turned on */

          board[i][j] = get_single_set_position(candidates[i][j]);
          clear_all_bits(&candidates[i][j]);
          count++;

          #ifdef DEBUG
            printf("\n FILLING NUMBER");
          #endif
        }
      }
    }

    tot_change += count;
  }while(count > 0);

#ifdef DEBUG
  printf("\n Total Change: %d", tot_change);
#endif

  return tot_change;
}

int initial_sweep(int board[ROWS][COLS])
{
  /* Checks for any obvious squares */
  
  /* Possibility counts keep track of number of
   * different possible values in a square
   */
  int possibility_count_r = 0;
  int possibility_count_c = 0;

  int val, candidate_r, candidate_c;
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
        
          possibility_count_r = 0;
          possibility_count_c = 0;

          for(val = 1; val <= MAX_COUNT; val++)
          {

            /*  Tries to see if a val is possible in a square (i, j)
             *  (by checking if the value exists anywhere else in that same row, col).
             *
             *  If the val is the only possible candidate for a square, 
             *  it is assigned to that square.
             */
            if(!exist_row(board[i], val, j))
            {
              candidate_r = val;
              possibility_count_r++;
            }

            if(!exist_col(board, val, i, j))
            {
              candidate_c = val;
              possibility_count_c++;
            }
          } 

          if(possibility_count_r == 1)
          {
            printf(" R FILLING IN %d at (%d, %d)", candidate_r, i, j);
            board[i][j] = candidate_r;
            changed++;
            tot_change++;
          }

          if(possibility_count_c == 1)
          {
            printf(" C FILLING IN %d at (%d, %d)", candidate_c, i, j);
            board[i][j] = candidate_c;
            changed++;
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

int exist_col(int board[ROWS][COLS], const int element, const int curr_row, const int curr_col)
{
  /* Returns true of element exists in the current column */

  int i;
  for(i = 0; i < MAX_COUNT; i++)  
  {
    if((element == board[i][curr_col]) && (i != curr_row))
    {
      return 1;
    }
  }
  return 0;
}

int exist_3x3(int board[ROWS][COLS], const int element, const int row, const int col)
{
  /* returns true if elements exist in the current box
   * false otherwise
   */

  int i, j;
  int row_start;
  int col_start;

  const int BIG_ROW = (int) row/3;
  const int BIG_COL = (int) col/3;

  switch(BIG_ROW)
  {
    case 0: row_start = 0; break;
    case 1: row_start = 3; break;
    case 2: row_start = 6; break;
    default: return INVALID;
  }

  switch(BIG_COL)
  {
    case 0: col_start = 0; break;
    case 1: col_start = 3; break;
    case 2: col_start = 6; break;
    default: return INVALID;
  }

  for(i = row_start; i < (row_start + 3); i++)
  {
    for(j = col_start; j < (col_start + 3); j++)
    {
      if((element == board[i][j]) && (i != row) && (j != col))  {
        return 1;
      }
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
 
/** Bit manipulation operations **/
int getBitval(const int n)  
{
  /* helper function to return a bit pattern that 
   * returns an int with the nth bit turned on
   */

  return (int) pow(2, (int)n);
}

int checkbit(int n, const int b)  
{
  /* returns true if bth bit in n is set
   *  0 otherwise
   */
  
  return (n & (getBitval(b)))?1:0;
}

void setbit(int *n, const int b)  
{
  /* sets the bth bit of n */

  *n |= getBitval(b);
}

void clearbit(int *n, const int b)  
{
  /* clears bth bit of n */

  *n &= ~(getBitval(b));

}

void clear_all_bits(int *bitpattern)
{
  *bitpattern &= 0;
}

int is_single_bit_on(const int x) 
{
  return ((x & (x-1)) == 0)?1:0;
}

int get_single_set_position(const int bitpattern)
{
  switch(bitpattern)
  {
    case 1: return 0;

    case 2: return 1;
    case 4: return 2;
    case 8: return 3;

    case 16: return 4;
    case 32: return 5;
    case 64: return 6;

    case 128: return 7;
    case 256: return 8;
    case 512: return 9;
    
    default: return -1;
   }
}

#include<stdio.h>
#include<stdlib.h>
#include<limits.h>

#define MAX_SIZE 10

static int ROW_STACK[MAX_SIZE];
static int COL_STACK[MAX_SIZE];
static int NUM_STACK[MAX_SIZE];

int top = -1;

void push(const int r, const int c, const int n);
void pop();
void pop_fill(int *, int *, int *);
void disp_stack();

int main()  {
  int i;
    
  printf("\n All change is hard at first, messy in the middle, and gorgeous at the end");

  srand(100);

  /* Fill up ten elements with random values */
  for(i = 0; i < MAX_SIZE; i++) {
    push(rand() % 100, rand() % 100, rand() % 100);
  }

  disp_stack();

  for(i = 0; i < MAX_SIZE + 2; i++) {
    pop();
    disp_stack();
  }

  return 0;
}

/** Stack Operations **/
void disp_stack() {
  int i;
  if(top < 0) {
    printf("\n Error: Stack Empty!");
  }
  else  {
    for(i = top; i >= 0; i--) {
      printf("\n %d \t %d \t %d ", ROW_STACK[i], COL_STACK[i], NUM_STACK[i]);
    }
    printf("\n-----------------------------");
  }
}

void push(const int r, const int c, const int n)  {
  if(top < (MAX_SIZE - 1)) {
    top++;
    ROW_STACK[top] = r;
    COL_STACK[top] = c;
    NUM_STACK[top] = n;
  }
  else  {
    printf("\n Error: Overflow!");
  }
}

void pop()  {
  if(top > -1)  top--;
  else          printf("\n Error: Underflow!");
}

int peek_r()  
{
  if(top > -1)
  {
    return ROW_STACK[top];
  }
  else
  {
    #ifdef DEBUG
      printf("\n ERROR: peek_r: underflow!");
    #endif
    return -1;
  }
}

int peek_c()  
{
  if(top > -1)
  {
    return COL_STACK[top];
  }
  else
  {
    #ifdef DEBUG
      printf("\n ERROR: peek_c: underflow!");
    #endif
    return -1;
  }
}

int peek_n()  
{
  if(top > -1)
  {
    return NUM_STACK[top];
  }
  else
  {
    #ifdef DEBUG
      printf("\n ERROR: peek_n: underflow!");
    #endif
    return -1;
  }
}

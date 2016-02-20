#include<stdio.h>
#include<math.h>

#define ZERO 1

#define ONE 2
#define TWO 4
#define THREE 8
#define FOUR 16
#define FIVE 32
#define SIX 64
#define SEVEN 128
#define EIGHT 256
#define NINE 512

void setb(int *, int);
void clearb(int *, int);
int checkBit(int, int);
int getBitval(int);

int main()  {
  int a = 0;
  int i;

  const int TW = 2;
  const int TH = 4;
//  printf("\n Size of int: %d", sizeof(int));

  printf("\n Initial value of A: %d", a);

  a |= TWO;

  printf("\n After ORing TWO: %d", a);

  a |= THREE;

  printf("\n After ORing THREE: %d", a);

  a &= ZERO;

  printf("\n After ANDing 0 (to clear all bits): %d", a);

  setb(&a, 6);
  setb(&a, 5);
  setb(&a, 4);

  printf("\n After setting 6th bit: %d", a);

  for(i = 0; i < 10; i++) {
    printf("\n Testing bit  %d of %d: %d", i, a, checkBit(a, i));
  }

  clearb(&a, 5);

  for(i = 0; i < 10; i++) {
    printf("\n Testing bit  %d of %d: %d", i, a, checkBit(a, i));
  }

  return 0;
}

// Function definitions

void setb(int *n, int b)  {
  // sets the bth bit of n

  *n |= getBitval(b);
}

int getBitval(int n)  {
  return (int) pow(2, n);
}

int checkBit(int n, int b)  {
  // returns true if bth bit in n is set
  // 0 otherwise
  
  return (n & (getBitval(b)))?1:0;
}

void clearb(int *n, int b)  {
  *n &= ~(getBitval(b));
}


all:	sudoku

sudoku: sudoku.c
	gcc sudoku.c -lm -Wall -ansi -pedantic -o sudoku
clean:
	rm sudoku

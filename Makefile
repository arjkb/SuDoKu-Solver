all:	sudoku

sudoku: sudoku.c
	gcc -g sudoku.c -lm -Wall -ansi -pedantic -o sudoku
clean:
	rm sudoku

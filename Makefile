all:	sudoku

sudoku: sudoku.c
	gcc -Wall -ansi -pedantic -o sudoku sudoku.c
clean:
	rm sudoku

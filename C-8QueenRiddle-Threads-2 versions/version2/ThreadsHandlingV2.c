/*
 * ThreadsHandling.c
 *
 *  Created on: 6 Jan 2020
 *      Author: Ido Zada (203531058) & Guy Rinsky (205701907)
 */

#include "ThreadsHandlingV2.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>


//Checks if the input (chess) is valid by a couple of metrics
int fixChessBoard(char* chess, char* fix_chess) {
	int i, j = 0;
	for (i = 0; i < NUMBER_OF_CHARS && j <= 63; i++) {
		if (('a' <= *(chess + i) && *(chess + i) <= 'z')
				|| ('A' <= *(chess + i) && *(chess + i) <= 'P')
				|| ('R' <= *(chess + i) && *(chess + i) <= 'Z')) {
			return -1;
		}
		if ((*(chess + i) == 'Q') || (*(chess + i) == '.')) {
			*(fix_chess + j) = *(chess + i);
			j++;
		}
	}
	if (counter(fix_chess) == NUMBER_OF_FIX_CHARS) {
		return 0;
	} else
		return -1;

}

//Check's the amount  of the chars 'Q' and '.' in the matrix
int counter(char* fix_chess) {
	int i;
	int counter = 0;
	for (i = 0; i < NUMBER_OF_FIX_CHARS; i++) {
		if ((*(fix_chess + i) == 'Q') || (*(fix_chess + i) == '.')) {
			counter++;
		}
	}
	return counter;
}

//Pinpoints the thread to the designated function
int chess_is_legal(char* chess, char type, int index) {
	check c;
	switch (type) {
	case ROW:
		c = checkRowQueen(chess, index);
		if (c == LEGAL) {
			return LEGAL;
		}

		break;
	case COL:
		c = checkColQueen(chess, index);
		if (c == LEGAL) {
			return LEGAL;
		}
		break;
	case LEFT_DIAG:
		if(index <= MAIN_DIAGONAL_LOC){
			c = check_Diagonal_Left(chess, index,0);
		}else{
			c = check_Diagonal_Left(chess,14-index,1);
		}
		if (c == LEGAL) {
			return LEGAL;
		}
		break;
	case RIGHT_DIAG:
		c = check_Diagonal_Right(chess, index);
		if (c == LEGAL) {
			return LEGAL;
		}
		break;
	}
	return ILLEGAL;

}

//Checks a row if it has only 1 'Q' (Queen)
int checkRowQueen(char* chess, int index) {
	int i, check = ILLEGAL;
	for (i = 0; i < SIZE; i++) {
		if (*(chess + (index * SIZE + i)) == 'Q') {
			if (check == ILLEGAL) {
				check = LEGAL;
			} else {
				return ILLEGAL;
			}
		}
	}
	if (check == ILLEGAL) {
		return ILLEGAL;
	} else {
		return LEGAL;
	}
}

//Checks a column if it has only 1 'Q' (Queen)
int checkColQueen(char* chess, int index) {
	int i, check = ILLEGAL;
	for (i = 0; i < SIZE; i++) {
		if (*(chess + (i * SIZE + index)) == 'Q') {
			if (check == ILLEGAL) {
				check = LEGAL;
			} else {
				return ILLEGAL;
			}
		}
	}
	if (check == ILLEGAL) {
		return ILLEGAL;
	} else {
		return LEGAL;
	}

}
//Checks the board (matrix) if there is maximum 1 'Q' in every diagonal
//while checking it each diagonal at a time
int check_Diagonal_Left(char* chess, int index, int type) {
	int check = ILLEGAL, j, generic_var1, generic_var2;
	if (index == FIRST_OR_LAST_ITER)
		return LEGAL;
	//Chooses how to initialize the generic variables depending on the variable 'type'
	switch (type) {
	case DOWN_SIDE:
		generic_var1 = MAIN_DIAGONAL_LOC;
		generic_var2 = index;
		break;
	case UP_SIDE:
		generic_var2 = MAIN_DIAGONAL_LOC;
		generic_var1 = index;
		break;
	}
	//A generic "for" to compute for the upper triangle
	//side of the board (matrix) and the down triangle side of the board (matrix)
	//by initializing generic variables accordingly, depending on the variable 'type'
	for (j = MAIN_DIAGONAL_LOC; j >= MAIN_DIAGONAL_LOC - index; j--,generic_var1--,generic_var2--) {
		if (*(chess + (generic_var1 * SIZE + generic_var2)) == 'Q') {
			if (check == ILLEGAL) {
				check = LEGAL;
			} else {
				return ILLEGAL;
			}
		}
	}
	return LEGAL;
}

//Checks the board (matrix) if there is maximum 1 'Q' in every diagonal
//while checking it each diagonal at a time
int check_Diagonal_Right(char* chess, int index) {
	int check = ILLEGAL,diag_count = MAIN_DIAGONAL_LOC,generic_var,generic_var2, j;
	if (index == FIRST_OR_LAST_ITER)
		return LEGAL;
	//Chooses how to initialize the generic variables depending on the variable 'index'
	if( index <= MAIN_DIAGONAL_LOC){
		diag_count = index;
		generic_var = 0;
		generic_var2 = index;
	}else{
		generic_var = index - diag_count;
		generic_var2 = diag_count;
	}
	//A generic "for" to compute for the upper triangle
	//side of the board (matrix) and the down triangle side of the board (matrix)
	//by initializing generic variables accordingly, depending on the variable 'index'
	for (j = generic_var; j <=  generic_var2; j++, diag_count--) {
		if (*(chess + (diag_count * SIZE + j)) == 'Q') {
			if (check == ILLEGAL) {
				check = LEGAL;
			} else {
				return ILLEGAL;
			}
		}
	}
	return LEGAL;
}

void freeAllocation(task** tasks){
	int i;
	for(i = 0 ; i < NUM_OF_TASKS ;i++){
		free(tasks[i]);
	}
	free(tasks);
}


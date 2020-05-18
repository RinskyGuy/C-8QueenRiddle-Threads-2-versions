/*
 * ThreadsHandling.h
 *
 *  Created on: 6 Jan 2020
 *      Author: Ido Zada (203531058) & Guy Rinsky (205701907)
 */

#ifndef PROCESSHANDLING_H_
#define PROCESSHANDLING_H_

#define NUM_OF_THREADS 46

#define NUMBER_OF_CHARS 1000
#define NUMBER_OF_FIX_CHARS 64


#define MAIN_DIAGONAL_LOC 7
#define FIRST_OR_LAST_ITER 0
#define SIZE 8
#define ROW 		 0
#define COL 		 1
#define LEFT_DIAG    2
#define RIGHT_DIAG   3
typedef enum {
	ILLEGAL, LEGAL
} check;

typedef enum {
	DOWN_SIDE, UP_SIDE
} type;

typedef struct{
	char* fixed_chess;
	int task_choice;
	int index;
}thread_data;

int counter(char* fix_chess);
int chess_is_legal(char* chess , char type,int index);
int fixChessBoard(char* chess, char* fix_chess);
int checkRowQueen(char* chess, int index);
int checkColQueen(char* chess, int index);
int check_Diagonal_Left(char* chess, int index, int type) ;
int check_Diagonal_Right(char* chess, int index);
void initialization(thread_data* type, int task_choice, char* chess, int index);




#endif /* PROCESSHANDLING_H_ */

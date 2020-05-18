/*
 * ThreadsHandling.h
 *
 *  Created on: 6 Jan 2020
 *      Author: Ido Zada (203531058) & Guy Rinsky (205701907)
 */

#ifndef PROCESSHANDLING_H_
#define PROCESSHANDLING_H_

#define NUM_OF_TASKS   46
#define N_THREADS 4

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
	char* chess;
	int offset;
	int task_type;
	int done;
}task;


int counter(char* fix_chess);
int chess_is_legal(char* chess , char type,int index);
int fixChessBoard(char* chess, char* fix_chess);
int checkRowQueen(char* chess, int index);
int checkColQueen(char* chess, int index);
int check_Diagonal_Left(char* chess, int index, int type) ;
int check_Diagonal_Right(char* chess, int index);
void initialization(task** tasks,char* chess);
void freeAllocation(task** tasks);
void* directory_func(void* args);



#endif /* PROCESSHANDLING_H_ */

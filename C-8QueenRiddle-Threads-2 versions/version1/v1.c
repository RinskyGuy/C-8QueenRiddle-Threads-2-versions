/*
 * main.c
 *
 *  Created on: 6 Jan 2020
 *      Author: Ido Zada (203531058) & Guy Rinsky (205701907)
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include "ErrorManagement.h"
#include "ThreadsHandling.h"
#include <pthread.h>

int res[NUM_OF_THREADS];

//Classifies each index by its task and returns the proper index to input result
int indexClassification(int task_choice, int index) {
	switch (task_choice) {
	case COL:
		return index + 8;
	case LEFT_DIAG:
		return index + 16;
	case RIGHT_DIAG:
		return index + 31;
	default:
		return index;
	}
}

//Sends each thread to its designated function
void* directory_func(void* args) {
	int result_location;
	thread_data* th = (thread_data*) args;
	result_location = indexClassification(th->task_choice, th->index);
	res[result_location] = chess_is_legal(th->fixed_chess, th->task_choice,
			th->index);
	free(args);
	return NULL;
}

int main(int argc, const char* argv[]) {
	char chess[NUMBER_OF_CHARS], fix_chess[NUMBER_OF_FIX_CHARS];
	int std_in = -1; //File descriptor
	int i, j, l, result = 1;
	pthread_t threads[NUM_OF_THREADS];
	thread_data *row, *col, *l_diagonal, *r_diagonal;
	//Checks if we need to get
	//input from Standard Input or from a File
	if (argc <= 1) {
		argc = 2;
		std_in = STDIN_FILENO;
	}
	//If our input is from a File, then open std_in as File Descriptor
	if (std_in != STDIN_FILENO)
		check_error((std_in = open(argv[1], O_RDONLY)), argv[0], OPEN_ERR);
	check_error(read(std_in, chess, NUMBER_OF_CHARS), argv[0], READ_ERR);
	//If our input is from a File, then close the File Descriptor std_in
	if (std_in > 0)
		check_error(close(std_in), argv[0], CLOSE_ERR);
	check_error(fixChessBoard(chess, fix_chess), argv[0], BOARD_ERR);

	for (i = 0; i < NUM_OF_THREADS; i++) {
		//Allocate memory here instead of outside because race condition
		row = malloc(sizeof(thread_data));
		col = malloc(sizeof(thread_data));
		l_diagonal = malloc(sizeof(thread_data));
		r_diagonal = malloc(sizeof(thread_data));

		//8 threads to check rows
		if (i < 8) {
			initialization(row, ROW, fix_chess, i);
			check_error(
					pthread_create(&threads[i], NULL, directory_func,
							(void *) row), argv[0], CREATE_THREAD_ERR);
			//8 threads to check columns
		} else if (i >= 8 && i < 16) {
			initialization(col, COL, fix_chess, i - 8);
			check_error(
					pthread_create(&threads[i], NULL, directory_func,
							(void *) col), argv[0], CREATE_THREAD_ERR);

			//15 threads to check left diagonal
		} else if (i >= 16 && i < 31) {
			initialization(l_diagonal, LEFT_DIAG, fix_chess, i - 16);
			check_error(
					pthread_create(&threads[i], NULL, directory_func,
							(void *) l_diagonal), argv[0], CREATE_THREAD_ERR);

			//15 threads to check right diagonal
		} else {
			initialization(r_diagonal, RIGHT_DIAG, fix_chess, i - 31);
			check_error(
					pthread_create(&threads[i], NULL, directory_func,
							(void *) r_diagonal), argv[0], CREATE_THREAD_ERR);
		}
	}
	//Wait for all the threads to finish
	for (j = 0; j < NUM_OF_THREADS; j++) {
		pthread_join(threads[j], NULL);
	}
	//Computing the result
	for (l = 0; l < NUM_OF_THREADS; l++) {
		result &= res[l];
	}
	printf(result ? "Solution is legal\n" : "Solution is illegal\n");

	free(row);
	free(col);
	free(l_diagonal);
	free(r_diagonal);
	return 1;
}

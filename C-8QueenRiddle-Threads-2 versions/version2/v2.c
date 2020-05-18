/*
 * v2.c
 *
 *  Created on: 6 Jan 2020
 *       Author: Ido Zada (203531058) & Guy Rinsky (205701907)
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>

#include "ErrorManagementV2.h"
#include "ThreadsHandlingV2.h"

int result = 0;
task** tasks;
pthread_mutex_t tasks_lock, result_lock;
pthread_cond_t cond;
const char* name;
int idx = 0;

//Initializing all the tasks in the data structure with variables
void initialization(task** tasks, char* chess) {
	int i;
	for (i = 0; i < NUM_OF_TASKS; i++) {
		tasks[i] = (task*) malloc(sizeof(task));
		tasks[i]->chess = chess;
		tasks[i]->done = 0;
		if (i <= 7) {
			tasks[i]->task_type = 0;
			tasks[i]->offset = i;
		} else if (8 <= i && i <= 15) {
			tasks[i]->task_type = 1;
			tasks[i]->offset = i - 8;
		} else if (16 <= i && i <= 30) {
			tasks[i]->task_type = 2;
			tasks[i]->offset = i - 16;
		} else {
			tasks[i]->task_type = 3;
			tasks[i]->offset = i - 31;
		}
	}
}



//Sends each thread to its designated function
void* directory_func(void* args) {
	int res, index;
	task* task;
	while (1) {
		check_error(pthread_mutex_lock(&tasks_lock),name,MUTEX_LOCK_ERR);
		index = idx;
		if (index >= NUM_OF_TASKS) {
			check_error(pthread_cond_signal(&cond), name,COND_ERROR);
			check_error(pthread_mutex_unlock(&tasks_lock), name,UNLOCK_ERR);
			break;
		}
		task = tasks[idx++];
		check_error(pthread_mutex_unlock(&tasks_lock), name,UNLOCK_ERR);
		res = chess_is_legal(task->chess,task->task_type,task->offset);
		check_error(pthread_mutex_lock(&result_lock), name,MUTEX_LOCK_ERR);
		result += res;
		check_error(pthread_mutex_unlock(&result_lock), name,UNLOCK_ERR);
		if (!res || ++index >= NUM_OF_TASKS) {
			check_error(pthread_cond_signal(&cond), name,COND_ERROR);
			check_error(pthread_mutex_lock(&tasks_lock), name,MUTEX_LOCK_ERR);
			idx = NUM_OF_TASKS;
			check_error(pthread_mutex_unlock(&tasks_lock), name,UNLOCK_ERR);
			break;
		}
	}
	return NULL;
}

int main(int argc, const char* argv[]) {
	char chess[NUMBER_OF_CHARS], fix_chess[NUMBER_OF_FIX_CHARS];
	int std_in = -1; //File descriptor
	int i, j;
	pthread_t threads[N_THREADS];
	name = argv[0];
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
	//Allocating and initializing the array of global tasks
	check_error(
			((tasks = (task**) malloc(sizeof(task*) * NUM_OF_TASKS)) == NULL) ? -1 : 1,
			argv[0], ALLOC_ERR);
	initialization(tasks, fix_chess);
	check_error(pthread_mutex_init(&tasks_lock, NULL), argv[0], MUTEX_INIT_ERR);
	check_error(pthread_mutex_init(&result_lock, NULL), argv[0],
	MUTEX_INIT_ERR);
	check_error(pthread_cond_init(&cond, NULL), argv[0], COND_INIT_ERR);
	for (i = 0; i < N_THREADS; i++) {
		check_error(pthread_create(&threads[i], NULL, directory_func, NULL),
				argv[0], CREATE_THREAD_ERR);

	}

	check_error(pthread_cond_wait(&cond, &result_lock), argv[0],COND_ERROR);


	printf((result == NUM_OF_TASKS) ? "Solution is legal!\n" : "Solution is illegal!\n");

	for (j = 0; j < N_THREADS; j++) {
		check_error(pthread_join(threads[j],NULL),argv[0],JOIN_THREAD_ERR);
	}

	freeAllocation(tasks);



	return 1;
}

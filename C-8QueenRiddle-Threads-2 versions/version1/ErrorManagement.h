/*
 * ErrorManagement.h
 *
 *  Created on: 6 Jan 2020
 *      Author: Ido Zada (203531058) & Guy Rinsky (205701907)
 */

#ifndef ERRORMANAGEMENT_H_
#define ERRORMANAGEMENT_H_

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>



#define _ERROR_MSG "%s: %s Error! errono: %d\n"

#define CREATE_THREAD_ERR	0
#define JOIN_THREAD_ERR		1
#define OPEN_ERR			2
#define CLOSE_ERR			3
#define READ_ERR			4
#define WRITE_ERR			5
#define FILE_N_F_ERR		6
#define BOARD_ERR			7

void check_error(int err, const char* name, char reason){
	const char* MSG[] = {
			"Create_thread_err",
			"Join_thread_err",
			"Open",
			"Close",
			"Read",
			"Write",
			"File Not Found",
			"Inappropriate Board --> Terminate"
	};
	if(err >= 0)
		return;
	fprintf(stderr,_ERROR_MSG, name, MSG[(int)reason], errno);
	exit(err);
}


#endif /* ERRORMANAGEMENT_H_ */

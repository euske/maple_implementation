#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#include "trigger.h"

int exec_order[] = {1001, 2001, 1002, 2002};
int exec_length = sizeof(exec_order)/sizeof(exec_order[0]);

typedef struct
{	
	int score;
}student;

void *
func1 (void *std)
{
	inst_begin(1001);
	if (((student *)std)->score >= 60){
		inst_end(1001);
		inst_begin(1002);
		if (((student *)std)->score < 60){
			crash();
		}
		inst_end(1002);
	}
	return NULL;
}

void *
func2 (void *std)
{
	inst_begin(2001);
	((student *)std)->score = 30;
	//std = NULL;
	inst_end(2001);
	inst_begin(2002);
	((student *)std)->score = 80;
	inst_end(2002);
	return NULL;
}

int
main()
{
	student taro = {80};

	student *student1 = &taro;

	inst_initialize(exec_order, exec_length);

	pthread_t t1;
	pthread_t t2;

	pthread_create(&t1, NULL, func1, (void *)student1);
	pthread_create(&t2, NULL, func2, (void *)student1);
	
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);

	inst_uninitialize();

	exit(0);
}

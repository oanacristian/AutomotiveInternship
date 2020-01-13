/*
 * tasks.h
 *
 *  Created on: Dec 17, 2019
 *      Author: egon.osvath
 */

#ifndef TASKS_H_
#define TASKS_H_

typedef struct Task
{
	unsigned long long start_clock;
	unsigned long long timeout;
	void (*task_function_pointer)(void);

    struct Task *next;

}Task;

void init_task(Task* task, unsigned long long start_clock, unsigned long long timeout, void (*task_functions_address)());

void set_task_start_clock(Task* task, unsigned long long start_clock);
void set_task_timeout(Task* task, unsigned long long  timeout);
void set_task_function_pointer(Task* task, void (*task_functions_address)());

unsigned char is_task_ready(Task*, unsigned long long);

void execute_task(Task*);

void set_next(Task*, Task*);
void add_task(Task* head, Task* new_task);
void remove_task(Task*,Task*);


#endif /* TASKS_H_ */

/*
 * tasks.c
 *
 *  Created on: Dec 17, 2019
 *      Author: egon.osvath
 */

#include "tasks.h"
#include "null.h"

void init_task(Task* task, unsigned long long start_clock, unsigned long long timeout, void (*task_functions_address)())
{
	task->start_clock = start_clock;
	task->timeout = timeout;
	task->task_function_pointer = task_functions_address;
	task->next = NULL;
}

unsigned char is_task_ready(Task* task, unsigned long long timer)
{
	return task->start_clock + task->timeout > timer?0:1;
}

void set_task_start_clock(Task* task, unsigned long long start_clock)
{
	task->start_clock = start_clock;
}

void set_task_timeout(Task* task, unsigned long long timeout)
{
	task->timeout = timeout;
}

void set_task_function_pointer(Task* task, void (*task_functions_address)())
{
	task->task_function_pointer = task_functions_address;
}


void execute_task(Task* task)
{
	task->task_function_pointer();
	task->start_clock=task->start_clock+task->timeout;
}

//List operations

void add_task(Task* head,Task* new_task)
{
	Task* iterator = head;
	if(head!=NULL)
	{
		while(iterator->next)
		{
			iterator=iterator->next;
		}
		iterator->next = new_task;
		new_task->next = NULL;
	}
	else
	{
		head = new_task;
	}
}

void set_next(struct Task* task, struct Task* next_task)
{
	task->next=next_task;
}

void remove_similar_tasks(Task* head,  void (*task_functions_address)())
{
	if(head->task_function_pointer==task_functions_address)
	{
		head = head->next;
		return;
	}
	Task* iterator = head;
	while(iterator->next)
	{
		if(iterator->next->task_function_pointer==task_functions_address)
		{
			iterator->next = iterator->next->next;
		}
		else
		{
			iterator=iterator->next;
		}
	}
}


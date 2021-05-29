#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ucontext.h>

#define STACK_SIZE 8192

tcb_t *thread_queue = 0;
tcb_t *curr_thread = 0;
int g_tid = 1;

// addQueue(head, item) adds a queue item, pointed to by “item”, to the queue pointed to by head.
void addQueue(tcb_t **head, tcb_t *item) {
	// your code
}

// delQueue(head) deletes an item from head and returns a pointer to the deleted item. If the queue is already empty, flag error.
tcb_t *delQueue(tcb_t **head) {
	// your code
}

typedef struct tcb_t {
	struct tcb_t *next;
	struct tcb_t *prev;
	ucontext_t   context;
	int threadid;
}tcb_t;

void init_TCB (tcb_t *tcb, void *function, void *stackP, int stack_size) {
	memset(tcb,'\0', sizeof(tcb_t));
	getcontext(&tcb->context);
	tcb->context.uc_stack.ss_sp = stackP;
	tcb->context.uc_stack.ss_size = (size_t) stack_size;
	makecontext(&tcb->context,function,0);
}


void create_thread(void (*function)(void)) {
	void *stack = (void *)malloc(STACK_SIZE);
	tcb_t *tcb = (tcb_t *)malloc(sizeof(tcb_t));
	init_TCB(tcb, function, stack, STACK_SIZE);
	tcb->threadid = g_tid++;
	addQueue(&thread_queue, tcb);
}

void run() {
	curr_thread = delQueue(&thread_queue);
	ucontext_t parent; 
	getcontext(&parent); // magic sauce
	swapcontext(&parent, &(curr_thread->context)); // start the first thread
}

void yield() {  
	tcb_t *prev_thread;
	addQueue(&thread_queue, curr_thread); 
	prev_thread = curr_thread;
	curr_thread = delQueue(&thread_queue);
	//swap the context, from prev_thread to the thread pointed to curr_thread
	swapcontext(&(prev_thread->context), &(curr_thread->context));
}

int get_thread_id() {
	return curr_thread->threadid;
}

/* Semaphore Implementation */

typedef struct semaphore_t {
	int counter;
	tcb_t *sem_q;
} semaphore_t;


// mallocs a semaphore structure, initializes it to the value and returns the pointer to the semaphore.
semaphore_t sem_create(int value) {

}

// decrements the semaphore. 
// if the value is <= 0, then blocks the thread in the queue associated with the semaphore.
void P(semaphore_t *sem) {
	sem->counter--;
	// your code
}

// increments the semaphore.
// if the value => 1, then takes a PCB out of the semaphore queue and puts it into the thread_queue. 
// Note: The V routine also "yields" to the next runnable thread.
void V(semaphore_t *sem) {
	sem->counter++;
	// your code
	yield();
}

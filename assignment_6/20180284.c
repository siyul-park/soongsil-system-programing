#include "20180284.h"

// An example of a semphore for the mutual exclusion
semaphore_t mutex;

void reader(void)
{ 
	// your code
}

void writer(void)
{
	// your code
}

int main(int argc, char *argv[])
{
	// example of initializing a semaphore
    mutex = sem_create(1);

    // your code

    // create threads
    create_thread(reader);
    create_thread(reader);
    create_thread(reader);
    create_thread(writer);
    
    // Let's run
    run();
    
    // stupid while loop
    while (1) {
    	sleep(1);
    }
}   





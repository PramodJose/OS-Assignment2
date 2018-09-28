/*	You are encouraged to understand the code and make modifications so that your code doesn't end up looking the same as others.
**	Some modifications that you can make are:-
**	1) Some of the functions like pthread_create() returns a non-zero value if it is unable to create a new thread. Add code that checks
**	   for the return values from functions and if there is an error, display an error message on screen.
**	2) This program can also be done using a mutex (pthread_mutex_t). Try rewriting your code with a mutex instead of a binary semaphore.
**	3) Try re-writing the code using more than one semaphore. I haven't given it much thought, but I think this can be done using 3 mutexes
**	   or semaphores or a combination of them.
**	NOTE: This is a bare-bones structure and should not be considered as the final code. Please make changes otherwise your code may end up being the same as others.
*/

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

int sharedVar = 0;					// This is the variable we will share between the two threads.

sem_t read;							// Used to block the new thread if user input has not been taken yet. Initially set to 0 so that the new thread cannot perform a DOWN operation.
									// The main thread will do an UP on "read" after writing to the shared variable. Only after that can the new thread
									// read the shared variable. This also ensures that the new thread will only read the shared variable after it is written to.

void* thread(void* args)			// This is the what the new thread will execute.
{
	sem_wait(&read);				// This is a DOWN operation. When read is 0, the thread is blocked, because it won't be able to perform this DOWN operation.
									// In other words, if the new thread gets control even before the main thread can take user input, it gets blocked.
									// It can only proceed after the main thread performs an UP operation on "read".

	printf("You entered %d\n", sharedVar);	//Display what the user entered.

	return ((void*) 0);
}

void main()							// This is the main thread.
{
	pthread_t pid;


	sem_init(&read, 0, 0);			// Here, we are setting the initial value of the semaphore "read".
									// The second argument, pshared, is set to 0 because we are sharing the semaphore between threads of the same process.
									// If we want to share the semaphore among two or more processes, we should make the second argument as 1 (or any non-zero value).
									// If we make pshared as 0, it means we are sharing the semaphore among threads of a process.
									// The last argument is the initial value of the semaphore. It is set to 0 so that the new thread cannot read the shared variable before the main thread writes to it.

	pthread_create(&pid, NULL, thread, NULL);	// Create a new thread.

	printf("Enter a number: ");
	scanf("%d", &sharedVar);		// Take user input.

	sem_post(&read);				// This is an UP operation. "read" now becomes 1. Now the new thread can proceed. Note that we have taken the user input by now.

	pthread_join(pid, NULL);		// Wait for the new thread to finish.

	sem_destroy(&read);				// Destroy the semaphore.
}
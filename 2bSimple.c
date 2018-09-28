/*	You are encouraged to understand the code and make modifications so that your code doesn't end up looking the same as others.
**	Some modifications that you can make are:-
**
**	1) Instead of taking an integer as input, take a string as input (or maybe a float). The shared variable would be an array of characters (or float) in that case.
**	2) Functions like pthread_create(), sem_init(), sem_destroy(), sem_wait(), sem_post(), return a non-zero value if an error occurs. Add code that checks
**	   for the return values from such functions and if there is an error, display an error message on screen. 
**	3) This program can also be done using a mutex (pthread_mutex_t). Try rewriting your code with a mutex instead of a binary semaphore.
**	4) Try re-writing the code using more than one semaphore/mutex. I haven't given it much thought, but I think this can be done using 3 mutexes or 3 semaphores 
**	   or a combination of mutexes and semaphores.
**	NOTE: This is a bare-bones structure and should not be considered as the final code. Please make changes otherwise your code may end up being the same as others.
*/

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

int sharedVar = 0;					// This is the variable we will share between the two threads.

sem_t read;							// Used to block the new thread if user input has not been taken yet. Initially set to 0 so that the new thread cannot perform a DOWN operation.
									// The main thread will do an UP on "read" after writing to the shared variable. Only after that can the new thread
									// read the shared variable. This also ensures that the new thread will only read the shared variable after it is written to by the main thread.

void* thread(void* args)			// This is the what the new thread will execute.
{
	sem_wait(&read);				// This is a DOWN operation. When read is 0, the new thread is blocked, because it won't be able to perform this DOWN operation.
									// DOWN operation is like an infinite loop. It continuously checks whether "read" is 0 or not. If it is 0, it stays in that infinite loop, else it proceeds to the next statement.
									// When the main thread (or any other thread) performs an UP operation on "read", the value of "read" changes and becomes non-zero, thus breaking
									// this infinite loop and the new thread can move to the next statement.
									// In other words, if the new thread gets control even before the main thread can take user input, it gets blocked in this infinite loop.
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

	sem_destroy(&read);				// Destroy the binary semaphore.
}
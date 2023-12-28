#ifndef _THREAD_SAFE
	#define _THREAD_SAFE
#endif
#ifndef _POSIX_C_SOURCE
	#define _POSIX_C_SOURCE 200112L
#endif

#include "printerror.h"

#include <unistd.h> 
#include <stdlib.h> 
#include <stdio.h> 
#include <stdint.h>
#include <inttypes.h>
#include <pthread.h> 

#include "DBGpthread.h"

pthread_mutex_t mutex;

int numExecThread = 1;

#define MAX 5

void *Thread (void *arg) 
{ 
    pthread_t th;
	intptr_t indice=(intptr_t) arg;
    int rc, indiceFiglio;

    printf("Thread %ld \n", indice);
    printf("Num Thread: %d \n", numExecThread);

    sleep(1);

    DBGpthread_mutex_lock(&mutex, "Prendo Mutex");

    while (numExecThread<=5) {
        numExecThread++;
        indiceFiglio=numExecThread;
        rc = pthread_create(&th, NULL, Thread, (void*) &indiceFiglio);
        if ( rc ) PrintERROR_andExit(rc, "Thread Creation Error");
    }
    numExecThread--;

    DBGpthread_mutex_unlock(&mutex, "Lascio Mutex");

    printf("Thread %ld Muore \n", indice);

	pthread_exit(NULL); 
}

int main ( int argc, char* argv[] ) 
{ 
	pthread_t th; 
	int  rc;
	intptr_t i;

	/* inizializzazione variabili globali */
	rc = pthread_mutex_init(&mutex, NULL); 
	if( rc ) PrintERROR_andExit(rc,"pthread_mutex_init failed");

	rc=pthread_create(&th,NULL,Thread, (void*) &i); 
	if(rc) PrintERROR_andExit(rc,"pthread_create failed");
	

	pthread_exit(NULL);
	return(0); 
} 
  

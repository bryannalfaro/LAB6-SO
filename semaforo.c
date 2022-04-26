/*
Universidad del Valle de Guatemala
LABORATORIO 6
Integrantes:
Diego Arredondo
Bryann Alfaro
Raul Jimenez
Donaldo Garcia

Referencia:
https://stackoverflow.com/questions/11160763/create-a-dynamic-number-of-threads
https://www.delftstack.com/es/howto/c/pthread-get-thread-id-in-c/
https://www.geeksforgeeks.org/use-posix-semaphores-c/
*/
#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>
#include<unistd.h>
#define NUM_THREADS 5
#define NUM_ITERATION 2
int RESOURCES =3 ;

sem_t semaphore;

void *thread(void *arg){
    printf("INICIANDO THREAD %d\n", gettid());
    printf("%d- SEMAFORO ABIERTO CON EXITO\n",gettid());
    int j;
    for(j=0; j< NUM_ITERATION; j++){
        printf("%d- INICIANDO ITERACION %d\n",gettid(), j);
        sem_wait(&semaphore);

        RESOURCES= RESOURCES -1;
        printf("%d- RECURSO TOMADO\n",gettid());
        sleep(2);
        printf("%d- RECURSO USADO\n",gettid());
        RESOURCES= RESOURCES +1;
        sem_post(&semaphore);
        printf("%d- RECURSO DEVUELTO\n",gettid());
    }
    
}

int main(){
    sem_init(&semaphore, 0 , 1);
    int i;
    printf("INICIANDO PROGRAMA\n");
    printf("CREANDO POOL DE THREADS\n");
    pthread_t pool_threads[NUM_THREADS];
    for(i=0; i<NUM_THREADS; i++){
        pthread_create(&pool_threads[i], NULL, &thread, NULL);
    }
    sleep(0.5);
    printf("ESPERANDO THREADS\n");

    for(i=0; i<NUM_THREADS; i++){
        pthread_join(pool_threads[i], NULL);
    }
    sleep(2);
    printf("value %d\n", RESOURCES);
    sem_destroy(&semaphore);
}

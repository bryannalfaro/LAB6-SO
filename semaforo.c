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
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <string.h>

#define NUM_THREADS 5
#define NUM_ITERATION 2
int RESOURCES =3 ;

sem_t semaphore;

struct resource {
    FILE* data;
};

void *thread(void *arg){
    int tid = gettid();
    int j;

    // OBTENEMOS LOS DATOS DEL FILE ENVIADO POR LOS PARAMETROS
    FILE *data= (FILE*)arg;

    printf("INICIANDO THREAD %d\n", tid);
    printf("%d- SEMAFORO ABIERTO CON EXITO\n",tid);

    // IMPRIMOS EL RESULTADO EN EL FILE
    fprintf(data, "Iniciando thread %d\n", tid);
    fprintf(data, "Esperando thread\n");
    fprintf(data, "%d- semafoto abierto con exito\n",tid);

    for(j=0; j< NUM_ITERATION; j++){
        printf("%d- INICIANDO ITERACION %d\n",tid, j);
        fprintf(data, "%d- Iniciando iteracion %d\n",tid, j);
        sem_wait(&semaphore);

        RESOURCES= RESOURCES -1;
        printf("%d- RECURSO TOMADO\n",tid);
        fprintf(data, "%d- (!) Recurso tomado\n",tid);
        sleep(2);
        printf("%d- RECURSO USADO\n",tid);
        fprintf(data, "%d- Buenos dias! Recurso usado\n",tid);
        RESOURCES= RESOURCES +1;
        sem_post(&semaphore);
        printf("%d- RECURSO DEVUELTO\n",tid);
        fprintf(data, "%d- Recurso devuelto :)\n",tid);
    }

    fprintf(data, "%d - Semaforo muerto\n", tid);
    fprintf(data, "Terminando thread %d\n", tid);
    pthread_exit(NULL);
    
}

int main(){

    // CREATE THE FILE
    FILE* file = fopen("semaphore_log.txt", "wt");
    if(file == NULL){
        printf("ERROR CREATING FILE\n");
        return 1;
    }
    // struct resource *semaphore_log = (struct resource *)malloc(sizeof(struct resourcer));

    sem_init(&semaphore, 0 , 1);
    int i;
    printf("INICIANDO PROGRAMA\n");
    printf("CREANDO POOL DE THREADS\n");

    // ESCRIBIMOS EN EL LOG
    fprintf(file, "Iniciando programa\n");
    fprintf(file, "Creando threads\n");

    pthread_t pool_threads[NUM_THREADS];

    for(i=0; i<NUM_THREADS; i++){
        pthread_create(&pool_threads[i], NULL, &thread, (void *)file);
    }
    sleep(0.5);
    printf("ESPERANDO THREADS\n");

    for(i=0; i<NUM_THREADS; i++){
        pthread_join(pool_threads[i], NULL);
    }

    sleep(2);
    printf("value %d\n", RESOURCES);
    sem_destroy(&semaphore);

    fclose(file);
    return 0;
}

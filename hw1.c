#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <time.h> // for experiment

int temp_a, temp_c, temp_m, temp_x = 0;         // make the temporary ints parsed from each line in file a global var for ez access
int sum = 0;
int roundCounter = 0;
bool quitThread = false;
int nextRound = 0;
int currThreadNum = 0;

// need to make on heap so that it can be dynamic in size
pthread_mutex_t m[3], m2;
pthread_cond_t c[3];
int results[3];
//


typedef struct {                         // each thread is set to have 4 integer properties
    int *a;
    int *c;
    int *m;
    int *x;
    int *score;
    size_t used_a;
    size_t size_a;
    size_t used_c;
    size_t size_c;
    size_t used_m;
    size_t size_m;
    size_t used_x;
    size_t size_x;
    size_t used_score;
    size_t size_score;
} Thread;

void initThread(Thread *thready, size_t initialSize){
    thready->a = malloc(initialSize * sizeof(int));
    thready->c = malloc(initialSize * sizeof(int));
    thready->m = malloc(initialSize * sizeof(int));
    thready->x = malloc(initialSize * sizeof(int));
    thready->score = malloc(initialSize * sizeof(int));
    thready->used_a = 0;
    thready->size_a = initialSize;
    thready->used_c = 0;
    thready->size_c = initialSize;
    thready->used_m = 0;
    thready->size_m = initialSize;
    thready->used_x = 0;
    thready->size_x = initialSize;
    thready->used_score = 0;
    thready->size_score = initialSize;
}

void insertThread(Thread *thready, int new_a, int new_c, int new_m, int new_x) {
    if(thready->used_a == thready->size_a){
        thready->size_a *= 2;
        thready->a = realloc(thready->a,thready->size_a * sizeof(int));
    }
    if(thready->used_c == thready->size_c){
        thready->size_c *= 2;
        thready->c = realloc(thready->c,thready->size_c * sizeof(int));
    }
    if(thready->used_m == thready->size_m){
        thready->size_m *= 2;
        thready->m = realloc(thready->m,thready->size_m * sizeof(int));
    }
    if(thready->used_x == thready->size_x){
        thready->size_x *= 2;
        thready->x = realloc(thready->x,thready->size_x * sizeof(int));
    }
    if(thready->used_score == thready->size_score){
        thready->size_score *= 2;
        thready->score = realloc(thready->score,thready->size_score * sizeof(int));
    }
    thready->a[thready->used_a++] = new_a;
    thready->c[thready->used_c++] = new_c;
    thready->m[thready->used_m++] = new_m;
    thready->x[thready->used_x++] = new_x;
    thready->score[thready->used_score++] = 0;
}

void freeThread(Thread *thready){
    free(thready->a);
    thready->a = NULL;
    free(thready->c);
    thready->c = NULL;
    free(thready->m);
    thready->m = NULL;
    free(thready->x);
    thready->x = NULL;
    free(thready->score);
    thready->score = NULL;
    thready->used_a = thready->size_a = 0;
    thready->used_c = thready->size_c = 0;
    thready->used_m = thready->size_m = 0;
    thready->used_x = thready->size_x = 0;
    thready->used_score = thready->size_score = 0;
}
Thread threads;
int numThreads;                                         // number of threads to be created
int numRounds;                                        // number of rounds to go through


// Function provided to generate random numbers
int f1(int x, int a, int c, int m) {
    long int x1 = x * a + c;
    return (int)(x1 % m);
}

// Below is snippet from class
void *runner(void *param) {             // The thread will execute in this function (changed from void)
//    int i, upper = atoi(param);
//    sum = 0;
//
//
//
//    for(i = 1; i <= upper; i++) {
//        sum += i;
//    }
//    //
//    Thread *curr_thread = param;
//    printf("threadView: %d %d %d %d\n", curr_thread->x[roundCounter],curr_thread->a[roundCounter],curr_thread->c[roundCounter],curr_thread->m[roundCounter]);
//    sum = f1(curr_thread->x[roundCounter],curr_thread->a[roundCounter],curr_thread->c[roundCounter],curr_thread->m[roundCounter]);
//    //
    int q = *((int *) param);
    printf("Child %d : Thread created\n", q);
    int internalCounter = roundCounter;
    int currSum = 0;
    while(quitThread == false){
        if(internalCounter == roundCounter && nextRound <= numThreads) {
            currSum = f1(threads.x[q], threads.a[q], threads.c[q], threads.m[q]);
            printf("threadView %d, %d, %d, %d: %d %d %d %d\n", q, roundCounter, nextRound, currSum, threads.x[q], threads.a[q],threads.c[q], threads.m[q]);
            threads.x[q] = currSum;
            nextRound++;
        }
        internalCounter++;
    }
    pthread_exit(0);
}

// runner function
void *multiThreaderRunner(void *param) {
    int x = ((Thread*)param)->x[currThreadNum];
    int a = ((Thread*)param)->a[currThreadNum];
    int c = ((Thread*)param)->c[currThreadNum];
    int m = ((Thread*)param)->m[currThreadNum];


    sum = f1(x, a, c, m);
    printf("\nsummy: %d\n", sum);

    return NULL;
}

// We assume the input file path to be accessed via argv[1]
int main(int argc, char **argv) {
    printf("Name of Input File: %s\n", argv[1]);        // "when program starts, you should print out name of file"
    /// File Parsing Starts Here
        FILE *in_file = fopen(argv[1], "r");
        struct stat sb;
        stat(argv[1], &sb);
        char *file_contents = malloc(sb.st_size);
        int counter = 0;
        while (fscanf(in_file, "%[^\n] ", file_contents) != EOF) {
            if(counter == 0){
                numThreads = atoi(file_contents);
                initThread(&threads, numThreads);
            }
            else if(counter == 1){numRounds = atoi(file_contents);}
            else if (counter > 1){
                printf("Line items: %s\n", file_contents);
                sscanf(file_contents,"%d %d %d %d", &temp_a, &temp_c, &temp_m, &temp_x);
                insertThread(&threads, temp_a, temp_c, temp_m, temp_x);
            }
            counter++;
        }
        fclose(in_file);
    /// File Parsing End
    // threads, numThreads, and numRounds are now populated

    /// Thread Experiment Start
//        pthread_t tid;                  // this is the thread identifier
//        pthread_attr_t attr;            // set of thread attributes
//
//        pthread_attr_init(&attr);                           // setting the default attributes of the thread
//        pthread_create(&tid, &attr, runner, argv[1]);       // create the thread
//        pthread_join(tid, NULL);                        // wait for thread to exit
//
//        printf("sum = %d\n", sum);
    /// End of Thread Experiment



    // Create all threads (each assigned number starting from 0)
    pthread_t threader[numThreads];
    pthread_attr_t attribute;
    pthread_attr_init(&attribute);

    for(int k = 0; k < numThreads; k++){
        int *x = (int *)malloc(sizeof(int));
        *x = k;
        pthread_create(&(threader[k]), &attribute, runner, (void *) x);
        printf("Parent: Created thread with ID: %d\n", threader[k]);
    }

    // Run through Rounds
    for(int j = 0; j < numRounds; j++){
        printf("Main process start round %d\n", j+1);
        while(nextRound != numThreads){
            // wait
        }
        // for all threads get x
        // if x is largest or smallest (no tie), increment thread score
        int smallestThread;
        int largestThread;
        for(int l = 0; l < numThreads; l++){
            // update some global var here to send message to threads
        }
        nextRound = 0;
        roundCounter++;

    }
    quitThread = true;

    for (int m = 0; m < numThreads; m++){
        pthread_join(threader[m], NULL);
        printf("Parent: Joined thread %d\n", threader[m]);
    }

    printf("Got Here! %s %d %d %d", argv[1], numThreads, numRounds, threads.a[2]);
    freeThread(&threads);
    pthread_exit(NULL);
    return 0;
}
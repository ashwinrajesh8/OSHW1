#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <unistd.h>

/*
 * Ashwin Rajesh, Operating Systems, Program 1
 * File created: 2/9/2022
 * Last edit: 2/15/2022
 * Pass the full path to the desired input file as program arguments
 * More information can be found via https://github.com/ashwinrajesh8/OSHW1 (currently private)
*/

// Global vars:
int temp_a, temp_c, temp_m, temp_x = 0;         // make the temporary ints parsed from each line in file a global var for ez access
int roundCounter = 0;
bool quitThread = false;
int nextRound = 0;

typedef struct {                         // each thread is set to have 4 integer properties + score weight
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

// More global vars:
Thread threads;
int numThreads;                                         // number of threads to be created
int numRounds;                                        // number of rounds to go through

// Function provided to generate random numbers
int f1(int x, int a, int c, int m) {
    long int x1 = x * a + c;
    return (int)(x1 % m);
}

void *runner(void *param) {             // The thread will execute in this function
    int q = *((int *) param);
    int internalCounter = roundCounter;
    int currSum = 0;
    while(quitThread == false){
        if(internalCounter == roundCounter && nextRound <= numThreads) {
            currSum = f1(threads.x[q], threads.a[q], threads.c[q], threads.m[q]);
            //Test print: printf("threadView %d, %d, %d, %d: %d %d %d %d\n", q, roundCounter, nextRound, currSum, threads.x[q], threads.a[q],threads.c[q], threads.m[q]);
            threads.x[q] = currSum;
            printf("Thread %d call f1() returns %d\n", q, currSum);
            nextRound++;
        }
        internalCounter++;
    }
    pthread_exit(0);
}

// We assume the input file path to be accessed via argv[1]
int main(int argc, char **argv) {
    printf("Name of Input File: %s\n\n", argv[1]);        // "when program starts, you should print out name of file"
    /// File Parsing Starts Here
        FILE *in_file = fopen(argv[1], "r");
        struct stat sb;
        stat(argv[1], &sb);
        char *file_contents = malloc(sb.st_size);
        int counter = 0;
        while (fscanf(in_file, "%[^\n] ", file_contents) != EOF) {
            if(counter == 0){
                sscanf(file_contents, "%d %d", &numThreads, &numRounds);
                initThread(&threads, numThreads);
            }
            else if (counter > 0){
                //print test: printf("Line items: %s\n", file_contents);
                sscanf(file_contents,"%d %d %d %d", &temp_a, &temp_c, &temp_m, &temp_x);
                insertThread(&threads, temp_a, temp_c, temp_m, temp_x);
            }
            counter++;
        }
        fclose(in_file);
    /// File Parsing End
    // threads, numThreads, and numRounds are now populated

    /// Multi threading starts here
        pthread_t threader[numThreads];
        pthread_attr_t attribute;
        pthread_attr_init(&attribute);

        // Create & Initialize all threads (each assigned number starting from 0)
        for(int k = 0; k < numThreads; k++){
            int *x = (int *)malloc(sizeof(int));
            *x = k;
            pthread_create(&(threader[k]), &attribute, runner, (void *) x);
            //print test: printf("Parent: Created thread with ID: %d\n", threader[k]);
        }

        // Run through Rounds
        for(int j = 0; j < numRounds; j++){
            printf("\nMain process start round %d\n", j+1);
            sleep(1);                                                   // added in order to ensure printing order (since thread results may print faster than the above print)
            while(nextRound != numThreads){
                // wait for all threads to generate f1()
            }
            int smallestThread = threads.x[0];
            int largestThread = threads.x[0];
            int currMax = INT_MIN;
            int currMin = INT_MAX;
            // find min & max:
            for(int l = 0; l < numThreads; l++){
                if(threads.x[l] == currMax){
                    largestThread = INT_MAX;
                }
                if(threads.x[l] > currMax){
                    largestThread = l;
                    currMax = threads.x[l];
                }
                if(threads.x[l] == currMin){
                    smallestThread = INT_MIN;
                }
                if(threads.x[l] < currMin){
                    smallestThread = l;
                    currMin = threads.x[l];
                }
            }
            printf("Round %d finished. Winners are ", roundCounter);
            if(smallestThread != INT_MIN){
                threads.score[smallestThread]++;
                printf("%d ", smallestThread);
            }
            if(largestThread != INT_MAX){
                threads.score[largestThread]++;
                printf("%d ", largestThread);
            }
            printf("\n");
            nextRound = 0;
            roundCounter++;
        }
        quitThread = true;

        // Output thread scores & overall winner
        int maxScore = 0;
        printf("\n");
        for (int n = 0; n < numThreads; n++){
            if(threads.score[n] > maxScore){
                maxScore = threads.score[n];
            }
            printf("Score for thread %d: %d\n", n, threads.score[n]);
            pthread_join(threader[n], NULL);
        }
        printf("\nOverall winner: ");
        for(int p = 0; p < numThreads; p++){
            if(threads.score[p] == maxScore) {
                printf("%d ", p);
            }
        }
        printf("\n");

        freeThread(&threads);
        pthread_exit(NULL);
    /// Multi threading ends
    return 0;
}
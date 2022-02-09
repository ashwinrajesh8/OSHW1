#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>

struct Thread {
    int a;
    int c;
    int m;
    int x;
};

// Function provided to generate random numbers
int f1(int x, int a, int c, int m) {
    long int x1 = x * a + c;
    return (int)(x1 % m);
}

// Main
// We assume the input file path to be accessed via argv[1]
int main(int argc, char **argv) {
    printf("Name of Input File: %s\n", argv[1]);        // "when program starts, you should print out name of file"

    struct Thread *threads;

    int numThreads;                                         // number of threads to be created
    int numRounds;                                        // number of rounds to go through

    FILE *in_file = fopen(argv[1], "r");
    struct stat sb;
    stat(argv[1], &sb);
    char *file_contents = malloc(sb.st_size);
    int counter = 0;
    while (fscanf(in_file, "%[^\n] ", file_contents) != EOF) {
        if(counter == 0){numThreads = atoi(file_contents);}
        else if(counter == 1){numRounds = atoi(file_contents);}
        else if (counter > 1){
            printf("Line items: %s\n", file_contents);
            int temp_a;
            int temp_c;
            int temp_m;
            int temp_x;
            char *num_store = malloc(sb.st_size);
            int numCounter = 0;
//            while (scanf(file_contents, " ", num_store)){
//                if(numCounter == 0){
//                    temp_a = atoi(num_store);
//                }
//                else if(numCounter == 1){
//                    temp_c = atoi(num_store);
//                }
//                else if(numCounter == 2){
//                    temp_m = atoi(num_store);
//                }
//                else if(numCounter == 3){
//                    temp_x = atoi(num_store);
//                }
//                numCounter++;
//            }
            // move below into while loop
//            for(int i = 0; i < 4; i++){
//                threads[counter-2].a = temp_a;
//            }
        }
        counter++;
    }
    fclose(in_file);


    printf("Got Here! %s %d %d", argv[1], numThreads, numRounds);
    return 0;
}
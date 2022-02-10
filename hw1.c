#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <pthread.h>

int temp_a, temp_c, temp_m, temp_x;         // make the temporary ints parsed from each line in file a global var for ez access

struct Thread {                         // each thread is set to have 4 integer properties
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

// We assume the input file path to be accessed via argv[1]
int main(int argc, char **argv) {
    printf("Name of Input File: %s\n", argv[1]);        // "when program starts, you should print out name of file"

    struct Thread *threads;                                   // dynamically allocating thread struct cuz we have no idea how many threads exactly there may be
    int numThreads;                                         // number of threads to be created
    int numRounds;                                        // number of rounds to go through

    /// File Parsing Starts Here
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
                sscanf(file_contents,"%d %d %d %d", &temp_a, &temp_c, &temp_m, &temp_x);
                threads[counter-2].a = temp_a;
                threads[counter-2].c = temp_c;
                threads[counter-2].m = temp_m;
                threads[counter-2].x = temp_x;
            }
            counter++;
        }
        fclose(in_file);
    /// File Parsing End
    // threads, numThreads, and numRounds are populated

    printf("Got Here! %s %d %d", argv[1], numThreads, numRounds);
    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

// Function provided to generate random numbers
int f1(int x, int a, int c, int m) {
    long int x1 = x * a + c;
    return (int)(x1 % m);
}

// Main
// We assume the input file path to be accessed via argv[1]
int main(int argc, char **argv) {
    int numThreads = 3;             // number of threads to be created
    int numRounds = 5;              // number of rounds to go through

    // Test that file is openable
//    FILE *fptr;
//    if ((fptr = fopen(argv[1],"r")) == NULL){
//        printf("Error! opening file");
//        exit(1);
//    }


    FILE *in_file = fopen(argv[1], "r");
    struct stat sb;
    stat(argv[1], &sb);
    char *file_contents = malloc(sb.st_size);

    while (fscanf(in_file, "%[^\n] ", file_contents) != EOF) {
        printf("> %s\n", file_contents);
    }
    fclose(in_file);


    printf("Got Here! %s", argv[1]);
    return 0;
}
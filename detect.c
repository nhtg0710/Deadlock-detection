#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include "utils.h"
#include "graph.h"

#define EXEC "-e"
#define PER_PROCESS "-c" 
#define FILE_SIGNATURE "-f"

int main(int argc, char **argv) { 
    int e_entered = 0;
    int c_entered = 0;
    char *filepath;
    int i;
    for (i = 0; i < argc; i++){
        if (strcmp(argv[i], EXEC) == 0){
            e_entered = 1;
        } 
        if (strcmp(argv[i], PER_PROCESS) == 0){
            c_entered = 1;
        }
        if (strcmp(argv[i], FILE_SIGNATURE) == 0) {
            filepath = argv[i+1];
        }
    }
    FILE* fp = fopen(filepath, "r");
    if (fp == NULL){
        exit(0);
    }
    readProblem(fp, e_entered, c_entered);
    return 0; 
}
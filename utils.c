#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "graph.h"
#include "utils.h"
#define INITIALIZER -1 //used for initialisation of arrays
#define PRESENT -1

int maxFrequencyCount(int *arr, int length){
    int *frequency = malloc(sizeof(int) * length);
    assert(frequency);
    int visited = INITIALIZER;
    for(int i = 0; i < length; i++){  
        int count = 1;  
        for(int j = i+1; j < length; j++){  
            if(arr[i] == arr[j]){  
                count++;  
                //To avoid counting same element again  
                frequency[j] = visited;  
            }  
        }  
        if(frequency[i] != visited)  
            frequency[i] = count;  
    }  
    int max = visited;
    int i;
    for (i = 0; i < length; i++){
        if (frequency[i] > max){
            max = frequency[i];
        }
    }
    return max;
}

// find the minimum value of an array
int min(int *arr, int length){
    int i; int min = arr[0];
    for (i = 0; i < length; i++){
        if (arr[i] < min){
            min = arr[i];
        }
    }
    return min;
}
// find the maximum value of an array
int max(int *arr, int length){
    int i; int max = arr[0];
    for (i = 0; i < length; i++){
        if (arr[i] > max){
            max = arr[i];
        }
    }
    return max;
}

// cycles form deadlocks so we need to detect them
int detectCycle(int* lockedFileofProcess, int *requestedFileofProcess, 
int *visited, int lengthOfArr, int lengthOfVisited, int minProcessID){
    int i; int j; int detected = 0; int initial = INITIALIZER;

    // i here represents process ID
    for (i=0; i < lengthOfArr; i++){
        if ((lockedFileofProcess[i] != -1) 
        && (requestedFileofProcess[i] != -1)){
            visited[lockedFileofProcess[i]] += 1;
            initial = i;
            break;
        }
    }
    // we run 2 arrays, 1 array contains files being locked by which process,
    // and another for files being requested
    // with each process ID that isn't in the input given, both of its files
    // are set to -1, meaning this process isn't present
    for (i = initial; i < lengthOfArr; i++){
        if ((lockedFileofProcess[i] != -1) 
        && (requestedFileofProcess[i] != -1)){
            int temp = requestedFileofProcess[i];
            int found = 0;
            for (j = initial; j < lengthOfArr; j++){
                if (temp == lockedFileofProcess[j]){
                    visited[temp] += 2;
                    found = 1;
                    break;
                }
            }
            if (found == 0){
                lockedFileofProcess[i] = -1;
                if (i < lengthOfArr - 1){
                    visited[lockedFileofProcess[i+1]] += 1;
                }
                requestedFileofProcess[i] = -1;
                visited[lockedFileofProcess[i]] -= 1;
            }
            if (visited[temp] >= 2){
                detected = 1;
                break;
            }
        }
    }
    // find process to terminate if cycle is detected
    if (detected){
        int return_value = INITIALIZER; 
        int processCausingDeadlock = INITIALIZER;
        for (i = initial; i < lengthOfArr; i++){
            if (max(visited, lengthOfVisited) != INITIALIZER
                && visited[lockedFileofProcess[i]] 
                == max(visited, lengthOfVisited)){ 
                processCausingDeadlock = i;
                return_value = processCausingDeadlock;
                
                for (int g = initial; g < lengthOfArr; g++){
                    if (requestedFileofProcess[processCausingDeadlock]
                    == lockedFileofProcess[g]
                    &&visited[requestedFileofProcess[processCausingDeadlock]] 
                    == visited[lockedFileofProcess[g]]){
                        if (g < processCausingDeadlock){
                            return_value = g;
                        }
                    }
                }
                break;
            }

        }

        //remove everything from the old cycle in case there is more to come
        int loop = INITIALIZER;
        if (processCausingDeadlock > return_value){
            loop = processCausingDeadlock;
        } else {
            loop = return_value;
        }
        for (i = 0; i <= lengthOfArr; i++){
            if (i <= loop){
                lockedFileofProcess[i] = -1;
                requestedFileofProcess[i] = -1;
            }
            visited[lockedFileofProcess[i]] = -1;
            visited[requestedFileofProcess[i]] = -1;
        }
        return return_value;
    }
    return -1;
}

int presentProcessUndetected(int *arr, int length){
    for (int i = 0; i < length; i ++){
        if (arr[i] == PRESENT){
            return 1;
        }
    }
    return 0;
}
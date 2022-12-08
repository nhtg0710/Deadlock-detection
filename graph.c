#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "graph.h"
#include "utils.h"

#define INITIALEDGES 16

#define INITIAL_SIZE 8 //the minimum array size, will realloc if gets bigger
#define INITIALIZER -1 //used for initialisation of arrays
#define NOT_PRESENT -2
#define PRESENT -1
struct graphProblem{
    int numProcesses;
    int numFiles;
    int executionTime;
    int cycle_count;
};

void readProblem(FILE *file, int e_entered, int c_entered){

    struct graphProblem *problem = malloc(sizeof(struct graphProblem));
    assert(problem);
    problem->cycle_count = 0;

    int *processes = malloc(INITIAL_SIZE * sizeof(int));
    assert(processes);
    int *lockedFiles = malloc(INITIAL_SIZE * sizeof(int));
    assert(lockedFiles);
    int *requestedFiles = malloc(INITIAL_SIZE * sizeof(int));
    assert(requestedFiles);
    int count = 0;
    int maxFileID = 0;
    int *cycle = malloc(INITIAL_SIZE * sizeof(int));
    assert(cycle);

    //reading problem line by line, adding to the graph
    while (fscanf(file, "%d %d %d", &processes[count], 
    &lockedFiles[count], &requestedFiles[count]) == 3){
        problem->numProcesses += 1;
        if (lockedFiles[count] > requestedFiles[count] 
        && lockedFiles[count] > maxFileID){
            maxFileID = lockedFiles[count];
        } else if (lockedFiles[count] < requestedFiles[count] 
        && requestedFiles[count] > maxFileID){
            maxFileID = requestedFiles[count];
        } else if (lockedFiles[count] == requestedFiles[count] 
        && lockedFiles[count] > maxFileID){
            maxFileID = lockedFiles[count];
        }
        
        count += 1;
        if (count >= INITIAL_SIZE){
            processes = realloc(processes, count * 2 * sizeof(int));
            lockedFiles = realloc(lockedFiles, count * 2 * sizeof(int));
            requestedFiles = realloc(requestedFiles, count * 2 * sizeof(int));
            cycle = realloc(cycle, count * 2 * sizeof(int));
        }
        problem->numProcesses = count;
    }

    //count how many files are requested and locked, without duplication
    int *visited = malloc((maxFileID+1) * sizeof(int));
    assert(visited);
    int i;
    for (i = 0; i < maxFileID+1; i++){
        visited[i] = 0;
    }
    for (i = 0; i < problem->numProcesses; i++){
        if (visited[lockedFiles[i]] == 0){
            visited[lockedFiles[i]] = 1;
            problem->numFiles += 1;
        }
        if (visited[requestedFiles[i]] == 0){
            visited[requestedFiles[i]] = 1;
            problem->numFiles += 1;
        }
    }
    if (!c_entered){
        printf("Processes %d\n", problem->numProcesses);
        printf("Files %d\n", problem->numFiles);
    }
    
    if (e_entered == 1){
        problem->executionTime = 
        maxFrequencyCount(lockedFiles, problem->numProcesses) 
        + maxFrequencyCount(requestedFiles, problem->numProcesses);
        printf("Execution time %d\n", problem->executionTime);
    }
    
    int maxProcessID = max(processes, problem->numProcesses);
    int minProcessID = min(processes, problem->numProcesses);

    // create an array to store visted status of locked and requested files
    // length 
    int * lockedFilesOfProcess = malloc(sizeof(int) * (maxProcessID+1));
    assert(lockedFilesOfProcess);
    int * requestedFilesOfProcess = malloc(sizeof(int) * (maxProcessID+1));
    assert(requestedFilesOfProcess);
    for (i = 0; i < maxProcessID+1; i++){
        lockedFilesOfProcess[i] = INITIALIZER;
        requestedFilesOfProcess[i] = INITIALIZER;
    }   
    for (i = 0; i < problem->numProcesses; i++){
        lockedFilesOfProcess[processes[i]] = lockedFiles[i];
        requestedFilesOfProcess[processes[i]] = requestedFiles[i];
    }

    // this is to check files are visited twice to form a cycle
    int *fileTraversalStatus = malloc(sizeof(int) * (maxFileID+1));
    assert(fileTraversalStatus);
    for (i = 0; i < maxFileID+1; i++){
        fileTraversalStatus[i] = INITIALIZER;
    }
    if (!c_entered){
        int detected = detectCycle(lockedFilesOfProcess, 
        requestedFilesOfProcess,fileTraversalStatus, 
        maxProcessID+1, maxFileID+1, minProcessID);
        if (detected != -1){
            printf("Deadlock detected\n");
            printf("Terminate %d ", detected);
            while(detected != -1){
                detected = detectCycle(lockedFilesOfProcess, 
                requestedFilesOfProcess,fileTraversalStatus, 
                maxProcessID+1, maxFileID+1, minProcessID);
                if (detected != -1){
                    printf("%d ", detected);
                }

            }
            printf("\n");

        } else {
            if (!e_entered){
                printf("No deadlocks\n");
            }
        }
    }
    if (c_entered){
        int start_time = 0;
        int simulation_time = 0;
        int *simulationTimeOfProcess = malloc(sizeof(int)*(maxProcessID+1));
        for (i = 0; i < maxProcessID+1; i++){
            simulationTimeOfProcess[i] = NOT_PRESENT;
        }
        for (i = 0; i < problem->numProcesses; i++){
            simulationTimeOfProcess[processes[i]] = PRESENT;
        }

        assert(simulationTimeOfProcess);
        int *visited = malloc(sizeof(int) * (maxFileID + 1));
        assert(visited);
        for (i = 0; i < maxFileID+1; i ++){
            visited[i] = INITIALIZER;
        }

        while(presentProcessUndetected(simulationTimeOfProcess, 
        maxProcessID + 1)){
            for (i=0; i < maxProcessID+1; i++){
                if (visited[lockedFilesOfProcess[i]] == -1 
                && visited[requestedFilesOfProcess[i]] == -1 
                && simulationTimeOfProcess[i] == PRESENT){
                    visited[lockedFilesOfProcess[i]] = 1;
                    visited[requestedFilesOfProcess[i]] = 1;
                    simulationTimeOfProcess[i] = start_time;
                }
            }
            for (i = 0; i < maxProcessID + 1; i++){
                visited[lockedFilesOfProcess[i]] = -1;
                visited[requestedFilesOfProcess[i]] = -1;
            }
            start_time += 1;
            simulation_time += 1;
        }
        for (int j = 0; j < simulation_time; j++){
            for (i = 0; i < maxProcessID + 1; i++){
                if (simulationTimeOfProcess[i] == j){
                    printf("%d %d %d,%d\n", j, i, lockedFilesOfProcess[i], 
                    requestedFilesOfProcess[i]);
                }
            }
        }
        printf("Simulation time %d\n", simulation_time);
    }
    printf("\n");
}
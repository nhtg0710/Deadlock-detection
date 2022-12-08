#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "graph.h"

struct graphProblem;

struct graphProblem *newGraphProblem();

int maxFrequencyCount(int *arr, int length);

int deadlockDetected(int *arr1, int *arr2, int length);

int min(int *arr, int length);

int max(int *arr, int length);

int detectCycle(int* lockedFileofProcess, int *requestedFileofProcess, int *visited, int lengthOfArr, int lengthOfVisited, int minProcessID);

int presentProcessUndetected(int *arr, int length);
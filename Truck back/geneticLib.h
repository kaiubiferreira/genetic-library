#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "enable.h"

struct tNode
{
    int value;
    float constant;
    struct tNode ** son;
    struct tNode * parent;
};

struct tRandomConstant
{
    int blnHas;
    int blnIsInteger; // If it is not integer, consider as float
    float startRange, endRange;
    int terminalIndex;
};

struct tProperty
{
    int populationSize;
    int generations;
    float mutationProbality;
    float reproductionProbability;
    float crossOverProbability;
    bool blnOverSelection;
    float overSelectionProportion;
    bool blnHasInputFile;

    struct tRandomConstant randomConstant; // In some problems the some terminal nodes might consist of random numbers

    int * functionSets; // Array consistent of function/terminal identifications and number of attributes (terminal will have 0 attributes)
    int functionQuantity;
    int terminalQuantity;
    float maxFitness;
    int currentNode; // Used as a global variable when finding the cross nodes


    struct tNode ** population;
    float * fitness;
    float * rawFitness;
    float * priorityTable;
};

void printGene(struct tNode*);
float rawFitness(struct tNode*, bool simulte = false);

void sortPopulation();

int selectOrganism(double, int, int); // Selects the organism with binary search

struct tNode * copyTree(struct tNode *, struct tNode *);

int returnMaxElements(struct tNode *);

struct tNode * returnCrossNode(struct tNode *, int);

void setFree(struct tNode *);

float createRandomConstant(struct tRandomConstant);

int height(struct tNode *);

void importOrganism(int *);

void exportOrganism(struct tNode *);

struct tNode * createOrganism(int, int, int, struct tNode *);

void nextGeneration();

void showOrganism(struct tNode *, FILE * output);

void firstGeneration();

void showPopulation();

void normalizedFitness(); // Adjusted Fitness = 1/(1 + maxFitness - rawFitnes(i), normalized = a(i)/sum

int startLife(struct tProperty, int *); // functionArguments represents the number of children each function can have


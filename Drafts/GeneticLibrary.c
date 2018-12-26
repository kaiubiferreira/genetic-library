#include <stdio.h>
#include <stdlib.h>
#include <time.h>


struct tProperty
{
    int populationSize;
    int generations;
    float mutationProbality;
    float reproductionProbability;
    float crossOverProbability;

    float maxFitness;

    int * functionSets; // Array consistent of function/terminal identifications and number of attributes (terminal will have 0 attributes)
    int functionQuantity;
    int terminalQuantity;
    int quantity;
    int currentNode;

    struct tNode ** population;
    float * fitness;
};

struct tNode
{
    int value;
    int order; // indicates which position the node is on the father's son array, 0, 1, 2, ... important for crossover
    struct tNode ** son; // Vector of pointers allowing the node to have up to n sons
    struct tNode * father;
};


struct tNode * generateDNA(struct tProperty * property, int maxHeight, int height, int blnFull, struct tNode * father, int order)
{
    struct tNode * node = malloc(sizeof(struct tNode));
    int i = 0;

    node->father = father;
    node->order = order;

    // Selects the Gene
    if(height == maxHeight) // The Node has to be a terminal
        node->value = rand()%property->terminalQuantity + property->functionQuantity; // The terminal sets are in the last position of the functionset matrix
    else if(blnFull == 1) // The Node has to be a function
        node->value = rand()%property->functionQuantity;
    else // The node can be anything
        node->value = rand()%property->quantity;

    node->son = malloc(property->functionSets[node->value] * sizeof(struct tNode));

    while(i < property->functionSets[node->value])
    {
        node->son[i] = generateDNA(property, maxHeight, height + 1, blnFull, node, i);
        i++;
    }

    return node;
}

int firstGeneration(struct tProperty * property)
{
    int i, maxHeight = 3;
    int blnFull = 1;

    for(i = 0; i < property->populationSize; i++, blnFull *=-1)
    {
        maxHeight = ((i*10)/property->populationSize+2)/2 + 1; // 20% with size 2, 20% with 3, and so on..
        property->population[i] = generateDNA(property, maxHeight, 0, blnFull, NULL, -1);
    }
}

float rawFitness(struct tNode *);

void normalizedFitness(struct tProperty * property) // Adjusted Fitness = 1/(1 + maxFitness - rawFitnes(i), normalized = a(i)/sum
{
    int i;
    float sum = 0, probabilitySum = 0;
    int j = 0, start = 0; // Creates the priority table

    for(i = 0; i < property->populationSize; i++)
    {
        property->fitness[i] = 1/(1+ property->maxFitness - rawFitness(property->population[i])); // Adjusted Fitness
        sum += property->fitness[i];
    }

    for(i = 0; i < property->populationSize; i++) // Normalized Fitness
    {
        probabilitySum += property->fitness[i] / sum;
        property->fitness[i] = probabilitySum;
    }
}

struct tNode * selectOrganism(struct tProperty *property, double target, int left, int right)
{
    int half = (left + right)/2;

    if(left >= right)
        return property->population[left];
    else if(property->fitness[half] < target)
        return selectOrganism(property, target, half +1, right);
    else if(property->fitness[half] > target)
        return selectOrganism(property, target, left, half - 1);
    else
        return property->population[half];
}

struct tNode * returnCrossNode(struct tProperty * property, struct tNode * node, int crossNode)
{
    int i;
    struct tNode * aux = NULL;

    if(property->currentNode == crossNode)
    {
        return node;
    }
    else if(node != NULL)
    {
        for (i = 0; i < property->functionSets[node->value]; i++)
        {
            property->currentNode++;
            if (aux == NULL)
                aux = returnCrossNode(property, node->son[i], crossNode);
        }
    }

    return aux;
}

int returnMaxElements(struct tProperty * property, struct tNode * node)
{
    int i;
    int sum = 1; // It's needed to be set as 1 in order to count every node

    if (node != NULL)
    {
        for (i = 0; i < property->functionSets[node->value]; i++)
        {
            sum += returnMaxElements(property, node->son[i]);
        }
    }
    return sum;
}

struct tNode * copyTree(struct tProperty *property, struct tNode * old, struct tNode * father)
{
    int i;
    struct tNode * newNode = malloc(sizeof(struct tNode));

    if (old == NULL)
    {
        printf("NULL2");
        getch();
    }
    newNode->value = old->value;
    newNode->father = father; // ACHEI O ERROOOOOO!!!!!!!!!!!!!!!!!! (acho), nao pode atribuir o pai, animal
    newNode->order = old->order;
    newNode->son = malloc(property->functionSets[newNode->value]*sizeof(struct tNode));

    for(i = 0; i < property->functionSets[newNode->value]; i++)
    {
        newNode->son[i] = copyTree(property, old->son[i], newNode);
    }
    return newNode;
};

void setFree(struct tProperty * property, struct tNode * node, int first)
{
    int i;

    if(node == NULL)
    {
        printf("NULL3");
        getch();
    }

    for(i = 0; i < property->functionSets[node->value]; i++)
    {
        setFree(property, node->son[i], 0);
    }

    if(!first)
    {
       // printf("1");
       free(node);
    }
}

void nextGeneration(struct tProperty * property)
{
    int i;
    struct tNode ** nextPopulation = malloc(property->populationSize*sizeof(struct tNode));
    struct tNode * father, * mother;
    struct tNode * fatherCrossOver;
    struct tNode * motherCrossOver;
    struct tNode * nodeAux;

    int aux = 0;
    int fatherMaxElements = 0;
    int motherMaxElements = 0;
    float crossPoint;

    for(i = 0; i < (int)(property->crossOverProbability * property->populationSize); i+=2)
    {
        printf("\ni%d ", i);
        father = copyTree(property, selectOrganism(property, rand()/(double)RAND_MAX, 0, property->populationSize), NULL);

        showOrganismDNA(property,father);

        nodeAux = selectOrganism(property, rand()/(double)RAND_MAX, 0, property->populationSize);
        mother = copyTree(property, nodeAux, NULL);

        if(mother == NULL || father == NULL)
        {
            printf("NULL");
            getch();
        }

        fatherMaxElements = returnMaxElements(property, father);
        motherMaxElements = returnMaxElements(property, mother);

        property->currentNode = 0;
        fatherCrossOver = returnCrossNode(property, father, rand()%fatherMaxElements);

        property->currentNode = 0;
        motherCrossOver = returnCrossNode(property, mother, rand()%motherMaxElements);

        if(motherCrossOver == NULL || fatherCrossOver == NULL)
        {
            printf("NULL5");
            getch();
        }

        aux = motherCrossOver->order;
        motherCrossOver->order = fatherCrossOver->order;
        fatherCrossOver->order = aux;

        if(motherCrossOver->father != NULL)
            motherCrossOver->father->son[fatherCrossOver->order] = fatherCrossOver;

        if(fatherCrossOver->father != NULL)
            fatherCrossOver->father->son[motherCrossOver->order] = motherCrossOver;

        nodeAux = motherCrossOver->father;
        motherCrossOver->father = fatherCrossOver->father;
        fatherCrossOver->father = nodeAux;

        nextPopulation[i] = father;
        nextPopulation[i+1] = mother;
    }

    while(i < property->populationSize)
    {
        printf("\nri%d", i);
        nextPopulation[i] = copyTree(property, selectOrganism(property, rand()/(double)RAND_MAX, 0, property->populationSize), NULL);
        i++;
    }

    // ADD MUTATION

    printf("iiiii>%d", i);
    //getch();

    for(i = 0; i < property->populationSize; i++)
    {
        setFree(property, property->population[i], 1);
        property->population[i] = nextPopulation[i];
        //showOrganismDNA(property, property->population[i]);
        //printf("\n");
    }
    //showPopulation(property);
    printf("finished once");

    //getch();
}

void startLife(struct tProperty * property)
{
    int i;
    //srand(time(NULL));
    srand(2);
    firstGeneration(property);

    normalizedFitness(property);

    for(i = 1; i < property->generations; i++)
    {
        nextGeneration(property);
        normalizedFitness(property);
    }

    //test(property);
}

void showOrganismDNA(struct tProperty * property, struct tNode * node)
{
    int i;
    if(node != NULL)
    {
        printf("%d ", node->value);

        //printf(" %p", node);
        for(i = 0; i < property->functionSets[node->value]; i++)
        {
            showOrganismDNA(property, node->son[i]);
        }
    }
}

void showPopulation(struct tProperty * property)
{
    int i;
    for(i = 0; i < property->populationSize; i++)
    {
        printf("\ni[%d]>", i);
        showOrganismDNA(property, property->population[i]);
    }
}

void test(struct tProperty * property)
{

}

int main()
{
    struct tProperty property; // Public Variable

    property.functionQuantity = 2;
    property.terminalQuantity = 2;
    property.quantity = 4;
    property.functionSets = malloc(property.quantity*sizeof(unsigned short int));
    property.populationSize = 400;
    property.generations = 3;
    property.population = malloc(property.populationSize*(sizeof(struct tNode)));
    property.fitness = malloc(property.populationSize*(sizeof(float)));

    property.maxFitness = 100.0;

    property.functionSets[0] = 1;
    property.functionSets[1] = 3;
    property.functionSets[2] = 0;
    property.functionSets[3] = 0;

    property.mutationProbality = 0.1;
    property.reproductionProbability = 0.1; // Non-sexual Reproduction
    property.crossOverProbability = 1 - (property.mutationProbality + property.reproductionProbability); // Sexual Reproduction

    //test(&property);
    startLife(&property);
    showPopulation(&property);
}

float rawFitness(struct tNode * node)
{
    return rand()%100;
}

#include <stdio.h>
#include <stdlib.h>
#include <time.h> // Used to set the srand
#include <math.h>

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
    int blnOverSelection;

    struct tRandomConstant randomConstant; // In some problems the some terminal nodes might consist of random numbers

    int * functionSets; // Array consistent of function/terminal identifications and number of attributes (terminal will have 0 attributes)
    int functionQuantity;
    int terminalQuantity;
    float maxFitness;
    int currentNode; // Used as a global variable when finding the cross nodes


    struct tNode ** population;
    float * fitness;
    float * rawFitness;
    int * priorityTable;
};

struct tProperty property;

void printGene(struct tNode*);
float rawFitness(struct tNode*);


int selectOrganism(double target, int left, int right) // Selects the organism with binary search
{

    int half = (left + right)/2;

    if(left >= right)
        return left;
    else if(property.fitness[half] < target)
        return selectOrganism(target, half +1, right);
    else if(property.fitness[half] > target)
        return selectOrganism(target, left, half - 1);
    else
        return half;
}

void generatePriorityTable()
{
    int i = 0, sum = 0;
    int j = 0, start = 0;
    int competetive1, competitive2;

    for(i = 0; i < property.populationSize; i++)
    {
        sum += property.rawFitness[i];
    }

    for(i = 0; i < property.populationSize; i++)
    {
        start = j;
        while ((j < (property.rawFitness[i]*property.populationSize/sum) + start) && j < property.populationSize)
        {
            property.priorityTable[j] = i;
            j++;
        }
    }

    while(j < property.populationSize)
    {
        competetive1 = rand()%property.populationSize;
        competitive2 = rand()%property.populationSize;
        if (property.rawFitness[competetive1] > property.rawFitness[competitive2])
        {
            property.priorityTable[j] = competetive1;
        }
        else
        {
            property.priorityTable[j] = competitive2;
        }
        j++;
    }
}

struct tNode * copyTree(struct tNode * old, struct tNode * parent)
{
    int i;
    struct tNode * newNode = (struct tNode*) malloc(sizeof(struct tNode));

    newNode->value = old->value;
    newNode->parent = parent;
    newNode->constant = old->constant;

    if(property.functionSets[newNode->value] > 0)
    {
        newNode->son = (struct tNode**) malloc(property.functionSets[newNode->value]*sizeof(struct tNode));
        for(i = 0; i < property.functionSets[newNode->value]; i++)
        {
            newNode->son[i] = copyTree(old->son[i], newNode);
        }
    }
    return newNode;
};

int returnMaxElements(struct tNode * node)
{
    int i;
    int sum = 1; // It's needed to be set as 1 in order to count every node

    if (node != NULL)
    {
        for (i = 0; i < property.functionSets[node->value]; i++)
        {
            sum += returnMaxElements(node->son[i]);
        }
    }
    return sum;
}

struct tNode * returnCrossNode(struct tNode * node, int crossNode)
{
    int i;
    struct tNode * aux = NULL;

    if(property.currentNode == crossNode)
    {
        return node;
    }
    else if(node != NULL)
    {
        for (i = 0; i < property.functionSets[node->value]; i++)
        {
            property.currentNode++;
            if (aux == NULL)
                aux = returnCrossNode(node->son[i], crossNode);
        }
    }

    return aux;
}

void setFree(struct tNode * node)
{
    int i;

    for(i = 0; i < property.functionSets[node->value]; i++)
    {
        setFree(node->son[i]);
    }

    free(node);
}

float createRandomConstant(struct tRandomConstant random)
{
    float r;
    r = rand()%(int)(random.endRange - random.startRange + 1) + random.startRange;

    if(random.blnIsInteger == 0)
        r += rand()/(double)RAND_MAX; // Adds a float (decimal) value from 0 to 1}

    if(r <= random.endRange)
        return r;
    else
        return createRandomConstant(random);
}

struct tNode * createOrganism(int maxHeight, int currentHeight, int blnFull, struct tNode * parent)
{
    struct tNode * node = (struct tNode *)malloc(sizeof(struct tNode));
    int i;

    if(currentHeight >= maxHeight)
        node->value = rand()%property.terminalQuantity + property.functionQuantity; // Selects only terminals
    else
    {
        if(blnFull == 1) // Select only function
            node->value = rand()%property.functionQuantity;
        else
            node->value = rand()%(property.terminalQuantity + property.functionQuantity); // Selects either terminals or functions
    }

    node->parent = parent;

    if(property.randomConstant.blnHas == 1 && property.randomConstant.terminalIndex == node->value)
    {
        node->constant = createRandomConstant(property.randomConstant);
    }

    if(node->value <= property.functionQuantity)
    {
        node->son = (struct tNode **)malloc(property.functionSets[node->value] * sizeof(struct tNode));

        for(i = 0; i < property.functionSets[node->value]; i++)
        {
            node->son[i] = createOrganism(maxHeight, currentHeight + 1, blnFull, node);
        }
    }

    return node;
}

int height(struct tNode * node)
{
    int heigher = 0;
    int sonHeight = 0;
    int i;

    for(i = 0; i < property.functionSets[node->value]; i++)
    {
        sonHeight = height(node->son[i]);
        if(sonHeight > heigher)
            heigher = sonHeight;
    }

    return heigher + 1;
}

void nextGeneration()
{
    int i, j;
    int indexFather, indexMother;
    int best; // Best fitness, to be used on elitism

    struct tNode ** newGeneration = (struct tNode **)malloc(property.populationSize*sizeof(struct tNode));
    struct tNode * father, * mother;
    struct tNode * fatherCrossNode, * motherCrossNode;
    struct tNode * aux;
    struct tNode * tumor, * tumorCrossNode;


    // CrossOver - Reproduction with genetic matter exchange
    for(i = 0; i <= property.populationSize * property.crossOverProbability; i+=2)
    {
        indexFather = selectOrganism(rand()/(double)RAND_MAX, 0, property.populationSize - 1);
        indexMother = selectOrganism(rand()/(double)RAND_MAX, 0, property.populationSize - 1);

        father = copyTree(property.population[indexFather], NULL);
        mother = copyTree(property.population[indexMother], NULL);

        property.currentNode = 0;
        fatherCrossNode = returnCrossNode(father, rand()%returnMaxElements(father));

        property.currentNode = 0;
        motherCrossNode = returnCrossNode(mother, rand()%returnMaxElements(mother));

        j = 0;
        if(fatherCrossNode->parent != NULL) // If it is not the root (fatherCrossNode == father)
        {
            //Finds the position of the son at the father's array son[]
            while(fatherCrossNode->parent->son[j] != fatherCrossNode)
            {
                j++;
            }

            fatherCrossNode->parent->son[j] = motherCrossNode;
        }
        else
        {
            father = motherCrossNode;
        }

        j = 0;
        if(motherCrossNode->parent != NULL)
        {
            while(motherCrossNode->parent->son[j] != motherCrossNode)
            {
                j++;
            }

            motherCrossNode->parent->son[j] = fatherCrossNode;
        }
        else
        {
            mother = fatherCrossNode;
        }

        aux = motherCrossNode->parent;
        motherCrossNode->parent = fatherCrossNode->parent;
        fatherCrossNode->parent = aux;

        if(height(father) < 17)
            newGeneration[i] = father;
        else
        {
            i--;
            free(father);
        }


        if(height(mother) < 17)
            newGeneration[i+1] = mother;
        else
        {
            i--;
            free(mother);

        }
    }

    //Elitism, guarantee the best organism of each generation survives to the next
    best = 0;
    for(j = 0; j < property.populationSize; j++)
    {
        if(property.rawFitness[j] > property.rawFitness[best])
            best = j;
    }
    if(i < property.populationSize)
    {
        newGeneration[i] = copyTree(property.population[best], NULL);
        i++;
    }

    //Non sexual Reproduction, basically copy the same organism to the next generation
    while(i < property.populationSize)
    {
        indexFather = selectOrganism(rand()/(double)RAND_MAX, 0, property.populationSize-1);
        newGeneration[i] = copyTree(property.population[indexFather], NULL);
        i++;
    }

    // Mutation
    for(i = 0; i < property.mutationProbality*property.populationSize; i++)
    {
        indexFather = rand()%property.populationSize;
        tumor = copyTree(newGeneration[indexFather], NULL);

        property.currentNode = 0;
        fatherCrossNode = returnCrossNode(tumor, rand()%returnMaxElements(tumor));
        tumorCrossNode = createOrganism(3, 0, -1, NULL);

        j = 0;
        if(fatherCrossNode->parent != NULL)
        {
            while(fatherCrossNode->parent->son[j] != fatherCrossNode)
            {
                j++;
            }
            fatherCrossNode->parent->son[j] = tumorCrossNode;
        }
        tumorCrossNode->parent = fatherCrossNode->parent;

        setFree(newGeneration[indexFather]);
        newGeneration[indexFather] = tumor;
    }

    for(i = 0; i < property.populationSize; i++)
    {
        setFree(property.population[i]);
        property.population[i] = newGeneration[i];
    }
}

void showOrganism(struct tNode * node)
{
    int i;
    printGene(node);

    for(i = 0; i < property.functionSets[node->value]; i++)
    {
        showOrganism(node->son[i]);
    }
}

void firstGeneration()
{
    int i, maxHeight = 3;
    int blnFull = 1;

    for(i = 0; i < property.populationSize; i++, blnFull *= -1)
    {
        maxHeight = ((i*10)/property.populationSize+2)/2 + 1; // 20% with size 2, 20% with 3, and so on..
        property.population[i] = createOrganism(maxHeight, 0, blnFull, NULL);
    }
}

void showPopulation()
{
    int i;
    for(i = 0; i < property.populationSize; i++)
    {
        printf("\ni[%d]>", i);
        showOrganism(property.population[i]);
    }
}

void normalizedFitness() // Adjusted Fitness = 1/(1 + maxFitness - rawFitnes(i), normalized = a(i)/sum
{
    int i;
    float sum = 0, probabilitySum = 0;
    int start = 0; // Creates the priority table

    for(i = 0; i < property.populationSize; i++)
    {
        property.rawFitness[i] = rawFitness(property.population[i]);
        property.fitness[i] = 1/(1 + fabs(property.maxFitness - property.rawFitness[i])); // Adjusted Fitness
        sum += property.fitness[i];
    }

    for(i = 0; i < property.populationSize; i++) // Normalized Fitness
    {
        probabilitySum += property.fitness[i] / sum;
        property.fitness[i] = probabilitySum;
    }
}

void startLife(struct tProperty p, int * functionArguments) // functionArguments represents the number of children each function can have
{
    int i, j;
    int min = 0;

    //srand(time(NULL)); // Start defines pseudo random parameters
    srand(5);

    p.fitness = (float *) malloc(p.populationSize * sizeof(float));
    p.rawFitness = (float *) malloc(p.populationSize * sizeof(float));
    p.population = (struct tNode**) malloc(p.populationSize * sizeof(struct tNode *));
    p.priorityTable = (int *) malloc(p.populationSize * sizeof(int));
    p.currentNode = 0;

    //Terminal Sets will have the defaul value 0, set by the calloc
    p.functionSets = (int *)calloc((p.functionQuantity + p.terminalQuantity), sizeof(int));

    for(i = 0; i < p.functionQuantity; i++)
    {
        p.functionSets[i] = functionArguments[i];
    }

    property = p;

    firstGeneration();
    normalizedFitness();

    i = 0;
    while(i < property.generations && property.rawFitness[min] < 89)
    {
        printf("Generation[%d] ", i);
        nextGeneration();
        normalizedFitness();

        for(min = 0, j = 1; j < property.populationSize; j++)
        {
            if(property.rawFitness[j] > property.rawFitness[min])
                min = j;
        }
        printf("%f\n", property.rawFitness[min]);
        i++;
    }

    printf("\n\n\n");
    showOrganism(property.population[min]);
    printf("\n%f", property.rawFitness[min]);
    printf("\nN of elements: %d", returnMaxElements(property.population[min]));
    printf("\nHeight: %d", height(property.population[min]));
}

#include "geneticLib.h"


struct tProperty propriedade;

int binarySearch(double target, int left, int right)
{
    int half = (left + right)/2;

    if(left >= right)
        return left;
    else if(propriedade.priorityTable[half] < target)
        return selectOrganism(target, half +1, right);
    else if(propriedade.priorityTable[half] > target)
        return selectOrganism(target, left, half - 1);
    else
        return half;
}

int selectOrganism(double target, int left, int right) // Selects the organism with binary search
{
    if(propriedade.blnOverSelection == true)
    {
        if(rand()%10 < 8)
        {
            return binarySearch(target, left, propriedade.populationSize * propriedade.overSelectionProportion);
        }
        else
        {
            return binarySearch(target, propriedade.populationSize * propriedade.overSelectionProportion + 1, right);
        }
    }
    else
    {
        return binarySearch(target, left, right);
    }
}

struct tNode * copyTree(struct tNode * old, struct tNode * parent)
{
    int i;
    struct tNode * newNode = (struct tNode*) malloc(sizeof(struct tNode));

    newNode->value = old->value;
    newNode->parent = parent;
    newNode->constant = old->constant;

    if(propriedade.functionSets[newNode->value] > 0)
    {
        newNode->son = (struct tNode**) malloc(propriedade.functionSets[newNode->value]*sizeof(struct tNode));
        for(i = 0; i < propriedade.functionSets[newNode->value]; i++)
        {
            newNode->son[i] = copyTree(old->son[i], newNode);
        }
    }
    return newNode;
}

int returnMaxElements(struct tNode * node)
{
    int i;
    int sum = 1; // It's needed to be set as 1 in order to count every node

    if (node != NULL)
    {
        for (i = 0; i < propriedade.functionSets[node->value]; i++)
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

    if(propriedade.currentNode == crossNode)
    {
        return node;
    }
    else if(node != NULL)
    {
        for (i = 0; i < propriedade.functionSets[node->value]; i++)
        {
            propriedade.currentNode++;
            if (aux == NULL)
                aux = returnCrossNode(node->son[i], crossNode);
        }
    }

    return aux;
}

void setFree(struct tNode * node)
{
    int i;

    for(i = 0; i < propriedade.functionSets[node->value]; i++)
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
        node->value = rand()%propriedade.terminalQuantity + propriedade.functionQuantity; // Selects only terminals
    else
    {
        if(blnFull == 1) // Select only function
            node->value = rand()%propriedade.functionQuantity;
        else
            node->value = rand()%(propriedade.terminalQuantity + propriedade.functionQuantity); // Selects either terminals or functions
    }

    node->parent = parent;

    if(propriedade.randomConstant.blnHas == 1 && propriedade.randomConstant.terminalIndex == node->value)
    {
        node->constant = createRandomConstant(propriedade.randomConstant);
    }

    if(node->value <= propriedade.functionQuantity)
    {
        node->son = (struct tNode **)malloc(propriedade.functionSets[node->value] * sizeof(struct tNode));

        for(i = 0; i < propriedade.functionSets[node->value]; i++)
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

    for(i = 0; i < propriedade.functionSets[node->value]; i++)
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

    struct tNode ** newGeneration = (struct tNode **)malloc(propriedade.populationSize*sizeof(struct tNode));
    struct tNode * father, * mother;
    struct tNode * fatherCrossNode, * motherCrossNode;
    struct tNode * aux;
    struct tNode * tumor, * tumorCrossNode;


    // CrossOver - Reproduction with genetic matter exchange
    for(i = 0; i <= propriedade.populationSize * propriedade.crossOverProbability; i+=2)
    {
        indexFather = selectOrganism(rand()/(double)RAND_MAX, 0, propriedade.populationSize - 1);
        indexMother = selectOrganism(rand()/(double)RAND_MAX, 0, propriedade.populationSize - 1);

        father = copyTree(propriedade.population[indexFather], NULL);
        mother = copyTree(propriedade.population[indexMother], NULL);

        propriedade.currentNode = 0;
        fatherCrossNode = returnCrossNode(father, rand()%returnMaxElements(father));

        propriedade.currentNode = 0;
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
    for(j = 0; j < propriedade.populationSize; j++)
    {
        if(propriedade.rawFitness[j] < propriedade.rawFitness[best])
            best = j;
    }

    if(i < propriedade.populationSize)
    {
        newGeneration[i] = copyTree(propriedade.population[best], NULL);
        i++;
    }

    //Non sexual Reproduction, basically copy the same organism to the next generation
    while(i < propriedade.populationSize)
    {
        indexFather = selectOrganism(rand()/(double)RAND_MAX, 0, propriedade.populationSize-1);
        newGeneration[i] = copyTree(propriedade.population[indexFather], NULL);
        i++;
    }

    // Mutation
    for(i = 0; i < propriedade.mutationProbality*propriedade.populationSize; i++)
    {
        indexFather = rand()%propriedade.populationSize;
        tumor = copyTree(newGeneration[indexFather], NULL);

        propriedade.currentNode = 0;
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

    for(i = 0; i < propriedade.populationSize; i++)
    {
        setFree(propriedade.population[i]);
        propriedade.population[i] = newGeneration[i];
    }
}

void showOrganism(struct tNode * node, FILE * output = NULL)
{
    int i;
    if(output == NULL)
        printGene(node);
    else
    {
        fprintf(output, "%d ", node->value);
        if(propriedade.randomConstant.blnHas == 1 && propriedade.randomConstant.terminalIndex == node->value)
        {
            fprintf(output, "%f ", node->constant);
        }
    }

    for(i = 0; i < propriedade.functionSets[node->value]; i++)
    {
        showOrganism(node->son[i], output);
    }
}

void firstGeneration()
{
    int i = 0, j = 0, maxHeight = 3;
    int blnFull = 1;

    if(propriedade.blnHasInputFile)
    {
        importOrganism(&j);
    }

    for(i = j; i < propriedade.populationSize; i++, blnFull *= -1)
    {
        maxHeight = ((i*10)/propriedade.populationSize+2)/2 + 1; // 20% with size 2, 20% with 3, and so on..
        propriedade.population[i] = createOrganism(maxHeight, 0, blnFull, NULL);
    }
}

void showPopulation()
{
    int i;
    for(i = 0; i < propriedade.populationSize; i++)
    {
        printf("\ni[%d]>", i);
        showOrganism(propriedade.population[i], NULL);
    }
}

void sortPopulation(int low,int high)
{
    int pivot, j, i;
    float temp;
    struct tNode * nodeTemp;

    if(low < high)
    {
        pivot = low;
        i = low;
        j = high;

        while(i<j)
        {
            while((propriedade.fitness[i]>=propriedade.fitness[pivot])&&(i<high))
            {
                i++;
            }

            while(propriedade.fitness[j]<propriedade.fitness[pivot])
            {
                j--;
            }

            if(i<j)
            {
                nodeTemp = propriedade.population[i];
                propriedade.population[i] = propriedade.population[j];
                propriedade.population[j] = nodeTemp;

                temp = propriedade.fitness[i];
                propriedade.fitness[i]=propriedade.fitness[j];
                propriedade.fitness[j]=temp;

                temp=propriedade.rawFitness[i];
                propriedade.rawFitness[i]=propriedade.rawFitness[j];
                propriedade.rawFitness[j]=temp;
            }
        }

        nodeTemp = propriedade.population[pivot];
        propriedade.population[pivot] = propriedade.population[j];
        propriedade.population[j] = nodeTemp;

        temp = propriedade.fitness[pivot];
        propriedade.fitness[pivot] = propriedade.fitness[j];
        propriedade.fitness[j] = temp;

        temp = propriedade.rawFitness[pivot];
        propriedade.rawFitness[pivot] = propriedade.rawFitness[j];
        propriedade.rawFitness[j] = temp;

        sortPopulation(low, j-1);
        sortPopulation(j+1, high);
    }
}

struct tNode * createOrganismFromFile(FILE * inputFile, struct tNode * parent)
{
    struct tNode * node = (struct tNode *)malloc(sizeof(struct tNode));
    int i;

    fscanf(inputFile, "%d ", &node->value);

    node->parent = parent;

    if(propriedade.randomConstant.blnHas == 1 && propriedade.randomConstant.terminalIndex == node->value)
    {
        fscanf(inputFile, "%f ", &node->constant);
    }

    if(node->value <= propriedade.functionQuantity)
    {
        node->son = (struct tNode **)malloc(propriedade.functionSets[node->value] * sizeof(struct tNode));

        for(i = 0; i < propriedade.functionSets[node->value]; i++)
        {
            node->son[i] = createOrganismFromFile(inputFile, node);
        }
    }

    return node;
}

void importOrganism(int * index)
{
    FILE * inputFile;

    inputFile = fopen("output.txt", "r");
    if(inputFile != NULL)
    {
        while(!feof(inputFile))
        {
            //cout << *index << " ";
            propriedade.population[*index] = createOrganismFromFile(inputFile, NULL);
            (*index)++;
        }
    }
}

void exportOrganism(struct tNode * node = NULL)
{
    FILE * outputFile;
    struct tNode * aux;

    sortPopulation(0, propriedade.populationSize - 1);

    outputFile = fopen("output.txt", "a+");
    if(outputFile != NULL)
    {
        if(node == NULL)
        {
            for(int i = 0; i < 10; i++)
            {
                showOrganism(propriedade.population[i], outputFile);
                fprintf(outputFile, "\n");
            }
        }
        else
        {
            showOrganism(node, outputFile);
            fprintf(outputFile, "\n");
        }
        fclose(outputFile);
    }

}

void normalizedFitness() // Adjusted Fitness = 1/(1 + maxFitness - rawFitnes(i), normalized = a(i)/sum
{
    int i;
    float sum = 0, probabilitySum = 0;
    float sumGroup1 = 0, sumGroup2 = 0;

    for(i = 0; i < propriedade.populationSize; i++)
    {
        //cout << "[" << i << "] ";
        propriedade.rawFitness[i] = rawFitness(propriedade.population[i]);
        //propriedade.fitness[i] = 1/(1 + fabs(propriedade.maxFitness - propriedade.rawFitness[i])); // Adjusted Fitness
        propriedade.fitness[i] = 1/propriedade.rawFitness[i]; // Adjusted Fitness
        sum += propriedade.fitness[i];
    }

   // sortPopulation(0, propriedade.populationSize-1);

    if(propriedade.blnOverSelection == true)
    {
        sortPopulation(0, propriedade.populationSize - 1);
        for(i = 0; i <= propriedade.populationSize * propriedade.overSelectionProportion; i++)
        {
            sumGroup1 += propriedade.fitness[i];
        }

        sumGroup2 = sum - sumGroup1;

        for(i = 0; i < propriedade.populationSize * propriedade.overSelectionProportion; i++) // Normalized Fitness
        {
            probabilitySum += propriedade.fitness[i] / sumGroup1;
            propriedade.fitness[i] = probabilitySum;
        }
        probabilitySum = 0;
        while(i < propriedade.populationSize)
        {
            probabilitySum += propriedade.fitness[i] / sumGroup2;
            propriedade.fitness[i] = probabilitySum;
            i++;
        }
    }
    else
    {
        for(i = 0; i < propriedade.populationSize; i++) // Normalized Fitness
        {
            probabilitySum += propriedade.fitness[i] / sum;
            propriedade.priorityTable[i] = probabilitySum;
        }
    }
}

int startLife(struct tProperty p, int * functionArguments) // functionArguments represents the number of children each function can have
{
    int i, j;
    int outputLast = 0, outputCount = 0;
    int min = 0;

    srand(time(NULL)); // Start defines pseudo random parameters
    //srand(24871);

    p.rawFitness = (float *) malloc(p.populationSize * sizeof(float));
    p.fitness = (float *) malloc(p.populationSize * sizeof(float));
    p.priorityTable = (float *) malloc(p.populationSize * sizeof(float));
    p.population = (struct tNode**) malloc(p.populationSize * sizeof(struct tNode *));
    p.currentNode = 0;

    //Terminal Sets will have the defaul value 0, set by the calloc
    p.functionSets = (int *)calloc((p.functionQuantity + p.terminalQuantity), sizeof(int));

    for(i = 0; i < p.functionQuantity; i++)
    {
        p.functionSets[i] = functionArguments[i];
    }

    propriedade = p;

    firstGeneration();
    normalizedFitness();

    i = 0;
    while(i < propriedade.generations && propriedade.rawFitness[min] > 0)
    {
        printf("Generation[%d] ", i);
        nextGeneration();
        normalizedFitness();

        for(min = 0, j = 1; j < propriedade.populationSize; j++)
        {
           // printf("\nR[i] %f    tS[i] %f Roleta %f", propriedade.rawFitness[j], propriedade.fitness[j], propriedade.priorityTable[j]);
            if(propriedade.rawFitness[j] > propriedade.rawFitness[min])
                min = j;
        }
        printf(">>%f\n", propriedade.rawFitness[min]);
        //cout << "Best: ";
        //rawFitness(propriedade.population[min], true);
        exportOrganism(propriedade.population[min]);
        i++;
    }

    printf("\n\n\n");
    showOrganism(propriedade.population[min]);
    printf("\n%f", propriedade.rawFitness[min]);
    printf("\nN of elements: %d", returnMaxElements(propriedade.population[min]));
    printf("\nHeight: %d", height(propriedade.population[min]));

    for(min = 0, j = 1; j < propriedade.populationSize; j++)
    {
       // printf("\nR[i] %f    tS[i] %f Roleta %f", propriedade.rawFitness[j], propriedade.fitness[j], propriedade.priorityTable[j]);
        if(propriedade.rawFitness[j] > propriedade.rawFitness[min])
            min = j;
    }

    exportOrganism(propriedade.population[min]);

    return(0);
}

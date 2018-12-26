#include "matrixLib.h"

struct tNode
{
    int actionSet; // 0 IF-FOOD-AHEAD, 1 PROGN2, 5 PROGN3
                   // 2 MOVE, 3 Left, 4 Right
    struct tNode * son1;
    struct tNode * son2;
    struct tNode * son3;
};

struct tRoot
{
    int actionSet;
    int fitness;
    int maxNode;
    int indexAux;
    struct tNode *son1;
    struct tNode *son2;
    struct tNode *son3;
};

int rawFitness(struct tRoot *root, struct tNode *node, struct tMatrix *m, struct tAnt *ant)
{
    ant->movements++;

    if (node == NULL)
    {
        if (root->actionSet == 0) // IF-FOOD-AHEAD
        {
            if(foodAhead(ant, m))
            {
                rawFitness(root, root->son1, m, ant);
            }
            else
            {
                rawFitness(root, root->son2, m, ant);
            }
        }
        else if(root->actionSet == 1)
        {
            rawFitness(root, root->son1, m, ant);
            rawFitness(root, root->son2, m, ant);
        }
        else if(root->actionSet == 2)
        {
            moveAnt(ant, m);
        }
        else if(root->actionSet == 3)
        {
            rotateAnt(ant, LEFT);
        }
        else if(root->actionSet == 4)
        {
            rotateAnt(ant, RIGHT);
        }
        else if(root->actionSet == 5)
        {
            rawFitness(root, root->son1, m, ant);
            rawFitness(root, root->son2, m, ant);
            rawFitness(root, root->son3, m, ant);
        }
    }
    else
    {
        if (node->actionSet == 0) // IF-FOOD-AHEAD
        {
            if(foodAhead(ant, m))
            {
                rawFitness(root, node->son1, m, ant);
            }
            else
            {
                rawFitness(root, node->son2, m, ant);
            }
        }
        else if(node->actionSet == 1)
        {
            rawFitness(root, node->son1, m, ant);
            rawFitness(root, node->son2, m, ant);
        }
        else if(node->actionSet == 2)
        {
            moveAnt(ant, m);
        }
        else if(node->actionSet == 3)
        {
            rotateAnt(ant, LEFT);
        }
        else if(node->actionSet == 4)
        {
            rotateAnt(ant, RIGHT);
        }
        else if(node->actionSet == 5)
        {
            rawFitness(root, node->son1, m, ant);
            rawFitness(root, node->son2, m, ant);
            rawFitness(root, node->son3, m, ant);
        }
    }
}

int height(struct tNode * node)
{
    int son1 = 0, son2 = 0, son3 = 0;
    int heigher = 0;

    if(node->son1 == NULL && node->son2 == NULL & node->son3 == NULL)
        return 0;
    else
    {
         if (node->son1 != NULL)
        son1 = height(node->son1);

        if (node->son2 != NULL)
            son2 = height(node->son2);

        if (node->son3 != NULL)
            son3 = height(node->son3);


        if (son1 >= son2)
            heigher = son1;
        else
            heigher = son2;

        if (son3 > heigher)
            heigher = son3;

        return heigher + 1;
    }
}

int heightTree(struct tRoot root)
{
    int son1 = 0, son2 = 0, son3 = 0, higher = 0;
    if(root.son1 != NULL)
        son1 = height(root.son1);

    if(root.son2 != NULL)
        son2 = height(root.son2);

    if(root.son3 != NULL)
        son3 = height(root.son3);

    if (son1 >= son2)
        higher = son1;
    else
        higher = son2;

    if (son3 > higher)
        higher = son3;

    return higher + 1;

}

struct tNode * generatePrimitive(struct tRoot * root, int height, int maxHeight, int blnFull)
{
    int number;

    if(height == maxHeight)
    {
        number = rand()%2 + 3;
        //printf(" {");
    }
    else if(blnFull == 1 || height == 0)
    {
        do
        {
            number = rand()%6;
        }while(number >=2 && number <=4);
    }
    else
        number = rand()%6;

    if(height == 0)
    {
        root->actionSet = number;
        //printf("\n\nr%d", number);
        root->son1 = generatePrimitive(root, height + 1, maxHeight, blnFull);
        root->son2 = generatePrimitive(root, height + 1, maxHeight, blnFull);
        if(root->actionSet == 5)
            root->son3 = generatePrimitive(root, height + 1, maxHeight, blnFull);

        return NULL;
    }
    else
    {
        struct tNode * newNode = malloc(sizeof(struct tNode));
        newNode->son1 = NULL;
        newNode->son2 = NULL;
        newNode->son3 = NULL;
        newNode->actionSet = number;
        //printf(" %d", number);

        if (number < 2)
        {
            newNode->son1 = generatePrimitive(root, height + 1, maxHeight, blnFull);
            newNode->son2 = generatePrimitive(root, height + 1, maxHeight, blnFull);
        }
        else if(number == 5)
        {
            newNode->son1 = generatePrimitive(root, height + 1, maxHeight, blnFull);
            newNode->son2 = generatePrimitive(root, height + 1, maxHeight, blnFull);
            newNode->son3 = generatePrimitive(root, height + 1, maxHeight, blnFull);
        }

        return newNode;
    }
}

void showDecisionTree(struct tRoot root, struct tNode * node)
{
    if(node == NULL)
    {
        printf("r%d ", root.actionSet);
        if(root.son1 != NULL)
        {
            showDecisionTree(root, root.son1);
        }
        if(root.son2 != NULL)
        {
            showDecisionTree(root, root.son2);
        }
        if(root.son3 != NULL)
        {
            showDecisionTree(root, root.son3);
        }
    }
    else
    {
        printf("%d ", node->actionSet);
        if(node->son1 != NULL)
        {
            showDecisionTree(root, node->son1);
        }
        if(node->son2 != NULL)
        {
            showDecisionTree(root, node->son2);
        }
        if(node->son3 != NULL)
        {
            showDecisionTree(root, node->son3);
        }
    }
}

void firstGeneration(struct tRoot population[M])
{
    int i;
    int blnFull = 1;
    int maxHeight;

    for (i = 0; i < M; i++)
    {
        maxHeight = rand()%5 +1;

        //printf("i[%d]: ", i);
        generatePrimitive(&population[i], 0, maxHeight, blnFull);
        //showDecisionTree(population[i], NULL);
        //printf("\n");
        blnFull *= -1;
    }
};

void fitness(struct tRoot *root, struct tMatrix m, int blnPrint)
{
    struct tAnt ant;
    int i = 0;

    ant.direction = RIGHT;
    ant.x = ant.y = 0;
    ant.fitness = 0;
    ant.movements = 0;

    while(ant.movements < 700)
    {
        rawFitness(root, NULL, &m, &ant);
    }
    root->fitness = ant.fitness;
    //printf("%d\n", ant.fitness);
    if (blnPrint)
        printMatrix(&m);
}

void rowPath(struct tRoot *root, struct tNode *node, struct tMatrix *m, struct tAnt *ant)
{
    ant->movements++;
    getch();
    system("cls");
    m->m[ant->x][ant->y] = ant->direction;
    printMatrix(m);

    if (node == NULL)
    {
        if (root->actionSet == 0) // IF-FOOD-AHEAD
        {
            if(foodAhead(ant, m))
            {
                rowPath(root, root->son1, m, ant);
            }
            else
            {
                rowPath(root, root->son2, m, ant);
            }
        }
        else if(root->actionSet == 1)
        {
            rowPath(root, root->son1, m, ant);
            rowPath(root, root->son2, m, ant);
        }
        else if(root->actionSet == 2)
        {
            moveAnt(ant, m);
        }
        else if(root->actionSet == 3)
        {
            rotateAnt(ant, LEFT);
        }
        else if(root->actionSet == 4)
        {
            rotateAnt(ant, RIGHT);
        }
        else if(root->actionSet == 5)
        {
            rowPath(root, root->son1, m, ant);
            rowPath(root, root->son2, m, ant);
            rowPath(root, root->son3, m, ant);
        }
    }
    else
    {
        if (node->actionSet == 0) // IF-FOOD-AHEAD
        {
            if(foodAhead(ant, m))
            {
                rowPath(root, node->son1, m, ant);
            }
            else
            {
                rowPath(root, node->son2, m, ant);
            }
        }
        else if(node->actionSet == 1)
        {
            rowPath(root, node->son1, m, ant);
            rowPath(root, node->son2, m, ant);
        }
        else if(node->actionSet == 2)
        {
            moveAnt(ant, m);
        }
        else if(node->actionSet == 3)
        {
            rotateAnt(ant, LEFT);
        }
        else if(node->actionSet == 4)
        {
            rotateAnt(ant, RIGHT);
        }
        else if(node->actionSet == 5)
        {
            rowPath(root, node->son1, m, ant);
            rowPath(root, node->son2, m, ant);
            rowPath(root, node->son3, m, ant);
        }
    }
}


void showPath(struct tRoot root, struct tMatrix m)
{
    struct tAnt ant;
    int i = 0;

    ant.direction = RIGHT;
    ant.x = ant.y = 0;
    ant.fitness = 0;
    ant.movements = 0;

    getch();
    while(ant.movements < 700)
    {
        rowPath(&root, NULL, &m, &ant);
    }
}

void generatePriorityTable(int priorityTable[M], struct tRoot population[M])
{
    int i = 0, sum = 0;
    int j = 0, start = 0;
    int competetive1, competitive2;

    for(i = 0; i < M; i++)
    {
        sum += population[i].fitness;
    }

    for(i = 0; i < M; i++)
    {
        start = j;
        while(j < (population[i].fitness*M/sum) + start)
        {
            priorityTable[j] = i;
            j++;
        }
    }

    while(j < M)
    {
        competetive1 = rand()%M;
        competitive2 = rand()%M;
        if (population[competetive1].fitness > population[competitive2].fitness)
        {
            priorityTable[j] = competetive1;
        }
        else
        {
            priorityTable[j] = competitive2;
        }
        j++;
    }
}

int countNodes(struct tRoot * root, struct tNode * node)
{
    root->maxNode++;

    if(node == NULL)
    {
        if(root->son1 != NULL && root->son2 != NULL)
        {
            countNodes(root, root->son1);
            countNodes(root, root->son2);
        }
        if(root->son3 != NULL)
            countNodes(root, root->son3);
    }
    else
    {
        if(node->son1 != NULL && node->son2 != NULL)
        {
            countNodes(root, node->son1);
            countNodes(root, node->son2);
        }
        if(node->son3 != NULL)
            countNodes(root, node->son3);
    }
}

struct tNode * returnNode(struct tRoot * root, struct tNode * node, int crossPoint) // return the pointer to the node at index x at the decision tree
{
    root->indexAux++;
    struct tNode *aux = NULL;

    if(root->indexAux == crossPoint)
    {
        root->indexAux = 0;
        if(node == NULL)
        {
            node = malloc(sizeof(struct tNode));
            node->actionSet = root->actionSet;
            node->son1 = root->son1;
            node->son2 = root->son2;
            node->son3 = root->son3;
        }

        return node;
    }
    else if(root->indexAux == root->maxNode)
    {
        printf("ERROR AT FUNCTION RETURN NODE!!! Crosspoint not found\n");
        return NULL;
    }
    else if(node == NULL)
    {
        if(root->son1 != NULL)
        {
            aux = returnNode(root, root->son1, crossPoint);
            if (aux == NULL && root->son2 != NULL)
                aux = returnNode(root, root->son2, crossPoint);
            if (aux == NULL && root->son3 != NULL)
                aux = returnNode(root, root->son3, crossPoint);

            return aux;
        }
        else
        {
            return NULL;
        }
    }
    else if (node != NULL)
    {
        if(node->son1 != NULL)
        {
            aux = returnNode(root, node->son1, crossPoint);
            if (aux == NULL && node->son2 != NULL)
                aux = returnNode(root, node->son2, crossPoint);
            if (aux == NULL && node->son3 != NULL)
                aux = returnNode(root, node->son3, crossPoint);

            return aux;
        }
        else
        {
            return NULL;
        }
    }
};

void copySubTree(struct tRoot * root, struct tNode * nodeOld, struct tNode * nodeNew)
{
    struct tNode * son1 = malloc(sizeof(struct tNode));
    struct tNode * son2 = malloc(sizeof(struct tNode));
    struct tNode * son3 = malloc(sizeof(struct tNode));

    son1->son1 = NULL;
    son1->son2 = NULL;
    son1->son3 = NULL;

    son2->son1 = NULL;
    son2->son2 = NULL;
    son2->son3 = NULL;

    son3->son1 = NULL;
    son3->son2 = NULL;
    son3->son3 = NULL;

    if(nodeNew == NULL)
    {
        root->actionSet = nodeOld->actionSet;
        root->indexAux = 0;
        root->maxNode = 0;


        if(nodeOld->son1 != NULL && nodeOld->son2 != NULL)
        {
            root->son1 = son1;
            root->son2 = son2;

            copySubTree(root, nodeOld->son1, root->son1);
            copySubTree(root, nodeOld->son2, root->son2);

            if(nodeOld->son3 != NULL)
            {
                root->son3 = son3;
                copySubTree(root, nodeOld->son3, root->son3);
            }
            else
            {
                free(son3);
            }
        }
        else
        {
            free(son1);
            free(son2);
            free(son3);
        }
    }
    else
    {
        nodeNew->actionSet = nodeOld->actionSet;

        if(nodeOld->son1 != NULL && nodeOld->son2 != NULL)
        {
            nodeNew->son1 = son1;
            nodeNew->son2 = son2;

            copySubTree(root, nodeOld->son1, nodeNew->son1);
            copySubTree(root, nodeOld->son2, nodeNew->son2);

            if(nodeOld->son3 != NULL)
            {
                nodeNew->son3 = son3;
                copySubTree(root, nodeOld->son3, nodeNew->son3);
            }
            else
            {
                free(son3);
            }
        }
        else
        {
            free(son1);
            free(son2);
            free(son3);
        }
    }
}

void crossGenes(struct tRoot * rootNew, struct tRoot * rootOld, struct tNode * nodeNew, struct tNode * nodeOld, struct tNode * gene, int crossPoint)
{
    rootNew->indexAux++;
    struct tNode * nodeCopy1 = malloc(sizeof(struct tNode));
    struct tNode * nodeCopy2 = malloc(sizeof(struct tNode));
    struct tNode * nodeCopy3 = malloc(sizeof(struct tNode));

    nodeCopy1->son1 = NULL;
    nodeCopy1->son2 = NULL;
    nodeCopy1->son3 = NULL;

    nodeCopy2->son1 = NULL;
    nodeCopy2->son2 = NULL;
    nodeCopy2->son3 = NULL;

    nodeCopy3->son1 = NULL;
    nodeCopy3->son2 = NULL;
    nodeCopy3->son3 = NULL;

    if(nodeOld == NULL)
    {
        rootNew->actionSet = rootOld->actionSet;
        if(rootOld->son1 != NULL && rootOld->son2 != NULL)
        {
            rootNew->son1 = nodeCopy1;
            rootNew->son2 = nodeCopy2;
            if(rootNew->indexAux+1 == crossPoint)
            {
                crossGenes(rootNew, rootOld, rootNew->son1, gene, gene, crossPoint);
            }
            else
            {
                crossGenes(rootNew, rootOld, rootNew->son1, rootOld->son1, gene, crossPoint);
            }

            if(rootNew->indexAux+1 == crossPoint)
            {
                crossGenes(rootNew, rootOld, rootNew->son2, gene, gene, crossPoint);
            }
            else
            {
                crossGenes(rootNew, rootOld, rootNew->son2, rootOld->son2, gene, crossPoint);
            }

            if(rootOld->son3 != NULL)
            {
                rootNew->son3 = nodeCopy3;
                if(rootNew->indexAux+1 == crossPoint)
                    crossGenes(rootNew, rootOld, rootNew->son3, gene, gene, crossPoint);
                else
                    crossGenes(rootNew, rootOld, rootNew->son3, rootOld->son3, gene, crossPoint);
            }
            else
            {
                free(nodeCopy3);
            }
        }
        else
        {
            free(nodeCopy1);
            free(nodeCopy2);
            free(nodeCopy3);
        }
    }
    else
    {
        nodeNew->actionSet = nodeOld->actionSet;
        if(nodeOld->son1 != NULL && nodeOld->son2 != NULL)
        {
            nodeNew->son1 = nodeCopy1;
            nodeNew->son2 = nodeCopy2;

            if(rootNew->indexAux+1 == crossPoint)
            {
                crossGenes(rootNew, rootOld, nodeNew->son1, gene, gene, crossPoint);
            }
            else
            {
                crossGenes(rootNew, rootOld, nodeNew->son1, nodeOld->son1, gene, crossPoint);
            }

            if(rootNew->indexAux+1 == crossPoint)
            {
                crossGenes(rootNew, rootOld, nodeNew->son2, gene, gene, crossPoint);
            }
            else
            {
                crossGenes(rootNew, rootOld, nodeNew->son2, nodeOld->son2, gene, crossPoint);
            }

            if(nodeOld->son3 != NULL)
            {
                nodeNew->son3 = nodeCopy3;
                if(rootNew->indexAux+1 == crossPoint)
                    crossGenes(rootNew, rootOld, nodeNew->son3, gene, gene, crossPoint);
                else
                    crossGenes(rootNew, rootOld, nodeNew->son3, nodeOld->son3, gene, crossPoint);
            }
            else
            {
                free(nodeCopy3);
            }
        }
        else
        {
            free(nodeCopy1);
            free(nodeCopy2);
            free(nodeCopy3);
        }
    }
}

void clearMemory(struct tRoot * root, struct tNode * node)
{
    if(node == NULL)
    {
        if(root->son1 != NULL)
            clearMemory(root, root->son1);
        if(root->son2 != NULL)
            clearMemory(root, root->son2);
        if(root->son3 != NULL)
            clearMemory(root, root->son3);
    }
    else
    {
        if(node->son1 != NULL)
            clearMemory(root, node->son1);
        if(node->son2 != NULL)
            clearMemory(root, node->son2);
        if(node->son3 != NULL)
            clearMemory(root, node->son3);

        free(node);
    }

}

void nextGeneration(struct tRoot population[M])
{
    struct tRoot next[M];
    struct tRoot aux;
    int priorityTable[M];
    int father, mother;
    int fatherCrossPoint, motherCrossPoint;
    struct tNode * nodeCross;

    generatePriorityTable(priorityTable, population);
    int i;

    for (i = 0; i+1 < 0.90*M; i+=2)
    {
        father = priorityTable[rand()%M];
        mother = priorityTable[rand()%M];
        population[father].maxNode = 0;
        population[father].indexAux = 0;
        countNodes(&population[father], NULL);

        population[mother].maxNode = 0;
        population[mother].indexAux = 0;
        countNodes(&population[mother], NULL);
        fatherCrossPoint = rand() % population[father].maxNode;
        motherCrossPoint = rand() % population[mother].maxNode;

        if (fatherCrossPoint == 0)
            fatherCrossPoint++;
        if (motherCrossPoint == 0)
            motherCrossPoint++;

        //printf("\nfather[%d](%d) <-> mother[%d](%d)", father, fatherCrossPoint, mother, motherCrossPoint);

        next[i].indexAux = next[i+1].indexAux = 0;
        next[i].maxNode = next[i+1].maxNode = 0;
        next[i].son1 = next[i+1].son1 = NULL;
        next[i].son2 = next[i+1].son2 = NULL;
        next[i].son3 = next[i+1].son3 = NULL;
        next[i].fitness = next[i+1].fitness = 0;

        if (fatherCrossPoint > 1 && motherCrossPoint > 1)
        {

            nodeCross = returnNode(&population[father], NULL, fatherCrossPoint);

            //printf("\ni[%d] = ", i);
            //printf("Father[%d] node[%d] nodecross: %d", father, fatherCrossPoint, nodeCross->actionSet);
            //printf(" into Mother[%d] node[%d]", mother, motherCrossPoint);

            crossGenes(&next[i], &population[mother], NULL, NULL, nodeCross, motherCrossPoint);

            //printf("\ni[%d] = ", i+1);

            nodeCross = returnNode(&population[mother], NULL, motherCrossPoint);
            //printf("Mother[%d] node[%d] nodecross: %d", mother, motherCrossPoint, nodeCross->actionSet);
            //printf(" into Father[%d] node[%d]\n", father, fatherCrossPoint);
            crossGenes(&next[i+1], &population[father], NULL, NULL, nodeCross, fatherCrossPoint);
        }
        else if(fatherCrossPoint == 1 && motherCrossPoint > 1)
        {
            nodeCross = returnNode(&population[father], NULL, fatherCrossPoint);
            //printf("\ni[>%d] = ", i);
            //printf("Father[%d] node[%d] nodecross: %d", father, fatherCrossPoint, nodeCross->actionSet);
            //printf(" into Mother[%d] node[%d]", mother, motherCrossPoint);
            crossGenes(&next[i], &population[mother], NULL, NULL, nodeCross, motherCrossPoint);

            //printf("\ni[%d] = ", i+1);
            //printf("Mother[%d] node[%d] nodecross: %d", mother, motherCrossPoint, nodeCross->actionSet);
            //printf(" into Father[%d] node[%d]\n", father, fatherCrossPoint);

            nodeCross = returnNode(&population[mother], NULL, motherCrossPoint);

            copySubTree(&next[i+1], nodeCross, NULL);

            //crossGenes(&next[i+1], &population[mother], NULL, nodeCross, NULL, 0)
/*
            showDecisionTree(population[father], NULL);
            printf("\n");
            showDecisionTree(population[mother], NULL);
            printf("\n");
            showDecisionTree(next[i], NULL);
            printf("\n");
            showDecisionTree(next[i+1], NULL);
            printf("\n");
            */

        }
        else if(motherCrossPoint == 1 && fatherCrossPoint > 1)
        {
            nodeCross = returnNode(&population[father], NULL, fatherCrossPoint);

            //printf("\ni[<%d] = ", i);
            //printf("Father[%d] node[%d] nodecross: %d", father, fatherCrossPoint, nodeCross->actionSet);
            //printf(" into Mother[%d] node[%d]", mother, motherCrossPoint);

            copySubTree(&next[i], nodeCross, NULL);

            nodeCross = returnNode(&population[mother], NULL, motherCrossPoint);

            //printf("\ni[%d] = ", i+1);
            //printf("Mother[%d] node[%d] nodecross: %d", mother, motherCrossPoint, nodeCross->actionSet);
            //printf(" into Father[%d] node[%d]\n", father, fatherCrossPoint);

            crossGenes(&next[i+1], &population[father], NULL, NULL, nodeCross, fatherCrossPoint);

/*
            showDecisionTree(population[father], NULL);
            printf("\n");
            showDecisionTree(population[mother], NULL);
            printf("\n");
            showDecisionTree(next[i], NULL);
            printf("\n");
            showDecisionTree(next[i+1], NULL);
            printf("\n");*/
        }
        else
        {
            i-=2;
        }
        // Limit the generate tree's height to 10

        //if(motherCrossPoint > 1 || fatherCrossPoint > 1)
        //{
            if ((i > 0)&& (heightTree(next[i]) > 10 || heightTree(next[i+1]) > 10))
            {
                population[i].indexAux = 0;
                nodeCross = returnNode(&population[i], NULL, 1);
                copySubTree(&next[i], nodeCross, NULL);
                //next[i] = population[i];
                clearMemory(&next[i+1], NULL);
                i--;
            }
        //}
    }

    //Reproduction
    if(i < M) // Guarantee the best of each generation makes to the next
    {
        next[i].indexAux = 0;
        next[i].maxNode = 0;
        next[i].son1 = NULL;
        next[i].son2 = NULL;
        next[i].son3 = NULL;
        next[i].fitness = population[0].fitness;

        population[0].indexAux = 0;
        nodeCross = returnNode(&population[0], NULL, 1);
        copySubTree(&next[i], nodeCross, NULL);
        i++;
    }

    while (i < M)
    {
        next[i].indexAux = 0;
        next[i].maxNode = 0;
        next[i].son1 = NULL;
        next[i].son2 = NULL;
        next[i].son3 = NULL;
        next[i].fitness = 0;

        father = priorityTable[rand()%M];
        population[father].maxNode = 0;
        countNodes(&population[father], NULL);
        fatherCrossPoint = rand() % population[father].maxNode;

        //Mutation
        if(i%5 == 1 && fatherCrossPoint > 1)
        {
            generatePrimitive(&aux, 0, 3, 0);
            nodeCross = returnNode(&aux, NULL, 1);
            crossGenes(&next[i], &population[father], NULL, NULL, nodeCross, fatherCrossPoint);
        }
        else
        {
            population[father].indexAux = 0;
            nodeCross = returnNode(&population[father], NULL, 1);
            copySubTree(&next[i], nodeCross, NULL);
            //next[i] = population[father];
        }
        i++;

    }

    //Replace last generation

    //getch();

    for(i = 0; i < M; i++)
    {
        clearMemory(&population[i], NULL);
        population[i] = next[i];
    }
    //getch();
}

void sortPopulation(struct tRoot population[M])
{
    struct tRoot aux;
    int highestValue;
    int highestPosition;
    int i, j;
    for(i = 0; i < M; i++)
    {
        highestValue = population[i].fitness;
        highestPosition = i;

        for (j = i; j < M; j++)
        {
            if(population[j].fitness > highestValue)
            {
                highestValue = population[j].fitness;
                highestPosition = j;
            }
        }

        if (highestPosition != i)
        {
            aux = population[i];
            population[i] = population[highestPosition];
            population[highestPosition] = aux;
        }
    }
}

int main()
{
    struct tRoot population[M];
    struct tMatrix m;
    int i = 0, j = 0;

    for(i = 0; i < M; i++)
    {
        population[i].son1 = NULL;
        population[i].son2 = NULL;
        population[i].son3 = NULL;
        population[i].fitness = 0;
        population[i].maxNode = 0;
        population[i].indexAux = 0;
    }

    //srand(time(NULL));
     srand(15);
    fillupMatrix(&m);
    //printMatrix(&m);

    firstGeneration(population);

    for(j = 0; j < G && population[0].fitness < 89; j++)
    {
        //printf("\n%d", j);

        for(i = 0; i < M; i++)
        {
            fitness(&population[i], m, 0);
        }

        sortPopulation(population);
        //for(i = 0; i < M; i++)
        //{
            printf("\ni[%d] = %d", j, population[0].fitness);
        //}
        //printf("\n\n\n");
        nextGeneration(population);

    }


    sortPopulation(population);

    showDecisionTree(population[0], NULL);
    printf("\n");
    for(i =0 ; i < 6; i++)
    {
        fitness(&population[i], m, 1);
        printf("\ni[%d] = %d\n", i, population[i].fitness);
        //showDecisionTree(population[i], NULL);
        printf("\n");
    }

    showPath(population[0], m);
}

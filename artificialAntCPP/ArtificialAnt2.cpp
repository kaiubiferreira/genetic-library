#include "geneticLibrary.h"
#include "matrixLib.h"

#define IF_FOOD_AHEAD 0
#define PROGN2 1
#define PROGN3 2
#define MOV 3
#define TURNLEFT 4
#define TURNRIGHT 5

void printGene(struct tNode * node)
{
    switch(node->value)
    {
        case IF_FOOD_AHEAD: printf("(if food) ") ;
            break;
        case PROGN2: printf("Exe2 ");
            break;
        case PROGN3: printf("Exe3 ");;
            break;
        case MOV: printf("Move ");
            break;
        case TURNLEFT: printf("Left ");
            break;
        case TURNRIGHT: printf("Right ");
            break;
    }
}

float calcFitness(struct tNode *node, struct tMatrix *m, struct tAnt *ant)
{

    ant->movements++;
    if (node->value == IF_FOOD_AHEAD) // IF-FOOD-AHEAD
    {
        if(foodAhead(ant, m))
        {
            calcFitness(node->son[0], m, ant);
        }
        else
        {
            calcFitness(node->son[1], m, ant);
        }
    }
    else if(node->value == PROGN2)
    {
        calcFitness(node->son[0], m, ant);
        calcFitness(node->son[1], m, ant);
    }
    else if(node->value == PROGN3)
    {
        calcFitness(node->son[0], m, ant);
        calcFitness(node->son[1], m, ant);
        calcFitness(node->son[2], m, ant);
    }
    else if(node->value == MOV)
    {
        moveAnt(ant, m);
    }
    else if(node->value == TURNLEFT)
    {
        rotateAnt(ant, LEFT);
    }
    else if(node->value == TURNRIGHT)
    {
        rotateAnt(ant, RIGHT);
    }

}

float rawFitness(struct tNode * node)
{
    struct tMatrix m;
    struct tAnt ant;
    int i = 0;

    fillupMatrix(&m);

    ant.direction = RIGHT;
    ant.x = ant.y = 0;
    ant.fitness = 0;
    ant.movements = 0;

    while(ant.movements < 700)
    {
        calcFitness(node, &m, &ant);
    }

    return (float)(ant.fitness);
}


int main()
{
    struct tProperty p;
    p.populationSize = 2000;
    p.generations = 500;
    p.mutationProbality = 0.00;
    p.reproductionProbability = 0.01;
    p.crossOverProbability = 0.9;
    p.terminalQuantity = 3;
    p.functionQuantity = 3;
    p.maxFitness = 89;
    p.randomConstant.blnHas = 0;
    //p.blnOverSelection = 1;
    //p.overSelectionProportion = 0.04;

    int function[3] = {2, 2, 3};
    startLife(p, function);
}

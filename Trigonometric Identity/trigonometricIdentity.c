#include "C:\Kaiubi\Development\Genetic Algorithms\GeneticLibrary\geneticLibrary.h"

#define SUM 0
#define SUB 1
#define MULT 2
#define DIV 3
#define SIN 4
#define X 5
// constant 6 = (1)


void printGene(struct tNode * node)
{
    switch(node->value)
    {
        case SUM: printf("+ ");
            break;
        case SUB: printf("- ");
            break;
        case MULT: printf("* ");;
            break;
        case DIV: printf("/ ");
            break;
        case SIN: printf("SIN() ");
            break;
        case X: printf("X ");
            break;
        default: printf("1 ");
    }
}

float calcSample(struct tNode * node, float sample)
{
    float value1 = 0;
    float value2 = 0;

    if(property.functionSets[node->value] >= 1)
    {
        value1 = calcSample(node->son[0], sample);
    }

    if(property.functionSets[node->value] == 2)
    {
        value2 = calcSample(node->son[1], sample);
    }

    switch (node->value)
    {
        case SUM: return value1 + value2;
            break;
        case SUB: return value1 - value2;
            break;
        case MULT: return value1 * value2;
            break;
        case DIV: if(value2 != 0)
                    return value1 / value2;
                  else
                    return 0;
            break;
        case SIN: return sin(value1);
            break;
        case X: return sample;
            break;
        default: return 1;
            break;
    }

    return 1;
}

float rawFitness(struct tNode * node)
{
    int i;
    float sample = 0;
    float result, sum = 0; // Sum = sum of the differences at each sample

    for(i = 0; i < 20; i++)
    {
        sample += 2*M_PI/20; // calculates the function in terms of discrete sample values from 0 to 2PI
        result = calcSample(node, sample);
        sum += fabs(result - cos(2*sample));
        //sum += fabs(result - (cos(sample)*sin(sample))); // Compares the function created with the root sidentity
    }

    return sum;
}

int main()
{
    struct tProperty p;
    p.populationSize = 5000;
    p.generations = 70;
    p.mutationProbality = 0.01;
    p.reproductionProbability = 0.1;
    p.crossOverProbability = 0.9;
    p.terminalQuantity = 2;
    p.functionQuantity = 5;
    p.maxFitness = 0;
    p.randomConstant.blnHas = 0;
    p.blnOverSelection = 0;

    int function[5] = {2, 2, 2, 2, 1};
    startLife(p, function);
}

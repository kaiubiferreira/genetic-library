#include "C:\Kaiubi\Development\Genetic Algorithms\GeneticLibrary\geneticLibrary.h"

//Initial attributes


//Phisical attributes
#define G 1.0  // Gravity
#define MC 0.9 // Car mass
#define MB 0.1 // Broom mass
#define T 0.02 // Time step
#define L 0.8106 // Broom lenght

//Functions - Terminals
#define SUM  0
#define SUB  1
#define MULT 2
#define DIV  3
#define SIG  4
#define ABS  5
#define SRT  6 // Square root
#define SQ   7 // Squared
#define CUB  8 // Cubed
#define SIN   9 // Real Greater-than
#define VEL  10// Velocity
#define ANG  11// Angle
#define AVL  12// Angular Velocity
#define R    13// Random floating-point constant from -1.000 to 1.000

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
        case ABS: printf("ABS() ");
            break;
        case SRT: printf("SRT() ");
            break;
        case SQ: printf("SQ() ");
            break;
        case CUB: printf("CUB() ");
            break;
        case SIN: printf("SIN() ");
            break;
        case VEL: printf("v ");
            break;
        case ANG: printf("%c ", 233); // Greek Upper case theta
            break;
        case AVL: printf("w ");
            break;
        case R: printf("%f ", node->constant);
            break;
    }
}

float angularAcceleration(float force, float angle, float angularVelocity)
{
    float result;
    result = G*sin(angle) + (cos(angle)*(-force - MB*L*angularVelocity*angle*angle*sin(angle)))/(MC + MB);
    result = result/(L*((4/3) - (MB*cos(angle)*cos(angle))/(MC + MB)));
    return result;
}

float linearAcelleration(float force, float angle, float angularVelocity)
{
    float result;
    result = (force + MB*L*(angle*angle*sin(angle) - angularVelocity*cos(angle)))/(MC + MB);
    return result;
}

float calcForce(float angle, float angularVelocity, float velocity, struct tNode * node)
{
    float value1 = 0, value2 = 0;
    if(property.functionSets[node->value] >= 1)
    {
        value1 = calcForce(angle, angularVelocity, velocity, node->son[0]);
    }

    if(property.functionSets[node->value] == 2)
    {
        value2 = calcForce(angle, angularVelocity, velocity, node->son[1]);
    }

    switch(node->value)
    {
        case SUM: return value1 + value2;
            break;
        case SUB: return value1 - value2;
            break;
        case MULT: return value1 * value2;
            break;
        case DIV: if(value2 != 0)
                    return (value1 / value2);
                  else
                    return 0;
            break;
        case SIG: if(value1 < 0) return -1;
                  else if(value1 > 0) return 1;
                  else return 0;
            break;
        case ABS: return fabs(value1);
            break;
        case SRT: return sqrt(fabs(value1));
            break;
        case SQ: return value1*value1;
            break;
        case CUB: return value1*value1*value1;
            break;
        case SIN:
                return sin(value1);
            break;
        case VEL: return velocity;
            break;
        case ANG: return angle; // Greek Upper case theta
            break;
        case AVL: return angularVelocity; // Greek Lower case omega
            break;
        case R: return node->constant;
            break;
    }

    printf("Valueeeeeee %d\n", node->value);
    getch();
}

float fitnessCase(float x, float velocity, float angle, float angularVelocity, float force, struct tNode * node)
{
    int i = 0;
    float time = 0.0;
    float forceBefore;
    float angleBefore;
    float angularVelocityBefore;
    float velocityBefore;
    int wrapper = 1;

   // printf("Att:\n\n");
    while(i < 300)
    {
        time += 0.02;
        forceBefore = force;
        angleBefore = angle;
        angularVelocityBefore = angularVelocity;
        velocityBefore = velocity;

        //printf("F = %f, v = %f angle = %f, angularVelocity = %f\n", force, velocity, angle*(180/M_PI), angularVelocity);

        force = calcForce(angleBefore, angularVelocityBefore, velocityBefore, node);

        if((force > 0 && wrapper < 0) || (force < 0 && wrapper > 0))
        {
            force *= -1;
        }

        wrapper *= -1;

        angle += 0.02*angularVelocityBefore; // The order is right, angle before angular velocity
        velocity += 0.02*linearAcelleration(forceBefore, angleBefore, angularVelocityBefore);
        angularVelocity += 0.02*angularAcceleration(forceBefore, angleBefore, angularVelocityBefore);

       // printf("F = %f, v = %f angle = %f, angularVelocity = %f\n", force, velocity, angle*(180/M_PI), angularVelocity);

        // Reach the desired state
        if(sqrt(velocity*velocity + angle*angle + angularVelocity*angularVelocity) < 0.7)
        {
            return time;
        }

        if(fabs(angle) >= M_PI/2)
            return 6;


        i++;
    }

    //getch();
    return time;
}

float rawFitness(struct tNode * node)
{
    int i;
    float x; // Position
    float v; // Velocity
    float angle; // angle =P
    float angularVelocity;
    float force; // Force initiated with 1 newton
    float time = 0;
    struct tRandomConstant randomConstant;
    randomConstant.blnHas = 1;
    randomConstant.blnIsInteger = 0;
    randomConstant.startRange = -0.2;
    randomConstant.endRange = 0.2;

    // Total of 10 fitness
    //printf("Organism:\n");
    //showOrganism(node);
    //printf("\n\n");
    for(i = 0; i < 10; i++)
    {
        force = 1.0;
        // initial values choosen pseudo randomly betweeen -0.20 and 0.20 for each fitness case
        x = createRandomConstant(randomConstant);
        v = createRandomConstant(randomConstant);
        angle = createRandomConstant(randomConstant);
        angularVelocity = createRandomConstant(randomConstant);
        time += fitnessCase(x, v, angle, angularVelocity, force, node);
    }

    return time/10;
}

int main()
{
    struct tProperty p;
    p.populationSize = 2000;
    p.generations = 50;
    p.mutationProbality = 0.01;
    p.reproductionProbability = 0.1;
    p.crossOverProbability = 0.9;
    p.terminalQuantity = 4;
    p.functionQuantity = 10;
    p.maxFitness = 0;
    p.randomConstant.blnHas = 1;
    p.randomConstant.blnIsInteger = 0;
    p.randomConstant.startRange = -1;
    p.randomConstant.endRange = 1;
    p.randomConstant.terminalIndex = R;

    int function[10] = {2,2,2,2,1,1,1,1,1,1};
    startLife(p, function);
}

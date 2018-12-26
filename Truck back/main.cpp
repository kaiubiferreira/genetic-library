#include <iostream>
#include "geneticLib.h"
#include <windows.h>
#define SUM 0
#define SUB 1
#define MULT 2
#define DIV 3
#define ATG 4
#define IFLTZ 5
#define X 6
#define Y 7
#define TANG 8
#define DIFF 9
#define R 10

using namespace std;
int counte = 0;

void printGene(struct tNode * node)
{
    switch(node->value)
    {
        case SUM: printf("+ ");
            break;
        case SUB: printf("- ");
            break;
        case MULT: printf("* ");
            break;
        case DIV: printf("/ ");
            break;
        case ATG: printf("ATG(a/b) ");
            break;
        case IFLTZ: printf("IFLTZ(3) ");
            break;
        case X: printf("X ");
            break;
        case Y: printf("Y ");
            break;
        case TANG: printf("TANG ");
            break;
        case DIFF: printf("DIFF ");
            break;
        case R: printf("%f ", node->constant);
            break;
    }
}

void drawRectangle(GLfloat x, GLfloat y, GLfloat base, GLfloat height)
{
    glBegin(GL_QUADS);
    {
        glVertex3f(x, - base/2, 0);
        glVertex3f(x, base/2, 0);

        glVertex3f(x + height, + base/2, 0);
        glVertex3f(x + height, - base/2, 0);
    }
    glEnd();
}

void drawTruck(GLfloat x, GLfloat y, GLfloat tang, GLfloat diff, GLfloat control)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);

    GLfloat cabBase = 6.0f;
    GLfloat cabHeight = 6.0f;
    GLfloat tralierBase = 6.0f;
    GLfloat trailerHeight = 14.0f;

    if(counte > 15)
        Sleep(1);

    x /= 60;
    y /= 60;
    cabBase /= 60;
    cabHeight /= 60;
    tralierBase /= 60;
    trailerHeight /= 60;

    int i, j;

    glLoadIdentity();


    glPushMatrix();
        glTranslatef(-1.5f, -1.0f, -3.0f);
        glColor3b(1,0,1);
        glRotatef(control, 0.0, 0.0, 1.0);
        drawRectangle(0, 0, 0.01, 0.5);
    glPopMatrix();

    glLoadIdentity();

    glTranslatef(-1.0f, 0.0f, -3.0f);
    glColor3b(1,1,1);
    drawRectangle(-0.01, 0, 10, 0.01);
    drawRectangle(-0.1, 0, 0.1, 0.1);

    glPushMatrix();

        glTranslatef(x, y, 0);
        glRotatef(tang, 0.0, 0.0, 1.0);
        //Trailer
        glColor3f(0.0f, 1.0f, 0.0f);
        drawRectangle(0, 0, tralierBase, trailerHeight);
        //Cab
        glTranslatef(trailerHeight + 0.05, 0, 0);
        glRotatef(diff, 0.0, 0.0, 1.0);
        glColor3f(1.0, 0.0, 0.0f);
        drawRectangle(0, 0, cabBase, cabHeight);
    glPopMatrix();

    glutPostRedisplay();
    glutSwapBuffers();
}

float calcControl(struct tNode * node, float x, float y, float tang, float diff)
{
    float value1 = 0, value2 = 0;

    if(node->value <= 5) // All functions have 2 arguments
    {
        value1 = calcControl(node->son[0], x, y, tang, diff);
        value2 = calcControl(node->son[1], x, y, tang, diff);
    }

    switch(node->value)
    {
        case SUM: return value1 + value2;
            break;
        case SUB: return value1 - value2;
            break;
        case MULT: return value1 * value2;
            break;
        case DIV:   if(value2 != 0)
                        return value1/value2;
                    else
                        return 0;
            break;
        case ATG:   return(atan2(value1, value2));
            break;
        case IFLTZ: if(value1 < 0)
                        return value2;
                    else
                        return calcControl(node->son[2], x, y, tang, diff);
            break;
        case X: return x;
            break;
        case Y: return y;
            break;
        case TANG: return tang;
            break;
        case DIFF:
                return diff;
            break;
        case R: return node->constant;
            break;
    }
}

float fitnessSet(struct tNode * node, bool simulate, float x, float y, float tang)
{
    float dc = 6, ds = 14, control = 0, r = 0.2;
    float time = 0;
    float diff;
    float tetaC, tempTetaC = 0;
    float A, B, C;
    bool stuck = false;

    diff = 0;

    tetaC = tang;

    //.showOrganism(node);

    //cout << endl << endl;

    while(time < 60)
    {
        if(x < 0)
            break;
        else if(x < 0.1 && fabs(y) < 0.42 && tang < 0.12) // Heats
        {
            Sleep(300);
            break;
        }
        else
        {
            control = calcControl(node, x, y, tang, diff);
            if (control > 1.2)
                control = 1.2;
            else if(control < -1.2)
                control = -1.2;

            //if(simulate)kaiubi
                drawTruck(x, y, tang*(180/M_PI), -diff*(180/M_PI), (control+M_PI_2)*180/M_PI);

            A = r*cos(control);
            B = A * cos(tetaC - tang);
            C = A * sin(tetaC - tang);
            x = x - B * cos(tang);
            y = y - B * sin(tang);


            tetaC = atan2((dc*sin(tetaC) - r * cos(tetaC)*sin(control)),(dc * cos(tetaC) + r * sin(tetaC)*sin(control)));
            tang = atan2((ds * sin(tang) - C * cos(tang)),(ds*cos(tang) + C*sin(tang)));

            if(fabs(tang - tetaC) < M_PI/2.5)
            {
                diff = tang - tetaC;
            }
            else
            {
                tetaC = tang - diff;
            }

            //cout << "tang: " << tang*180/M_PI << endl;
            //cout << "tetac: " << tetaC*180/M_PI << endl;
            //cout << "diff: " << diff*180/M_PI << endl;

        }
        time+= 0.02;
    }

    return sqrtf(x*x + 2*y*y + (80/M_PI) * tang*tang + sqrtf(time) + tetaC*tetaC);
}

float rawFitness(struct tNode * node, bool simulate)
{
    float sum = 0;
    int i;

    //sum += fitnessSet(node, simulate, 20, -50, -M_PI_2);
    sum += fitnessSet(node, simulate, 20, -50, M_PI_2);
    sum += fitnessSet(node, simulate, 20, 50, -M_PI_2);
    //sum += fitnessSet(node, simulate, 20, 50, M_PI_2);
    //sum += fitnessSet(node, simulate, 40, -50, -M_PI_2);
    sum += fitnessSet(node, simulate, 40, -50, M_PI_2);
    sum += fitnessSet(node, simulate, 40, 50, -M_PI_2);
    //sum += fitnessSet(node, simulate, 40, 50, M_PI_2);

    cout << sum << endl;
    counte++;
    return sum;
}

void drawScene()
{
    glLoadIdentity();

    //drawTruck(20, 40, 50, 25);

    glutSwapBuffers();
}

void calculate()
{
    struct tProperty p;
    p.populationSize = 30; //500, 100
    p.generations = 1;
    p.mutationProbality = 0.01;
    p.reproductionProbability = 0.01;
    p.crossOverProbability = 0.9;
    p.terminalQuantity = 5;
    p.functionQuantity = 6;
    p.maxFitness = 0;
    p.randomConstant.blnHas = 1;
    p.randomConstant.blnIsInteger = 0;
    p.randomConstant.startRange = -1;
    p.randomConstant.endRange = 1;
    p.randomConstant.terminalIndex = 10;
    p.blnOverSelection = false;
    p.overSelectionProportion = 0.36;
    p.blnHasInputFile = true;

    int function[6] = {2,2,2,2,2,3};
    startLife(p, function);
    exit(0);
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    Settings settings = *new Settings(calculate, "Truck");

    glutMainLoop();
    return EXIT_SUCCESS;
}

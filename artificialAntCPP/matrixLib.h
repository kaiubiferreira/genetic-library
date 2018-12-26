#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define T 32
#define UP 280
#define DOWN 281
#define LEFT 283
#define RIGHT 282
#define PATH 271

struct tMatrix
{
    int m[T][T];
};


void startAnt(struct tRoot **);
void fillupMatrix(struct tMatrix * m);
void printMatrix(struct tMatrix * m);

struct tAnt
{
    int x, y;
    int fitness;
    int direction; // UP 280 DOWN 281 LEFT 283 RIGHT 282
    int movements;
};

int foodAhead(struct tAnt *ant, struct tMatrix *m);
void moveAnt(struct tAnt * ant, struct tMatrix * m);
void rotateAnt(struct tAnt * ant, int rotateTo);

void showPath(struct tRoot root, struct tMatrix m);
void rowPath(struct tRoot *root, struct tNode *node, struct tMatrix *m, struct tAnt *ant);

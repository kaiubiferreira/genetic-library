#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define T 32
#define UP 280
#define DOWN 281
#define LEFT 283
#define RIGHT 282
#define M 10000
#define G 100
#define PATH 271

struct tMatrix
{
    int m[T][T];
};

void fillupMatrix(struct tMatrix * m)
{
    int i, j;
    for(i = 0; i < T; i++)
    {
        for(j = 0; j < T; j++)
        {
            m->m[i][j] = 0;
        }
    }

    for(i = 1; i <= 3; i++)
        m->m[0][i] = 1;

    for(i = 1; i <=5; i++)
        m->m[i][3] = 1;

    for(i = 3; i <= 12; i++)
        m->m[5][i] = 1;

    for(i = 5; i <= 24; i++)
        m->m[i][12] = 1;

    for(i = 12; i >=1; i--)
        m->m[24][i] = 1;

    for(i = 24; i <= 30; i++)
        m->m[i][1] = 1;

    for(i = 1; i <= 7; i++)
        m->m[30][i] = 1;

    for(i = 30; i >= 27; i--)
        m->m[i][7] = 1;

    for(i = 7; i <= 16; i++)
        m->m[27][i] = 1;

    for(i = 27; i >= 15; i--)
        m->m[i][16] = 1;

    for(i = 16; i < 20; i++)
        m->m[15][i] = 1;

    for(i = 15; i >= 5; i--)
        m->m[i][20] = 1;

    for(i = 20; i <= 24; i++)
        m->m[5][i] = 1;

    for(i = 5; i >= 2; i--)
        m->m[i][24] = 1;

    for(i = 24; i <= 29; i++)
        m->m[2][i] = 1;

    for(i = 2; i <= 14; i++)
        m->m[i][29] = 1;

    for(i = 29; i >= 23; i--)
        m->m[14][i] = 1;

    for(i = 14; i <= 18; i++)
        m->m[i][23] = 1;

    for(i = 23; i <= 27; i++)
        m->m[18][i] = 1;

    for(i = 18; i <= 22; i++)
        m->m[i][27] = 1;

    for(i = 27; i >= 23; i--)
        m->m[22][i] = 1;

    m->m[23][23] = 1;

    // insert the holes

    m->m[5][7] = 0;
    m->m[10][12] = 0;
    m->m[15][12] = m->m[16][12] = 0;
    m->m[24][12] = 0;
    m->m[24][5] = m->m[24][6] = m->m[24][1] = m->m[24][2] = 0;
    m->m[29][1] = m->m[30][1] = 0;
    m->m[30][6] = m->m[30][7] = 0;
    m->m[27][7] = 0;
    m->m[27][15] = m->m[27][16] = 0;
    m->m[23][16] = m->m[22][16] = m->m[17][16] = m->m[16][16] = m->m[15][16] = 0;
    m->m[15][18] = m->m[15][19] = m->m[15][20] = 0;
    m->m[12][20] = m->m[11][20] = m->m[6][20] = m->m[5][20] = 0;
    m->m[5][24] = m->m[5][23] = 0;
    m->m[2][24] = 0;
    m->m[2][28] = m->m[2][29] = 0;
    m->m[5][29] = m->m[7][29] = m->m[8][29] = m->m[10][29] = m->m[11][29] = m->m[13][29] = m->m[14][29] = 0;
    m->m[14][25] = m->m[14][24] = m->m[14][23] = 0;
    m->m[16][23] = m->m[17][23] = m->m[18][23] = 0;
    m->m[18][25] = m->m[18][26] = m->m[18][27] = 0;
    m->m[20][27] = m->m[21][27] = m->m[22][27] = 0;
    m->m[22][25] = m->m[22][24] = m->m[22][23] = 0;
}

void printMatrix(struct tMatrix * m)
{
    int i, j;
    for (i = 0; i < T; i++)
    {
        for (j = 0; j < T; j++)
        {
            if (m->m[i][j] == 0)
                printf("| ");
            else if(m->m[i][j] == 1)
                printf("|%c",475);
            else if((m->m[i][j] == UP) || (m->m[i][j] == DOWN) || (m->m[i][j] == RIGHT) || (m->m[i][j] == LEFT))
                printf("|%c", m->m[i][j]);
            else
                printf("|%c", 271);
        }
        printf("|\n");
    }
}

struct tAnt
{
    int x, y;
    int fitness;
    int direction; // UP 280 DOWN 281 LEFT 283 RIGHT 282
    int movements;
};

int foodAhead(struct tAnt *ant, struct tMatrix *m)
{
    int x, y;
    x = ant->x;
    y = ant->y;

    if (ant->direction == UP)
    {
        if(x == 0)
            x = T - 1;
        else
            x--;
    }
    else if(ant->direction == DOWN)
    {
        if(x == T - 1)
            x = 0;
        else
            x++;
    }
    else if(ant->direction == LEFT)
    {
        if(y == 0)
            y = T-1;
        else
            y--;
    }
    else if(ant->direction == RIGHT)
    {
        if(y == T-1)
            y = 0;
        else
            y++;
    }

    //printf("X-> %d, Y->%d \n", x, y);
    return m->m[x][y];
}

void moveAnt(struct tAnt * ant, struct tMatrix * m)
{
    int x, y;
    x = ant->x;
    y = ant->y;

    if (ant->direction == UP)
    {
        if(x == 0)
            x = T - 1;
        else
            x--;
    }
    else if(ant->direction == DOWN)
    {
        if(x == T - 1)
            x = 0;
        else
            x++;
    }
    else if(ant->direction == LEFT)
    {
        if(y == 0)
            y = T-1;
        else
            y--;
    }
    else if(ant->direction == RIGHT)
    {
        if(y == T-1)
            y = 0;
        else
            y++;
    }

    ant->x = x;
    ant->y = y;
    if(m->m[x][y] == 1)
    {
        m->m[x][y] == 0;
        ant->fitness++;
    }

    m->m[ant->x][ant->y] = PATH;
}

void rotateAnt(struct tAnt * ant, int rotateTo)
{
    if (rotateTo == LEFT)
    {
        if (ant->direction == UP)
            ant->direction = LEFT;
        else if (ant->direction == LEFT)
            ant->direction = DOWN;
        else if (ant->direction == DOWN)
            ant->direction = RIGHT;
        else if (ant->direction == RIGHT)
            ant->direction = UP;
    }
    else if (rotateTo == RIGHT)
    {
        if (ant->direction == UP)
            ant->direction = RIGHT;
        else if (ant->direction == RIGHT)
            ant->direction = DOWN;
        else if (ant->direction == DOWN)
            ant->direction = LEFT;
        else if (ant->direction == LEFT)
            ant->direction = UP;
    }
}

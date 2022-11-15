#ifndef FLAPPY_LIB_H_INCLUDED
#define FLAPPY_LIB_H_INCLUDED
#define obstaclesMAXSIZE 5

extern void clean();

extern bool title(bool);

extern bool menu(int);

extern void render(int, int, int, int, bool, int[obstaclesMAXSIZE][2], int, bool);

extern void obstacles(int, int, int, int, int[obstaclesMAXSIZE][2]);

#endif
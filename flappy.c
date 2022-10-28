/*  nombre: Flappy Cube
    autores: David Emmanuel Santana Romero y Ana Ramirez
    fecha: 29 de noviembre de 2022
    versión: 1.0
*/

// import libraries
#include<stdio.h> // standard library for input & output
#include<stdlib.h> // general standard library, needed for rand, srand, etc
#include<time.h> // time library, needed for time function
#include<conio.h> // non standard library, contains _getch function that doesn't wait for enter key when entering a character
#include<stdbool.h> // standard library for boolean logic

#define obstaclesSIZE 3 // define the number of simultaneous obstacles (or "pipes")
#define MAXHEIGHT 30 // screen height (max y) in characters
#define MAXLENGTH 30 // screen length (max x) in characters

int playerHeight = (MAXHEIGHT/2); // stores the height (y) the player is currently located at

// array of simultaneous obstacles
// array size is defined by obstaclesSIZE macro
// each element has 2 integers [x, y], which are the coordinates of the obstacle
// integer x defines the distance from origin to the obstacle
// integer y defines the height the player should pass through in order to not collide (the "aperture")
int obstaclesXHeight[obstaclesSIZE][2] = {{ 10, 12 }, { 20, 21 }, { 30, 20 }};

int points = 0; // points variable
bool gameOver = false; // whether game has ended or not

// function that renders the game's graphics (text-based)
void render() {
    // for loop that iterates to graphicate line by line (from top to bottom)
    for (int y = MAXHEIGHT; y >= 0; y--) {
        // for loop that iterates to graphicate each character of a line (from left to right)
        for (int x = 0; x < MAXLENGTH; x++) {
            // if we are graphicating coordinates [x, y] where x is 0 and y is the height the player is currently located at
            if (x == 0 && y == playerHeight) {
                printf("🐦 "); // print our player's character
            }
            // if not
            else {
                // for loop to decide whether an obstacle is present in the current coordinates or not
                for (int i = 0; i < obstaclesSIZE; i++) {
                    // if current x position is equal to the obstacle's x position
                    if (x == obstaclesXHeight[i][0]) {
                        // if height is obstacle's aperture height (y), print empty space
                        if (y == obstaclesXHeight[i][1]) printf("  ");
                        // else, if height is not obstacle's aperture height, it means the obstacle (or "pipe") is solid at these coordinates [x, y], therefore we print it
                        else printf("⬜");
                    }
                    // if not
                    else {
                        printf(" "); // print empty space
                    }
                }
            }
        }
        printf("\n"); // start new line
    }
}

void obstacles(int index) {
    if ((index+1) < obstaclesSIZE && obstaclesXHeight[index][1] != 0) {
        obstaclesXHeight[index][0] = obstaclesXHeight[(index+1)][0];
        obstaclesXHeight[index][1] = obstaclesXHeight[(index+1)][1];
    }
    else {
        obstaclesXHeight[index][0] = (MAXHEIGHT/2) + (rand() % 2);
        obstaclesXHeight[index][1] = 1 + (rand() % (MAXHEIGHT/2));
    }
}

void menu() {
    printf("\e[1;1H\e[2J");
    char menuKey = 0;
    do {
        printf("P A U S A\nPuntos: %d\n[ESC] Volver\n[Q] Salir", points);
        menuKey = _getch();
        if (menuKey == 'q') {
            gameOver = true;
            return;
        }
    } while (menuKey != 27);
}

void title() {
    puts("@@@@@@@@@@@@@@@@@@@@@@@@@@ \n     @@              @@        @@ \n   @@                @@        @@ \n@@@                  @@        @@ \n@@@                  @@        @@                                                                 @@@@@@@@@@@@@@@@@@@@@@@@ \n@@@        @@@@@@@@@@@@        @@  @@-----------------@@--------------@@@@  @@--------------@@@@  @@*          @@        @ \n@@@                  @@        @@@@                   @@                  @@@@                  @@@@*          @@        @ \n@@@                  @@        @@                     @@                    @@                    @@*          @@        @ \n@@@                  @@        @@                     @@                    @@                    @@*          @@        @ \n@@@                  @@        @@          @@         @@          @@        @@          @@        @@*          @@        @ \n@@@ ****** @@@@@@@@@@@@ ****** @@ ******** @@  *****  @@ ******** @@ ****** @@ ******** @@ ****** @@* ******** @@ ****** @ \n@@@ ****** @@@@@@@@@@@@ ****** @@ ******** @@  *****  @@ ******** @@ ****** @@ ******** @@ ****** @@* ****************** @ \n@@@ ****** @@        @@ ****** @@ ******** @@  *****  @@ ******** @@ ****** @@ ******** @@ ****** @@*   **************** @ \n@@@ ****** @@        @@ ****** @@ ******************  @@ ****************** @@ ****************** @@@@@@@ ************** @ \n@@@ ****** @@        @@ ****** @@ ******************  @@ ****************** @@ ****************** @@@ #@@@@ ************ @ \n@@@ ****** @@        @@ ****** @@@@   **************  @@ **************   @@@@ **************   @@@@@    @@@@@@@@ ****** @ \n@@@        @@        @@        @@@@@@                 @@ ********       @@@@@@ ********       @@@@         @@@@@@ ****** @ \n@@@@@@@@@@@@@        @@@@@@@@@@@@    @@@@@@@@@@@@@@@@@@@ ******** @@@@@@    @@ ******** @@@@@@             @@ ********** @ \n                                                      @@ ******** @@        @@ ******** @@                 @@ ******** @@@ \n                                                      @@          @@        @@          @@                 @@        @@@@ \n                                                      @@@@@@@@@@@@@@        @@@@@@@@@@@@@@                 @@@@@@@@@@@@ \n@@@@@@@@@@@@@@@@@@                                               @@@@@@@@@@@@@@ \n@@                @@                                             @@          @@ \n@@                  @@                                           @@          @@ \n@@                    @@                                         @@          @@ \n@                    @@@@@@@@@@@@@@@                            @@          @@ \n@          @@        @@           @@  @@              @@  @@                @@ \n@                    @@           @@@@                @@@@                  @@ \n@                    @@           @@                  @@                    @@ \n@                    @@           @@                  @@                    @@ \n@                    @@           @@          @@@@@@@@@@        @@          @@ \n@ ****************** @@ .*******  @@ ****** @@@@@@@@@@@@ ****** @@ ******** @@ \n@ ******** @@ ****** @@ .*******  @@ ****** @@@@      @@ ****** @@ ******** @@ \n@ ******** @@ ****** @@ .*******  @@ ****** @@        @@ ****** @@ ******** @@ \n@ ****************** @@ .*******  @@ ****** @@        @@ ****************** @@ \n@ ****************** @@ .*******  @@ ****** @@        @@ ****************** @@ \n@ **************   @@@@ .*******  @@ ****** @@        @@@@   ************** @@ \n@                @@@@@@           @@        @@          @@@@                @@ \n@@@@@@@@@@@@@@@@@    @@@@@@@@@@@@@@@@@@@@@@@@@              @@@@@@@@@@@@@@@@@@ \n@");
    puts("Presiona cualquier tecla para comenzar.");
}

int main() {
    title();
    srand(time(NULL));
    obstacles(0);
    while (!gameOver) {
        char keypress = _getch();
        if (keypress == 0) continue;
        switch (keypress) {
            case 27:
                menu();
                break;
            case 72:
            case 32:
            case 119:
                if (playerHeight < MAXHEIGHT) playerHeight++;
                break;
            case 80: 
                if (playerHeight > 0) playerHeight--;
                break;
            default:
                break;
        }
        printf("\e[1;1H\e[2J");
        render();
        for (int i = 0; i < obstaclesSIZE; i++) {
            if (obstaclesXHeight[i][0] == 0) {
                if (obstaclesXHeight[i][1] == playerHeight) {
                    points++;
                    obstacles(i);
                }
                else {
                    printf("\e[1;1H\e[2J");
                    puts("GAME OVER");
                    gameOver = true;
                    break;
                }
            }
            obstaclesXHeight[i][0]--;
        }
    }
    printf("Puntos: %d", points);
    return 0;
}
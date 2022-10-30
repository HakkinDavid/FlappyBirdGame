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
bool turbo = false;

// array of simultaneous obstacles
// array size is defined by obstaclesSIZE macro
// each element has 2 integers [x, y], which are the coordinates of the obstacle
// integer x defines the distance from origin to the obstacle
// integer y defines the height the player should pass through in order to not collide (the "aperture")
int obstaclesXY[obstaclesSIZE][2] = {{ 10, 12 }, { 20, 21 }, { 30, 20 }};

int points = 0; // points variable
bool gameOver = false; // whether game has ended or not

// function that standarizes screen cleaning across our code so we can easily change it in the future
void clean() {
    printf("\e[1;1H\e[2J"); // <-- this is a hack-ish way to clean screen
}

// function that renders the game's graphics (text-based)
void render() {
    // clear the screen before rendering
    clean();
    printf("Puntos: %d\t", points);
    turbo ? printf("Turbo <ON> [T]\n") : printf("Turbo <off> [T]\n");
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
                    if (x == obstaclesXY[i][0]) {
                        // if height is obstacle's aperture height (y), print empty space
                        if (y == obstaclesXY[i][1]) printf("  ");
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

// function that calculates obstacle's data
void obstacles(int index) {
    obstaclesXY[index][0] = MAXLENGTH;
    do {
        int randomRange = (rand() % 3);
        if (rand() % 2) randomRange *= -1;
        obstaclesXY[index][1] += randomRange;
    } while (obstaclesXY[index][1] > MAXHEIGHT || obstaclesXY[index][1] < 0 || obstaclesXY[index][1] == playerHeight);
}

void menu() {
    char menuKey = 0;
    do {
        clean();
        printf("P A U S A\nPuntos: %d\n[ESC] Volver\n[Q] Salir\n", points);
        menuKey = _getch();
        if (menuKey == 'q') {
            gameOver = true;
            return;
        }
    } while (menuKey != 27);
    render();
}


void title() {
    puts("\n  ______ _                           _______        _   ____  _         _ \n |  ____| |                         |__   __|      | | |  _ \\(_)       | |\n | |__  | | __ _ _ __  _ __  _   _     | | _____  _| |_| |_) |_ _ __ __| |\n |  __| | |/ _` | '_ \\| '_ \\| | | |    | |/ _ \\ \\/ / __|  _ <| | '__/ _` |\n | |    | | (_| | |_) | |_) | |_| |    | |  __/>  <| |_| |_) | | | | (_| |\n |_|    |_|\\__,_| .__/| .__/ \\__, |    |_|\\___/_/\\_\\___|____/|_|_|  \\__,_|\n                | |   | |     __/ |                                       \n                |_|   |_|    |___/                                        \n");
    puts("Controles\n\tSubir [🡡, Espacio, W]\n\tAvanzar [🡢, D]\n\tBajar [🡣, S]\n\tActivar o desactivar turbo [T]\tNota: Pierdes un turno\n\t[ESC] Menú");
    puts("Presiona cualquier tecla para comenzar.");
    _getch();
}

int main() {
    title();
    srand(time(NULL));
    for (int i = 0; i < obstaclesSIZE; i++) {
        obstacles(i);
        obstaclesXY[i][0] /= obstaclesSIZE;
        obstaclesXY[i][0] *= (i+1);
    }
    render();
    while (!gameOver) {
        char keypress = _getch();
        if (keypress == 0) continue;
        switch (keypress) {
            case 27:
                menu();
                continue;
                break;
            case 72:
            case 32:
            case 119:
                if (playerHeight < MAXHEIGHT) playerHeight++;
                if (turbo) playerHeight++;
                break;
            case 115:
            case 80: 
                if (playerHeight > 0) playerHeight--;
                if (turbo) playerHeight--;
                break;
            case 77:
            case 100:
                break;
            case 116:
                turbo = !turbo;
                break;
            default:
                continue;
                break;
        }
        for (int i = 0; i < obstaclesSIZE; i++) {
            obstaclesXY[i][0]--;
            if (obstaclesXY[i][0] == 0) {
                if (obstaclesXY[i][1] == playerHeight) {
                    points++;
                    obstacles(i);
                }
                else {
                    clean();
                    puts("GAME OVER");
                    gameOver = true;
                    break;
                }
            }
        }
        if (!gameOver) render();
    }
    printf("Puntos: %d", points);
    return 0;
}
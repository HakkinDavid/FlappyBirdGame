#include "flappy_lib.h"
#include <stdio.h> // standard input and output library
#include <stdlib.h> // general standard library, needed for rand, srand, etc
#include <time.h> // time library, needed for time function
#include <stdbool.h> // standard boolean library
#include <conio.h> // non standard library, contains _getch function that doesn't wait for enter key when entering a character

#define obstaclesMAXSIZE 5

// function that standarizes screen cleaning across our code so we can easily change it in the future
void clean() {
    #ifdef WINDOWS
        system("cls"); // clear windows console
    #else
        printf("\e[1;1H\e[2J"); // <-- this is a hack-ish way to clean screen, using a regular expression to fill the entire screen with blank spaces
    #endif
}

// function that renders a nice title screen
bool title(bool plainText) {
    clean(); // clean screen
    // flappy bird ascii art
    puts("\n  ______ _                           _______        _   ____  _         _ \n |  ____| |                         |__   __|      | | |  _ \\(_)       | |\n | |__  | | __ _ _ __  _ __  _   _     | | _____  _| |_| |_) |_ _ __ __| |\n |  __| | |/ _` | '_ \\| '_ \\| | | |    | |/ _ \\ \\/ / __|  _ <| | '__/ _` |\n | |    | | (_| | |_) | |_) | |_| |    | |  __/>  <| |_| |_) | | | | (_| |\n |_|    |_|\\__,_| .__/| .__/ \\__, |    |_|\\___/_/\\_\\___|____/|_|_|  \\__,_|\n                | |   | |     __/ |                                       \n                |_|   |_|    |___/                                        \n");
    // print controls
    printf("Controles\n\tSubir [%s, Espacio, W]\n\tAvanzar [%s, D]\n\tBajar [%s, S]\n\tActivar o desactivar turbo [T]\tNota: Pierdes un turno\n\t[ESC] Menú\n", (plainText ? "Flechita hacia arriba" : "🡡"), (plainText ? "Flechita hacia la derecha" : "🡢"), (plainText ? "Flechita hacia abajo" : "🡣"));
    // let them know how to get started
    puts("Presiona cualquier tecla para comenzar. Si el menú o el juego no se muestran correctamente, presiona 0.");
    char ch = _getch(); // wait for user to press a key
    // if 0 is pressed, we'll switch emoji usage
    if (ch == '0') {
        plainText = title(!plainText); // re run title screen
    }
    return plainText; // expose whether we choose to render game in plain text or not
}

// function for pause menu
bool menu(int points) {
    char menuKey = 0; // variable to handle keyboard on menu
    do {
        clean(); // clean screen
        printf("P A U S A\nPuntos: %d\n[ESC] Volver\n[Q] Salir\n", points); // print menu
        menuKey = _getch(); // wait for user to select an option
        if (menuKey == 'q') { // if q is pressed
            // end game & exit
            return true; // this value should be handled by a game over flag variable
        }
    } while (menuKey != 27); // menu can be exited with ESC
    return false; // exit menu and continue game
}

// function that renders the game's graphics (text-based)
void render(int length, int height, int playerHeight, int points, bool turbo, int obstaclesXY[obstaclesMAXSIZE][2], int obstaclesSIZE, bool plainText) {
    // clear the screen before rendering
    clean();
    
    // for loop that iterates to graphicate line by line (from top to bottom)
    for (int y = height; y >= 0; y--) {
        // for loop that iterates to graphicate each character of a line (from left to right)
        for (int x = 0; x < length; x++) {
            // if we are graphicating coordinates [x, y] where x is 0 and y is the height the player is currently located at
            if (x == 0 && y == playerHeight) {
                plainText ? printf("P  ") : printf("\U0001F426 "); // print our player's character
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
                        else plainText ? printf("B") : printf("\U00002B1C");
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

    // print points
    printf("Puntos: %d\t", points);
    // print turbo status
    turbo ? printf("Turbo <ON> [T]") : printf("Turbo <off> [T]");
}

// function that calculates an obstacle's data, based on the provided index
void obstacles(int index, int length, int height, int playerHeight, int obstaclesXY[obstaclesMAXSIZE][2]) {
    srand(time(NULL)); // set random seed based on time
    // spawn obstacle at the end of screen to create an endless map illusion
    obstaclesXY[index][0] = length;
    // DO set the obstacle's aperture height, relative to previous aperture height to ensure game is not plain impossible
    do {
        int randomDiff = 1 + (rand() % 3); // aperture height can vary up to 3 units
        if (rand() % 2) randomDiff *= -1; // aperture height might be above or below previous value
        obstaclesXY[index][1] += randomDiff; // set the aperture height to the previous aperture height plus the calculated random difference
    } while (obstaclesXY[index][1] > height || obstaclesXY[index][1] < 0 || obstaclesXY[index][1] == playerHeight); // ensure aperture height is reachable (0 to MAXHEIGHT) and that it is not player's current height -- because we'd like them to suffer :)
}
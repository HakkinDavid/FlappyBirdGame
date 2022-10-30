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
bool turbo = false; // whether turbo movement is activated or not

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
    // print points
    printf("Puntos: %d\t", points);
    // print turbo status
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

// function that calculates an obstacle's data, based on the provided index
void obstacles(int index) {
    // spawn obstacle at the end of screen to create an endless map illusion
    obstaclesXY[index][0] = MAXLENGTH;
    // DO set the obstacle's aperture height, relative to previous aperture height to ensure game is not plain impossible
    do {
        int randomDiff = 1 + (rand() % 3); // aperture height can vary up to 3 units
        if (rand() % 2) randomDiff *= -1; // aperture height might be above or below previous value
        obstaclesXY[index][1] += randomDiff; // set the aperture height to the previous aperture height plus the calculated random difference
    } while (obstaclesXY[index][1] > MAXHEIGHT || obstaclesXY[index][1] < 0 || obstaclesXY[index][1] == playerHeight); // ensure aperture height is reachable (0 to MAXHEIGHT) and that it is not player's current height -- because we'd like them to suffer :)
}

// function for pause menu
void menu() {
    char menuKey = 0; // variable to handle keyboard on menu
    do {
        clean(); // clean screen
        printf("P A U S A\nPuntos: %d\n[ESC] Volver\n[Q] Salir\n", points); // print menu
        menuKey = _getch(); // wait for user to select an option
        if (menuKey == 'q') { // if q is pressed
            gameOver = true; // end game
            return; // exit menu
        }
    } while (menuKey != 27); // menu can be exited with ESC
    render(); // if user exits menu, render the game once again
}

// function that renders a nice title screen
void title() {
    // flappy bird ascii art
    puts("\n  ______ _                           _______        _   ____  _         _ \n |  ____| |                         |__   __|      | | |  _ \\(_)       | |\n | |__  | | __ _ _ __  _ __  _   _     | | _____  _| |_| |_) |_ _ __ __| |\n |  __| | |/ _` | '_ \\| '_ \\| | | |    | |/ _ \\ \\/ / __|  _ <| | '__/ _` |\n | |    | | (_| | |_) | |_) | |_| |    | |  __/>  <| |_| |_) | | | | (_| |\n |_|    |_|\\__,_| .__/| .__/ \\__, |    |_|\\___/_/\\_\\___|____/|_|_|  \\__,_|\n                | |   | |     __/ |                                       \n                |_|   |_|    |___/                                        \n");
    // print controls
    puts("Controles\n\tSubir [🡡, Espacio, W]\n\tAvanzar [🡢, D]\n\tBajar [🡣, S]\n\tActivar o desactivar turbo [T]\tNota: Pierdes un turno\n\t[ESC] Menú");
    // let them know how to get started
    puts("Presiona cualquier tecla para comenzar.");
    _getch(); // wait for user to press a key
}

// main game function
int main() {
    title(); // render title screen
    srand(time(NULL)); // set random seed based on time
    
    //the following for loop generates the original obstacles
    for (int i = 0; i < obstaclesSIZE; i++) {
        obstacles(i); // call obstacles function over the current obstacle
        // put obstacles sort of evenly across screen
        obstaclesXY[i][0] /= obstaclesSIZE;
        obstaclesXY[i][0] *= (i+1);
    }

    render(); // render game for the first time
    // gameplay (while) loop, repeats as long as game has not ended
    while (!gameOver) {
        char keypress = _getch(); // wait for user to press a key
        if (keypress == 0) continue; // if NULL is received, ignore it
        switch (keypress) {
            // if esc key is pressed
            case 27:
                menu(); // go to pause menu
                continue; // don't waste a turn
                break;

            // if up arrow, space bar or W is pressed
            case 72:
            case 32:
            case 119:
                // as long as player hasn't reached the top of screen
                if (playerHeight < MAXHEIGHT) {
                    playerHeight++; // move up by 1
                    if (turbo) playerHeight++; // if turbo, move up once again
                }
                break;

            // if down arrow or S is pressed
            case 115:
            case 80:
                // as long as player hasn't reached the bottom of screen
                if (playerHeight > 0) {
                    playerHeight--; // move down by 1
                    if (turbo) playerHeight--; // if turbo, move down once again
                }
                break;

            // if right arrow or D is pressed, allow for game to advance a turn
            case 77:
            case 100:
                break;

            // if T is pressed, switch turbo (on/off)
            case 116:
                turbo = !turbo;
                break;
            
            // else, ignore input
            default:
                continue;
                break;
        }

        // the following for loop iterates through obstacles' object
        for (int i = 0; i < obstaclesSIZE; i++) {
            obstaclesXY[i][0]--; // update position on screen (decrease x by 1), this creates the illusion of player movement

            // if an obstacle has reached x = 0 (where player is located)
            if (obstaclesXY[i][0] == 0) {
                // if obstacle's aperture height matches player current height, it means it has been successfully dodged
                if (obstaclesXY[i][1] == playerHeight) {
                    // therefore
                    points++; // increase player points by 1
                    obstacles(i); // regenerate this obstacle
                }
                // if player is at a different height than obstacle's aperture height, it means player HAS COLLIDED
                // HOUSTON, WE'VE GOT A PROBLEM!!!
                else {
                    clean(); // clear screen
                    puts("GAME OVER"); // game ends here
                    gameOver = true; // die.
                    break; // no need to waste resources for a player that clearly cannot play this game correctly, escaping for loop
                }
            }
        }
        if (!gameOver) render(); // if game is still going on, render it on screen.
    }
    clean(); // clear screen
    printf("Puntos: %d", points); // once game ends, print final score
    return 0; // exit program without errors
}
/*  nombre: Flappy Cube
    autores: David Emmanuel Santana Romero y Ana Ramirez
    fecha: 29 de noviembre de 2022
    versión: 1.0
*/

// import libraries
#include <stdio.h> // standard library for input & output
#include <conio.h> // non standard library, contains _getch function that doesn't wait for enter key when entering a character
#include <stdbool.h> // standard library for boolean logic
#include <locale.h> // localization library
#include "flappy_lib.h" // game's own library
#include <windows.h> // windows library which allows us to set the console text properties

#define obstaclesSIZE 3 // define the number of simultaneous obstacles (or "pipes")
#define MAXHEIGHT 30 // screen height (max y) in characters
#define MAXLENGTH 30 // screen length (max x) in characters

int playerHeight = (MAXHEIGHT/2); // stores the height (y) the player is currently located at
bool turbo = false; // whether turbo movement is activated or not
bool plainText = false;

// array of simultaneous obstacles
// array size is defined by obstaclesSIZE macro
// each element has 2 integers [x, y], which are the coordinates of the obstacle
// integer x defines the distance from origin to the obstacle
// integer y defines the height the player should pass through in order to not collide (the "aperture")
int obstaclesXY[obstaclesSIZE][2] = {{ 10, 12 }, { 20, 21 }, { 30, 20 }};

int points = 0; // points variable
bool gameOver = false; // whether game has ended or not

// main game function
int main() {
    setlocale(LC_ALL, "es_MX"); // set our locals to mexican spanish
    SetConsoleOutputCP(65001); // set console output to support emoji
    plainText = title(plainText); // render title screen
    
    //the following for loop generates the original obstacles
    for (int i = 0; i < obstaclesSIZE; i++) {
        obstacles(i, MAXLENGTH, MAXHEIGHT, playerHeight, obstaclesXY); // call obstacles function over the current obstacle
        // put obstacles sort of evenly across screen
        obstaclesXY[i][0] /= obstaclesSIZE;
        obstaclesXY[i][0] *= (i+1);
    }

    render(MAXLENGTH, MAXHEIGHT, playerHeight, points, turbo, obstaclesXY, obstaclesSIZE, plainText); // render game for the first time
    // gameplay (while) loop, repeats as long as game has not ended
    while (!gameOver) {
        char keypress = _getch(); // wait for user to press a key
        if (keypress == 0) continue; // if NULL is received, ignore it
        switch (keypress) {
            // if esc key is pressed
            case 27:
                gameOver = menu(points); // go to pause menu and expect user's choice whether to end game or not
                render(MAXLENGTH, MAXHEIGHT, playerHeight, points, turbo, obstaclesXY, obstaclesSIZE, plainText); // re render game
                continue; // don't waste a turn
                break;

            // if up arrow, space bar or W is pressed
            case 72:
            case 32:
            case 119:
                // as long as player hasn't reached the top of screen
                if (playerHeight < MAXHEIGHT) {
                    playerHeight++; // move up by 1
                    if (turbo && playerHeight < MAXHEIGHT) playerHeight++; // if turbo, move up once again
                }
                break;

            // if down arrow or S is pressed
            case 115:
            case 80:
                // as long as player hasn't reached the bottom of screen
                if (playerHeight > 0) {
                    playerHeight--; // move down by 1
                    if (turbo && playerHeight > 0) playerHeight--; // if turbo, move down once again
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
                    obstacles(i, MAXLENGTH, MAXHEIGHT, playerHeight, obstaclesXY); // regenerate this obstacle
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
        if (!gameOver) render(MAXLENGTH, MAXHEIGHT, playerHeight, points, turbo, obstaclesXY, obstaclesSIZE, plainText); // if game is still going on, render it on screen.
    }
    clean(); // clear screen
    printf("Puntos: %d\n", points); // once game ends, print final score

    // allow for user to exit nicely
    puts("Presiona ESC para salir.");

    // exit program just until esc key is pressed
    char exitChar = 0;
    do {
        exitChar = _getch();
    } while (exitChar != 27);
    
    return 0; // exit program without errors
}
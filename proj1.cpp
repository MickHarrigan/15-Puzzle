/*****************************************
** File:    Proj1.cpp
** Project: CMSC 202 Project 1, Spring 2020
** Author:  Michael Harrigan
** Date:    2/25/20
** Section: 34
** E-mail:  mharrig1@umbc.edu 
**
** File contains the methods and functionality for Project 1.
** Takes the user input to either load a board, or create one itself.
** Then it displays the board it created, and prompts the user to
** take moves at solving the board.
**
**
***********************************************/

#include <iostream>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <cstdlib>
using namespace std;

// Constants for use in the beginning of the program
const int FIRST_INDEX = 0;              // first index for use in the boolean flags for boundaries of playerTurn()

const int YES = 1;                      // user input answer for loading a board from a file in introBoardReading()
const int NO = 2;                       // user input to load the default board in introBoardReading()

const int MAX_LENGTH_FILE = 80;         // arbitrary length of a filename that can be input

const int WIDTH_TOP = 9;                // setw() measurements to match that of the provided outputs for the project, used on the first column title
const int WIDTH_MID = 6;                // spacing for all the middle cells and between each place
const int WIDTH_LEFT = 5;               // distance from the each of the interior values in the board
const int WIDTH_SHORT_LEFT = 4;         // distance from the row header to the first item in the row

const char SEPARATOR_CHAR = '|';         // determines the end of a row value, as well as aesthetically pleasing for the formatting

const int LEN_ROW_BOARD = 4;            // amount of the rows that the board will use
const int LEN_COL_BOARD = 4;            // amount of columns that the board will use
const int EMPTY_SPACE = 0;              // the empty space that the user is looking for to swap with

//-------------------------------------------------------------------------------------------------
//  Name: introBoardReading
//  Pre-condition: boardChoice must be an integer
//                 gameBoard must be a defined array of certain length and width
//  Post-condition: Edits the board that is to be used, either from the default or the file
//-------------------------------------------------------------------------------------------------

void introBoardReading(int boardChoice, int gameBoard[][LEN_COL_BOARD]);


//-------------------------------------------------------------------------------------------------
//  Name: readBoard
//  Pre-condition: Variable for the file is initialized as a string or a c-string
//                 2 dimensional array for inputting the values from the file
//  Post-condition: Updated array with real values
//-------------------------------------------------------------------------------------------------

void readBoard(char board[MAX_LENGTH_FILE], int array[][LEN_COL_BOARD]);


//-------------------------------------------------------------------------------------------------
//  Name: dispBoard
//  Pre-condition: Array is defined, filled with values
//  Post-condition: Array is printed for the user to see and interact with
//-------------------------------------------------------------------------------------------------

void dispBoard(int gameBoard[][LEN_COL_BOARD]);


//-------------------------------------------------------------------------------------------------
//  Name: checkGameOver
//  Pre-condition: Board only contains legal values
//  Post-condition: Checks the board against a created correct board, ends early if not matching
//-------------------------------------------------------------------------------------------------

bool checkGameOver(int gameBoard[][LEN_COL_BOARD]);


//-------------------------------------------------------------------------------------------------
//  Name: playerTurn
//  Pre-condition: Array is printable and filled with integers
//  Post-condition: Moves the point selected by the user, altering the board
//-------------------------------------------------------------------------------------------------

void playerTurn(int gameBoard[][LEN_COL_BOARD]);


int main(){
    char retry = 'y';                                       // determines if the game will be replayed or not, initialized to yes
    while (retry != 'n'){
        int gameArray[LEN_ROW_BOARD][LEN_COL_BOARD];        // array that defines the board
        bool winCondition;                                  // boolean for checking if the game has ended
        int boardChoice;                                    // user input for either default or custom loaded board

        // Introduction to the game
        cout << "Welcome to the game Fifteen!" << endl;
        cout << "Would you like to load a board?\n" << "1. Yes\n" << "2. No" << endl;
        // user input for the desired board to be loaded
        cin >> boardChoice;

        // prepares the board based on the user input
        introBoardReading(boardChoice, gameArray);

        // displays board, prints "No win.", and asks for a move until the game is found to be over
        do{
            dispBoard(gameArray);
            if (winCondition == true){
                cout << "No win." << endl;
            } // end of if statement

            playerTurn(gameArray);
            winCondition = checkGameOver(gameArray);
            }while (winCondition == true); // end of do while loop
        
        // displays the winning board and prints out the winning statement
        dispBoard(gameArray);
        cout << "Congrats you've won!" << endl;

        // takes the user input to rerun the game from the beginning or to end the code
        cout << "Play again? (y/n)" << endl;
        cin >> retry;
        while ((retry != 'y') and (retry != 'n')){
            cout << "Enter either y or n." << endl;
            cout << "Play again? (y/n)" << endl;
            cin >> retry;
        } // end of while loop
    
    } // end of the while loop

    // farewell statement of the code.
    cout << "Thank you for playing Fifteen!" << endl;

} // end of main

void introBoardReading(int boardChoice, int gameBoard[][LEN_COL_BOARD]){ // Function 1, loads the default or takes the input of a loaded board

    // Initializes the default board for the game, based on the parameters of the amount of rows and columns.
    int num = 1;    // used to fill in the rows and columns starting with 1 -> 1 less than the total cells
    for (int i = 0; i < LEN_ROW_BOARD; i++){
        for (int j = 0; j < LEN_COL_BOARD; j++){
            if ((i == LEN_ROW_BOARD - 1) and (j == LEN_COL_BOARD - 2)){
                gameBoard[i][j] = EMPTY_SPACE;
                } // end of if statement
            
            else{
                gameBoard[i][j] = num;
                num++;
                } // end of else statement

            } // end of (inner) for loop

        } // end of (outer) for loop
    
    char playerFile[MAX_LENGTH_FILE];   // user inputted file name stored in this
    bool fileLoaded = false;            // used to show that the board is loaded either from default or via a file

    while (fileLoaded != true){
        switch (boardChoice){
            case YES:
                // if the user wants to load a file runs this case
                cout << "What is the file name?" << endl;
                cin >> playerFile;
                readBoard(playerFile, gameBoard);
                fileLoaded = true;
                break;

            case NO:
                // if no file is needed to be loaded, a default board is created and loaded
                cout << "Loading default" << endl;
                fileLoaded = true;
                break;

            default:
                // if something other than a 1 or 2 is input, then the user is reprompted
                cout << "Enter either 1 or 2 please." << endl;
                cin >> boardChoice;
                fileLoaded = false;
                break;
        } // end of switch statement

    } // end of while loop

} // end of Function 1

void readBoard(char board[], int array[][LEN_COL_BOARD]){ // Function 2, given a name, opens the file and creates a board to use from it 
    // opens the board, using the file name given by the user and puts each value into the supplied array
    ifstream boardFile(board);
    if (boardFile.is_open()){
        for (int i = 0; i < LEN_ROW_BOARD; i++){
            for (int j = 0; j < LEN_COL_BOARD; j++){
                boardFile >> array[i][j];
            } // end of (inner) for loop

        } // end of (outer) for loop

    } // end of if statement

    boardFile.close();
    
} // end of Function 2

void dispBoard(int gameBoard[][LEN_COL_BOARD]){ // Function 3, using the supplied board, displays it and the row and column headers

    // prints the column headers using setw to separate them
    for (int i = 0; i < LEN_COL_BOARD; i++){
        if (i == 0){
            cout << setw(WIDTH_TOP) << i + 1;
        } // end of if

        else{
            cout << setw(WIDTH_MID) << i + 1;
            if (i == LEN_COL_BOARD - 1){
                cout << endl;
            } // end of inner if statement

        } // end of else statement

    } // end of for loop

    // prints the row headers, plus the values of the array, and the separator characters
    // uses setW function to space out the values the same as the column headers above them
    for (int j = 0; j < LEN_ROW_BOARD; j++){
        cout << setw(WIDTH_LEFT) << j + 1 << SEPARATOR_CHAR;
        for (int x = 0; x < LEN_COL_BOARD; x++){
            if (x == 0){
                cout << setw(WIDTH_SHORT_LEFT) << gameBoard[j][x] << SEPARATOR_CHAR;
            } // end of if statement

            else{
                cout << setw(WIDTH_LEFT) << gameBoard[j][x] << SEPARATOR_CHAR;
                if (x == LEN_COL_BOARD - 1){
                    cout << endl;
                } // end of if statement

            } // end of else statement

        } // end of (inner) for loop

    } // end of (outer) for loop

} // end of Function 3

bool checkGameOver(int gameBoard[][LEN_COL_BOARD]){ // Function 4, checks if the game has ended or not
    int correctBoard[LEN_ROW_BOARD][LEN_COL_BOARD];     // an empty array that will be made into the "correct" array to be checked against

    int num = 1;                                        // counter variable to fill each of the cells throughout the nested for loop below
    for (int i = 0; i < LEN_ROW_BOARD; i++){
        for (int j = 0; j < LEN_COL_BOARD; j++){
            if ((i == LEN_ROW_BOARD - 1) and (j == LEN_COL_BOARD - 1)){
                correctBoard[i][j] = EMPTY_SPACE;
                } // end of if statement
            
            else{
                correctBoard[i][j] = num;
                num++;
                } // end of else statement

            } // end of (inner) for loop

        } // end of (outer) for loop

    // compares the array used for the game to that of the correct board, made above
    // if there is a point in wihch they do not match exactly, the function ends returning true, else it returns false where the game then ends
    for (int i = 0; i < LEN_ROW_BOARD; i++){
        for (int j = 0; j < LEN_COL_BOARD; j++){
            if (correctBoard[i][j] != gameBoard[i][j]){
                return true;
                } // end of if statement

            } // end of (inner) for loop

        } // end of (outer) for loop
    
    return false;

} // end of Function 4

void playerTurn(int gameBoard[][LEN_COL_BOARD]){ // Function 5, takes the inputs of the player and moves the selected point to the blank
    int rowInput = 0;       // user input for the row
    int colInput = 0;       // user input for the column

    // prompts the user for the row and column inputs, where input checkers also fit in
    cout << "What is the row (left side) and column (top) to slide?" << endl;
        cin >> rowInput;
        while ((rowInput < 1) or (rowInput > LEN_ROW_BOARD)){
            cout << "Please enter a valid response for the row." << endl;
            cin >> rowInput;
        } // end of while loop
    
        cin >> colInput;
        while ((colInput < 1) or (colInput > LEN_COL_BOARD)){
            cout << "Please enter a valid response for the column." << endl;
            cin >> colInput;
        } // end of while loop

    // if the selected space is an empty space (0), then tells the user of the mistake, reprints the board and reasks the user for inputs
    while (gameBoard[rowInput][colInput] == EMPTY_SPACE){
        cout << "You chose the blank" << endl;
        cout << "That tile cannot slide" << endl;

        dispBoard(gameBoard);

        cout << "What is the row (left side) and column (top) to slide?" << endl;
        cin >> rowInput;
        while ((rowInput < 1) or (rowInput > LEN_ROW_BOARD)){
            cout << "Please enter a valid response for the row." << endl;
            cin >> rowInput;
        } // end of while loop
    
        cin >> colInput;
        while ((colInput < 1) or (colInput > LEN_COL_BOARD)){
            cout << "Please enter a valid response for the column." << endl;
            cin >> colInput;
        } // end of while loop

    } // end of while loop

    int temp;                   // temporary variable used to swap the values of 2 cells
    rowInput = rowInput - 1;    // converts the variable to the viewpoint of the index, not of the user as it was before
    colInput = colInput - 1;    // converts to the index POV
    
    bool checkDown = true;      // boolean for checking below the input point
    bool checkUp = true;        // boolean for checking above the input point
    bool checkLeft = true;      // boolean for checking left of the input point
    bool checkRight = true;     //  boolean for checkign right of the input point

    // if the row is the first, flips checkUp, stopping the check above the input point
    if (rowInput == FIRST_INDEX){
        checkUp = false;
    } // end of if statement
    
    // if the row is the last, flips checkDown, stopping the check below the input point
    if (rowInput == LEN_ROW_BOARD - 1){
        checkDown = false;
    } // end of if statement
    
    // if the column is the first, flips checkLeft, stopping the check left of the input point
    if (colInput == FIRST_INDEX){
        checkLeft = false;
    } // end of if statement
    
    // if the column is the last, flips checkRight, stopping the check right of the input point
    if (colInput == LEN_COL_BOARD - 1){
        checkRight = false;
    } // end of if statement

    // the usage of ifs and not else if is to allow the corners to flip more than one of the bools, stopping the check out of bounds in both directions

    if ((gameBoard[rowInput + 1][colInput] == EMPTY_SPACE) and (checkDown == true)){
        // if the point directly below the input point is a zero, and the checkDown flag is still true, flips the zero and the chosen place
        temp = gameBoard[rowInput + 1][colInput];
        gameBoard[rowInput + 1][colInput] = gameBoard[rowInput][colInput];
        gameBoard[rowInput][colInput] = temp;
        cout << "The tile slid down" << endl;

    } // end of if statement

    if ((gameBoard[rowInput - 1][colInput] == EMPTY_SPACE) and (checkUp == true)){
        // if the point directly above the input point is a zero, and the checkUp flag is still true, flips the zero and the chosen place
        temp = gameBoard[rowInput - 1][colInput];
        gameBoard[rowInput - 1][colInput] = gameBoard[rowInput][colInput];
        gameBoard[rowInput][colInput] = temp;
        cout << "The tile slid up" << endl;

    } // end of if statement

    if ((gameBoard[rowInput][colInput + 1] == EMPTY_SPACE) and (checkRight == true)){
        // if the point directly right of the input point is a zero, and the checkRight flag is still true, flips the zero and the chosen place
        temp = gameBoard[rowInput][colInput + 1];
        gameBoard[rowInput][colInput + 1] = gameBoard[rowInput][colInput];
        gameBoard[rowInput][colInput] = temp;
        cout << "The tile slid right" << endl;

    } // end of if statement

    if ((gameBoard[rowInput][colInput - 1] == EMPTY_SPACE) and (checkLeft == true)){
        // if the point directly left of the input point is a zero, and the checkLeft flag is still true, flips the zero and the chosen place
        temp = gameBoard[rowInput][colInput - 1];
        gameBoard[rowInput][colInput - 1] = gameBoard[rowInput][colInput];
        gameBoard[rowInput][colInput] = temp;
        cout << "The tile slid left" << endl;

    } // end of if statement

} // end of Function 5
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct game//Struct to save in file
{
    int id;
    int playerTurn;
    char *Player1;
    char *Player2;
    char *grid[6];
};
static struct game loadedGames[100];
struct position//Struct to show position of a cell
{
    int column;
    int row;
};
struct position gridWin[4];// Used in fillwinboard,clearY and saveY to save the position of the win
char grid[7][6];//A board is stored here
static char playerOne[20];
static char playerTwo[20];
static int playerTurn;
static int gameId = 0;

//Function declarations
void startGame();//function to redirect to other functions
void drawBoard();//draw board
int mainMenu();//Check if input is valid if not return error message
int loadMenu();//Run load menu
void playNewGame();//stard new game
void clearBoard();//clear board from any "x" "o"
int fillBoard(int col, int playTurn);//Writes the playermark in the chosen column
int checkWin(int column, int row, char playMark);//check if player won
int checkWinByDiag1(int column, int row, int playMark);//Checks if a player won by Top Left-Bottom Right diagonal
int checkWinByDiag2(int column, int row, int playMark);//Checks if a player won by Top Right-Bottom left diagonal
int checkWinByRow(int row, int palyMark);//Checks if a player won by row
int checkWinByColumn(int column, char playMark);//Checks if a player won by column
void Aftermenu();//Menu after a game is finished
int getStartRow(int col, int row);//calculate starting row from column and row
int getStartCol(int col, int row);//calculate starting column from column and row
int getEndRow(int col, int row);//From entered column and row calculates the ending row
int getEndCol(int col, int row);//From entered column and row calculates the ending column
void printWin(char playerMark, char how[16]);//Prints out the player that won and how he won
void switchPlayers();//Switches the players
void drawBoardOnly();//Draw only board
int checkIsDigit(char inChar, int returnValue);//Checks if its a digit and validate it
void saveY();//Saves the position of the winning combination
void clearY();//Clears the y-s if there are not enough to win
void fillWinBoard();//Fills in the Y on the winning board
int columnFull(int col);//Checks if the column is full
int boardFull();//Checks if the board is full
void saveGame();//function to saves the game
void saveFile(struct game newGame);
void lastGameId();//save id from last game
void listAllSavedGames();
void listAllSavedGamesPlayer();//Lists all saved games or for one player
void loadGame(int startGame);//Function to load game
static int loadGameFromFile(int inGameId);

void startGame()
{
    int userInput;
    userInput = mainMenu();
    switch (userInput)
    {
    case 0:
        startGame();//If its an invalid input it loops back until a correct one is entered
        break;
    case 1:
        playNewGame();//function that start new game
        break;
    case 2:
        loadMenu();//function that load game
        break;
    case 3:
        printf("Thank you for playing\n");//exit game
        break;
    default:
        break;
    }
}
int checkIsDigit(char inChar, int returnValue)
{
    int result = returnValue;//store second value in result
    switch (inChar)//uses the input as the argument
    {
    case '0':
        result = 0;
        break;
    case '1':
        result = 1;
        break;
    case '2':
        result = 2;
        break;
    case '3':
        result = 3;
        break;
    case '4':
        result = 4;
        break;
    case '5':
        result = 5;
        break;
    case '6':
        result = 6;
        break;
    case '7':
        result = 7;
        break;
    case '8':
        result = 8;
        break;
    case '9':
        result = 9;//Check if its a number and returns its value
        break;
    default:
        break;
    }

    return result;// return is 0 if it is not digit
}
void drawBoard()
{
    int column;
    char charColumn;
    if (playerTurn == 1)//show message to selected player
    {
        printf("%s (X) turn - enter column 1-7 or 0 to save game\n", playerOne);
    }
    else
    {
        printf("%s (0) turn - enter column 1-7 or 0 to save game\n", playerTwo);
    }
    drawBoardOnly();
    if (boardFull() == 1)//Checks if the board is full
    {
        printf("Board full, choose option\n");
        Aftermenu();//Gives options after game is finished
    }
    else
    {
        fflush(stdin);
        charColumn = getchar();//Gets only the first character entered and stores it in charcolumn
        column = checkIsDigit(charColumn, -1);//Checks if its a digit, -1 is here because 0 is designated to saveGame already so we needed another number to print out a wrong input message
        fflush(stdin);
        switch (column)//Use player input for switch
        {
        case 0://Save game
            saveGame();//Calls the function that saves the game
            drawBoard();//Continues playing game
            break;
        case 1 ... 7://continue if user input number from 1 to 7
            if (columnFull(column - 1) == 1)//Checks if the column is full, its column -1 because of the difference between counting
            {
                printf("Column full, choose another one\n");//Message to choose another column because selected one is full
                drawBoard();//Continues playing game
            }
            else
            {
                if (fillBoard(column - 1, playerTurn) == 0)
                {
                    drawBoard();//If fillboard returns 0 the mark is written and then the players are switched
                }
                else
                {
                    Aftermenu();//If fillboard returns 1 it means someone won and it gives the Aftermenu menu
                }
            }
            break;
        default:
            printf("Please enter numbers form 1 to 7\n\n");//Message to player if they entered wrong number
            drawBoard();//Gives the same player a chance to redeem themselves
            break;
        }
    }
}
void drawBoardOnly()
{
    printf(" 1  2  3  4  5  6  7\n");
    for(int c = 0; c < 6; c++)//A loop that writes out the rows
    {
        for(int r = 0; r < 7; r++)//A loop that writes the columns
        {
            if(grid[r][c] == 'Y')//Checks if its a Y which is a win
            {
                printf("|_");//opens and closes a cell
                printf("_");//space in cell
                if(r==6)
                {
                    printf("|\n");//if the column comes to 6(7 because 0 is included) it closes it and new line
                }
            }
            else
            {
                printf("_%c_", grid[r][c]);//Prints out with whatever the value of the grid for that position is
                if(r==6)
                {
                    printf("|\n");//if the column comes to 6(7 because 0 is included) it closes it and new line
                }
            }

        }
    }
}
void playNewGame()
{
    printf("Enter player 1 name:");
    scanf("%s", playerOne);//Stores name for player one
    fflush(stdin);
    printf("Enter player 2 name:");
    scanf("%s", playerTwo);//Stores name for player two
    fflush(stdin);//Flushes the buffer
    playerTurn = 1;//Sets player one to play
    clearBoard();//Clears the board if anything stayed
    drawBoard();//The game is started
}
int fillBoard(int col, int playTurn)
{
    char playerMark;
    if (playTurn == 1)//Assigns the mark(X,O)to the player
    {
        playerMark = 'X';
    }
    else
    {
        playerMark = 'O';
    }

    for (int r = 5; r >= 0; r--)//A backward loop that goes through each row
    {
        if (grid[col][r] == ' ')//Checks if the place is empty
        {
            grid[col][r] = playerMark;//assigns the position entered to the playermark,col is user input and that input is used in this function and r mean that it will loop as many time as there is rows
            if (checkWin(col, r, playerMark) == 1)//Checks the win and if the function returns a win it calls the function fillwinboard
            {
                fillWinBoard();
                return 1;
            }
            else
            {
                switchPlayers();// If it is not a win it switches players
            }
            break;
        }
    }
    return 0;
}
void clearBoard()
{
    for (int c = 0; c < 7; c++)
    {
        for (int r = 0; r < 6; r++)
            grid[c][r] = ' ';//Goes through the whole board and check if it is blank spot or loop
    }
}
int checkWin(int column, int row, char playMark)
{
    if (checkWinByRow(row, playMark) == 1)//Checks if the player won by row return 1 if player did
    {
        return 1;//Return 1 if player won
    }
    else if (checkWinByColumn(column, playMark) == 1)//Checks if the player won by column return 1 if player did
    {
        return 1;//Returns 1 if player won
    }
    else if (checkWinByDiag1(column, row, playMark) == 1)//Checks if the player won by diagonal return 1 if player did
    {
        return 1;//Returns 1 if player won
    }
    else if (checkWinByDiag2(column, row, playMark) == 1)//Checks if the player won by diagonal return 1 if player did
    {
        return 1;//Returns 1 if the player won
    }
    return 0;
}
int checkWinByRow(int row, int playMark)
{
    int storeWin = 0;
    int checkWinCount = 0;
    for (int c = 0; c < 7; c++)//for loops for the column
    {
        if (grid[c][row] == playMark)//check if row is equal to playmark
        {
            saveY(c, row, checkWinCount);
            checkWinCount++;
            if (checkWinCount == 4)//wait for 4 counts to win
            {
                printWin(playMark, "by Row");
                storeWin = 1;
                break;
            }
        }
        else
        {
            checkWinCount = 0;
            clearY();
        }
    }
    return storeWin;
}

int checkWinByColumn(int column, char playMark)
{
    int storeWin = 0;
    int checkWinCount = 0;
    for (int r = 0; r < 6; r++)//For loops for the row
    {
        if (grid[column][r] == playMark)
        {
            saveY(column, r, checkWinCount);
            checkWinCount++;
            if (checkWinCount == 4)
            {
                printWin(playMark, "by Column");
                storeWin = 1;
                break;
            }
        }
        else
        {
            checkWinCount = 0;
            clearY();
        }
    }
    return storeWin;
}

int checkWinByDiag1(int column, int row, int playMark)
{
    int storeWin = 0;
    int checkWinCount = 0;
    int startRow;
    int startCol;
    startRow = getStartRow(column, row);//Gets start row by sending row and column
    startCol = getStartCol(column, row);//Gets starting column by sending row and column
    for (int r = startRow; r < 6; r++)
    {
        if (startCol <= 6)
        {
            if (grid[startCol][r] == playMark)//If its equal to the play mark it stores it in SaveY
            {
                saveY(startCol, r, checkWinCount);
                checkWinCount++;
                if (checkWinCount == 4)//wait for 4 count to continue
                {
                    printWin(playMark, "by diagonal Top Left-Bottom Right\n");
                    storeWin = 1;
                    break;
                }
            }
            else
            {
                checkWinCount = 0;//Resets count
                clearY();//Clears Y
            }
            startCol++;//increments column
        }
    }
    return storeWin;//Returns if its a win or not
}
int checkWinByDiag2(int column, int row, int playMark)//Takes the column,row and who's player turn it is
{
    int storeWin = 0;
    int checkWinCount = 0;//Count starts from 0
    int endRow;
    int endCol;
    endRow = getEndRow(column, row);//Gets the end row and stores it in endrow
    endCol = getEndCol(column, row);//Gets the end col and stores in endcol
    for (int r = endRow; r >= 0; r--)//Loop that starts from the end row
    {
        if (endCol <= 6)
        {
            if (grid[endCol][r] == playMark)//If the position is the same as playermark it goes through
            {
                saveY(endCol, r, checkWinCount);//saves the position
                checkWinCount++;//increments the count
                if (checkWinCount == 4)//wait for 4 to continue
                {
                    printWin(playMark, "by diagonal Top Right-Bottom Left\n");//Prints win message
                    storeWin = 1;//stores that player won
                    break;
                }
            }
            else//clear y if position is not same as playmark
            {
                checkWinCount = 0;
                clearY();
            }
            endCol++;//increments the col
        }
    }
    return storeWin;//Return if some player won or not
}
int getEndRow(int col, int row)
{
    int result;
    result = col + row;
    if (result > 5)//limit the calculation from going over 5
    {
        result = 5;
    }
    return result;//Returns the end row
}

int getEndCol(int col, int row)
{
    int result;
    result = col + row;
    result = col + row;
    if (result > 5)//limit the calculation from going over 5
    {
        result = 5;
    }
    result = (row + col) - result;
    return result;//Returns the end column
}
int getStartRow(int col, int row)
{
    int result;
    result = row - col;
    if (result < 0)//Row can not be smaller then 0
    {
        result = 0;
    }
    return result;
}

int getStartCol(int col, int row)
{
    int result;
    result = col - row;
    if (result < 0)//Column can not be smaller then 0
    {
        result = 0;
    }
    return result;
}
void switchPlayers()
{
    if (playerTurn == 1)
    {
        playerTurn = 2;
    }
    else
    {
        playerTurn = 1;//Switches players turn
    }
}

void fillWinBoard()
{
    for (int i = 0; i < 4; i++)//A loop that counts to 4
    {
        grid[gridWin[i].column][gridWin[i].row] = 'Y';//The position of the winning combination is changed to Y
    }
    drawBoardOnly();//Shows the winning board with the y
}

void clearY()
{
    for (int i = 0; i < 4; i++)//A loop that counts to 4
    {
        gridWin[i].column = -1;
        gridWin[i].row = -1;//if it is not a win clear values of the y gridwin
    }
}

void saveY(int column, int row, int count)
{
    gridWin[count].column = column;
    gridWin[count].row = row;//Saves the position of y in the gridwin
}

int columnFull(int col)
{
    if (grid[col][0] == ' ')//Checks if the top row of the entered column is empty
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

int boardFull()
{
    int result = 1;
    for (int i = 0; i < 7; i++)//for loop for the column
    {
        if (grid[i][0] == ' ')//Checks if the top row of each column is empty
        {
            result = 0;
            break;
        }
    }
    return result;
}
void printWin(char playerMark, char how[16])
{
    if (playerMark == 'X')//Checks who's players mark won
    {
        printf("Player %s (X) WON the game %s :)\n", playerOne, how);//Prints that x-player won and how
    }
    else
    {
        printf("Player %s (O) WON the game %s :)\n", playerTwo, how);//Prints that O-player won and how
    }
}
void Aftermenu()
{
    int userInput;
    char charInput;
    printf("What next?\n 1 - new game\n 0 - main menu\n");
    fflush(stdin);
    charInput = getchar();//get the user input
    userInput = checkIsDigit(charInput, -1);//Checks the input if it is valid
    fflush(stdin);
    switch (userInput)//Goes through the switch with player input
    {
    case 1:
        playNewGame();
        break;
    case 0:
        startGame();
        break;
    default:
        printf("Please try again!\n");
        Aftermenu();
        break;
    }
}
int loadMenu()//function to load game
{
    int userInput;
    char charInput;
    printf("Welcome to the loading menu, choose an option:\n");
    printf("1) List all saved games\n");
    printf("2) List all saved games for a particular player\n");
    printf("3) Show the board of one of the saved games\n");
    printf("4) Load a game\n");
    printf("5) Return to main menu\n");
    fflush(stdin);
    charInput = getchar();
    userInput = checkIsDigit(charInput, 0);
    fflush(stdin);
    switch (userInput)
    {
    case 1: // List All Saved Games
        listAllSavedGames();
        printf("\n");
        loadMenu();
        break;
    case 2: //List all saved games for a particular player
        listAllSavedGamesPlayer();
        loadMenu();
        break;
    case 3: //Show the board of one of the saved games
        loadGame(0);
        break;
    case 4:/* Load a game */
        loadGame(1);
        break;
    case 5://Back to main menu
        startGame();//function that is in int main
        break;

    default:
        break;
    }
}

void saveGame()
{
    struct game newGame;//calls the struct game
    newGame.id = 0;
    newGame.playerTurn = playerTurn;
    newGame.Player1 = playerOne;
    newGame.Player2 = playerTwo;//stores each variable into the struct correctly coresponding
    lastGameId();//Calls the function that gets the last id from the file
    saveFile(newGame);//calls the function savefile and sends it the struct
    printf("Game saved with id=%d\n", gameId);//prints out the game id
}

void saveFile(struct game newGame)
{
     ++gameId;//increments the gameid so that each time there is a new id
    FILE *file;//pointer to a file
    file = fopen("Save_Result.txt", "a");//opens the Save_Result.txt file in append
    if (file)/* if it exists */
    {
        fprintf(file,"%d=id\n",gameId);
        fprintf(file,"%d\n",newGame.playerTurn);
        fprintf(file,"%s\n",newGame.Player1);
        fprintf(file,"%s\n",newGame.Player2);//print in the file all of the above new line after each one
        for(int r = 0; r < 6; r++)
        {
            for(int c = 0; c < 7; c++)
            {
                fprintf(file,"%c",grid[c][r]);
            }
            fprintf(file,"\n");//double for loop to print out all the playermarks in the grid, after 6 a new line
        }
        fclose(file);//closes the file Save_Result.txt
    }
    else
    {
        --gameId;//if the file is opened for the first time the game id is 1 after that it is 2,3,4...
    }
}

void lastGameId()
{
    char line[20];//20 is the max characters in one line so we do not need to assign more
    FILE *file;
    file = fopen("Save_Result.txt", "r");//opens the file in read only
    if(file)
    {
        while (fgets(line, 20, file))//reads the file
        {
            if(strstr(line, "=id") != NULL)//if it finds =id it goes through
            {
                char *lineWithId = strtok(line, "=");//takes the value before the =
                gameId = atoi(lineWithId);//Converts the string into an int
            }
        }
        fclose(file);//close file
    }
    else
    {
        gameId = 0;//first time opening file the id is 0
    }
}

void listAllSavedGames()
{
    int lineCount = 0;//number of lines
    char line[20];
    int gamesFoundCount = 0;//a counter for the number of ids found
    FILE *file;
    file = fopen("Save_Result.txt", "r");
    if(file)
    {
        while (fgets(line, 20, file))
        {
            if(strstr(line, "=id") != NULL)
            {
                char *lineWithId = strtok(line, "=");
                gameId = atoi(lineWithId);
                printf("\nGameId=%d,", gameId);//prints out the gameid
                lineCount = 0;//resets line count because ever game starts with id
                gamesFoundCount++;//add because it found new game
            }
            switch (lineCount)//takes the current line it is on into the switch
            {
            case 1:
                printf(" Player %d turn,", atoi(line));//convert with atoi and print out
                break;
            case 2:
                printf(" Player 1 (X) name %s,", strtok(line, "\n"));//print out until \n because our info is all stored in new line
                break;
            case 3:
                printf(" Player 2 (O) name %s", strtok(line, "\n"));// prints out the contents of this line until it hits \n
                break;
            default:
                break;
            }
            lineCount++;//increments into the next line
        }
        printf("\n");//prints into a new line
        fclose(file);//close file
        if(gamesFoundCount == 0)//if there are no saved games to be found
        {
            printf("No saved games found in Save_Result.txt file\n");//print out message
        }
    }
    else
    {
        printf("Error opening results file\n");//print out error message
    }
}

void listAllSavedGamesPlayer()
{
    int lineCount = 0;
    char line[20];
    int localGameId;
    char localPlayer1[20];
    char localPlayer2[20];
    int localPlayerTurn;
    int playerFound;
    int playerFoundCount = 0;//count of the player name that is found
    char playerName[20];

    printf("Enter player name for saved games:");
    scanf("%s", playerName);//take input from player
    fflush(stdin);

    FILE *file;
    file = fopen("Save_Result.txt", "r");//read file
    if(file)
    {
        while (fgets(line, 20, file))
        {
            if(strstr(line, "=id") != NULL)
            {
                char *lineWithId = strtok(line, "=");//reads everything until the =
                localGameId = atoi(lineWithId);
                lineCount = 0;//resets the line
                playerFound = 0;//resets playerfound to 0 every time id is found(new game)
            }

            switch (lineCount)//put line into switch
            {
            case 1:
                localPlayerTurn = atoi(line);//stores the playerturn
                break;
            case 2:
                strcpy(localPlayer1, strtok(line, "\n"));//reads the string until it hits \n for that line and stores it in localplayer1
                if(strcmp(localPlayer1, playerName) == 0)//Use strcmp compares two strings and returns 0 if they are the same
                {
                    playerFound = 1;//changes when we found the player */
                    playerFoundCount++; //increments the number of games found
                    printf("\nGameId=%d,", localGameId);
                    printf(" Player %d turn,", localPlayerTurn);
                    printf(" Player 1 (X) name %s,", localPlayer1);//prints out all the info above
                }
                break;
            case 3:
                strcpy(localPlayer2, strtok(line, "\n"));//reads the string until it hits \n for that line and stores it in localplayer2
                if(playerFound == 1)//print out other player name if game is found
                {
                    printf(" Player 2 (O) name %s", localPlayer2);
                }
                else
                {
                    if(strcmp(localPlayer2, playerName) == 0)//if the players name is not found check here
                    {
                        playerFound = 1;
                        playerFoundCount++;
                        printf("\nGameId=%d,", localGameId);
                        printf(" Player %d turn,", localPlayerTurn);
                        printf(" Player 1 (X) name %s,", localPlayer1);
                        printf(" Player 2 (O) name %s", localPlayer2);//if we found it here print out all of the info above and increment foundcount
                    }
                }
                break;
            default:
                break;
            }
            lineCount++;//increments the line that is being read
        }
        printf("\n");
        fclose(file);
        if(playerFoundCount == 0)//if no games with that name were found
        {
            printf("No games found for this player %s\n", playerName);
        }
    }
    else
    {
        printf("Error opening results file\n");
    }
}

void loadGame(int startGame)
{
    int userInput;
    char charInput[2];
    int gameFound = 0;
    printf("Please enter Game Id:");
    fflush(stdin);
    scanf("%s", &charInput);//we get the gameid from the user
    fflush(stdin);
    userInput = atoi(charInput);//we convert the string into an int
    gameFound = loadGameFromFile(userInput);//use the input in loadgamesfromfile and stores the return in gamefound
    if(gameFound == 1)//id it is found go through
    {
        if(startGame == 1)//this if is whether we will only display the board or continue playing it
        {
            drawBoard();//continue playing game from where it was saved
        }
        else
        {
            drawBoardOnly();//draw only board
            loadMenu();//gives load menu again
        }
    }
    else
    {
        printf("Game not found for id=%d\n", userInput);//error message if game is not found
        loadMenu();//gives load menu again
    }

}

static int loadGameFromFile(int inGameId)
{
    int result = 0;
    int lineCount = 0;
    char line[20];
    int fileGameId;
    int gameIdFound = 0;
    clearBoard();//clear board from anything on it
    FILE *file;
    file = fopen("Save_Result.txt", "r");//read file
    if(file)
    {
        while (fgets(line, 20, file))
        {
            //Find game with id
            if(strstr(line, "=id") != NULL)
            {
                char *lineWithId = strtok(line, "=");
                fileGameId = atoi(lineWithId);
                if(fileGameId == inGameId)//if player entered game that exist it goes through
                {
                    gameIdFound = 1;
                    gameId = inGameId;
                    result = 1;//changes the result to found
                }
                else
                {
                    gameIdFound = 0;
                    result = 0;//if it does not find it
                }
                lineCount = 0;
            }
            if(gameIdFound == 1)//if the game is found go through
            {
                switch (lineCount)//line count
                {
                case 1:
                    playerTurn = atoi(line);//players turn
                    break;
                case 2:
                    strcpy(playerOne, strtok(line, "\n"));//reads the string until it hits \n for that line and stores it in player1
                    printf("%s (X)", playerOne);//prints out player ones name
                    break;
                case 3:
                    strcpy(playerTwo, strtok(line, "\n"));//same as player one just for player two
                    printf(" VS %s (O)\n", playerTwo);//prints out player ones name
                    break;
                case 4 ... 9:
                    for (int c = 0; c < 7; c++)
                    {
                        grid[c][lineCount-4] = line[c];//Storing the board from the file into the grid
                    }
                    if(lineCount == 9)//When it comes to the end of a saved game it returns the result
                    {
                        return result;
                    }
                    break;
                default:
                    break;
                }
            }
            lineCount++;//increments the line count
        }
        fclose(file);//close file
    }
    else
    {
        printf("Error opening Save_Results file\n");//error message if it can not open save file
        result = 0;
    }

    return result;
}
//ALL FUNCTIONS
int mainMenu()
{
    int choice = 0;
    char choiceChar;
    int result = 0;
    printf("Welcome to connect 4!\n");
    printf("Please choose an option:\n");
    printf("1) Play new game\n");
    printf("2) Load game\n");
    printf("3) Exit the game\n");
    choiceChar = getchar(); //Gets input from user and stores it in choiceChar
    choice = checkIsDigit(choiceChar, 0); //Sends the input and 0 to choiceChar and stores the return in choice
    switch (choice)//use digit in switch
    {
    case 1 ... 3:
        result = choice;//Check if choice is valid and store it in result
        return result;
        break;
    default:
        printf("Wrong input - Try again:\n\n");//If the result is not valid it prints error message and returns 0 as a result so it loop
        result = 0;
        return result;
        break;
    }
    return result;
}
//Main function
int main()
{
    startGame();//redirect to other functions
}

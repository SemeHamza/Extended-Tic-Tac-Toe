#include <iostream>
#include <string>
#include <iomanip>
using namespace std;

struct Player
{
        string firstName;
        string lastName;
        string fullName;
        char piece;
        int numWon;
        int numLost;
        int numDraw;
};

const int MIN_NUM_PLAYERS = 2, MAX_NUM_PLAYERS = 5;
const int MIN_ROW_SIZE = 3, MAX_ROW_SIZE = 12, MIN_COL_SIZE = 3, MAX_COL_SIZE = 15;

void getInfo(Player[], int&, int&);
bool validName(string);
bool validNumber(string, int&);
void parseName(string, Player[] , int);
char toLower(char);
char toUpper(char);
void initializeBoard(char[][MAX_COL_SIZE], int&, int&);
void getMove(char[][MAX_COL_SIZE], int, int, char, Player[]);
bool validMove(string, char[][MAX_COL_SIZE], int, int, int&, int&);
bool won(char[][MAX_COL_SIZE], int, int);
bool tie(bool, int, int, int);
void updateStats(bool, bool, char, Player[], int);
void dispalyStats(Player[], int, int, int);
void displayBoard(char[][MAX_COL_SIZE], int, int);

int main()
{
    Player players[MAX_NUM_PLAYERS];
    int numPlayers, longestName = 0;
    char startingTurn = 'a', movingTurn = 'a';
    char board[MAX_ROW_SIZE][MAX_COL_SIZE];
    int rowSize, columnSize;
    bool gameWon = false, gameDraw = false;
    int numMoves, gamesPlayed = 0;
    int quit;
    string validQuit;

    cout << "TIC-TAC-TOE\n"
         << "Let's get started!\n\n";

    getInfo(players, numPlayers, longestName);

    do
    {
        initializeBoard(board, rowSize, columnSize);

        numMoves = 0;

        do
        {
            displayBoard(board, rowSize, columnSize);
            getMove(board, rowSize, columnSize, movingTurn, players);
            numMoves++;

            gameWon = won(board, rowSize, columnSize);
            gameDraw = tie(gameWon, rowSize, columnSize, numMoves);
            updateStats(gameWon, gameDraw, movingTurn, players, numPlayers);

            if(movingTurn == 'a' + numPlayers - 1)
                movingTurn = 'a';
            else
                movingTurn++;

        }while(!gameWon && !gameDraw);

        gamesPlayed++;

        displayBoard(board, rowSize, columnSize);
        dispalyStats(players, numPlayers, gamesPlayed, longestName);

        if(gameWon)
        {
            startingTurn = movingTurn;
        }
        else
        {
            if(startingTurn == 'a' + numPlayers - 1)
                startingTurn = 'a';
            else
                startingTurn++;

            movingTurn = startingTurn;
        }

        cout << "Enter '0' to continue or enter '1' to quit:";
        getline(cin, validQuit);

        while(!validNumber(validQuit, quit) || quit < 0 || quit > 1)
        {
            cout << "Error! Enter '0' to continue or enter '1' to quit:";
            getline(cin, validQuit);
        }

    }while(quit == 0);

    return 0;
}

//==========================================================================
//Gets the number of players and information (full name) of each player
//Initializes the statistics of each player to zero
//==========================================================================

void getInfo(Player players[], int& numPlayers, int& longestName)
{
    string numberPlayers;

    cout << "Enter the number of players:";
    getline(cin, numberPlayers);

    while(!validNumber(numberPlayers, numPlayers) || (numPlayers < MIN_NUM_PLAYERS || numPlayers > MAX_NUM_PLAYERS))
    {
        cout << "Error! Enter the number of players:";
        getline(cin, numberPlayers);
    }

    for(int i = 0; i < numPlayers; i++)
    {
        string name;
        int nameLength = 0;

        players[i].piece = 'a' + i;
        players[i].numDraw = 0;
        players[i].numLost = 0;
        players[i].numWon = 0;

        cout << "Player #" << i + 1 << " , enter your full name:";
        getline(cin, name);

        while(!validName(name))
        {
            cout << "Error! Player #" << i + 1 << " , enter your full name:";
            getline(cin, name);
        }

        parseName(name, players, i);

        for(int i = 0; name[i] != 0; i++)
            nameLength++;
        if(nameLength > longestName)
            longestName = nameLength;
    }
}

//===========================================================================
//Returns true if an input is a number between 0 and 99
//Returns false otherwise
//===========================================================================

bool validNumber(string input, int& num)
{
    int i = 0;

    for(; input[i] != 0; i++)
    {
        if(input[i] < '0' || input[i] > '9')
        {
            return false;
        }
    }

    num = 0;

    if(i == 1)
    {
        num = input[0] - '0';
        return true;
    }
    else if(i == 2)
    {
        num = (input[0] - '0') * 10 + input[1] - '0';
        return true;
    }
    else
        return false;
}

//===========================================================================
//Returns true if an input is a valid name
//Returns false otherwise
//===========================================================================

bool validName(string name)
{
    int numSpace = 0;
    int i = 0, lastIndex;

    for(; name[i] != 0; i++)
    {
        name[i] = toUpper(name[i]);
        if((name[i] < 'A' || name[i] > 'Z') && (name[i] != ' '))
        {
            return false;
        }
        if(name[i] == ' ')
        {
            numSpace++;
        }
    }
    lastIndex = i - 1;
    if(name[0] == ' ' || name[lastIndex] == ' ')
    {
        return false;
    }
    else if(numSpace == 0)
    {
        return false;
    }
    return true;
}

//===========================================================================
//Splits an input into first name and last name
//===========================================================================

void parseName(string name, Player players[], int index)
{
    int indexFirstSpace = 0;

    for(int i = 0; name[i] != 0; i++)
    {
        name[i] = toLower(name[i]);
    }
    for(int i = 0; name[i] != 0; i++)
    {
      if(name[i] == ' ')
            name[i + 1] = toUpper(name[i+1]);
    }
    name[0] = toUpper(name[0]);

    players[index].fullName = name;

    for(; name[indexFirstSpace] != ' '; indexFirstSpace++)
    {
        players[index].firstName[indexFirstSpace] = name[indexFirstSpace];
    }
    players[index].firstName[indexFirstSpace] = 0;

    for(int i = ++indexFirstSpace, j = 0; name[i - 1] != 0; i++, j++)
    {
        players[index].lastName[j] = name[i];
    }
}

//===========================================================================
//Returns a lower case form of an input if  the character is an upper case alphabet
//Returns the input itself otherwise
//===========================================================================

char toLower(char letter)
{
    if(letter >= 'A' && letter <= 'Z')
    {
        letter += 'a' - 'A';
        return letter;
    }
    else
        return letter;
}

//===========================================================================
//Returns an upper case form of an input if  the character is a lower case alphabet
//Returns the input itself otherwise
//===========================================================================


char toUpper(char letter)
{
    if(letter >= 'a' && letter <= 'z')
    {
        letter += 'A' - 'a';
        return letter;
    }
    else
        return letter;
}

//===========================================================================
//Gets the dimension of the board
//Initializes the board to space
//===========================================================================

void initializeBoard(char board[][MAX_COL_SIZE], int& rowSize, int& columnSize)
{
    string row, column;

    cout << "\nPlease enter the dimension of the board.\n";
    cout << "Enter the number of rows    -> ";
    getline(cin, row);
    while(!validNumber(row, rowSize) || (rowSize < MIN_ROW_SIZE || rowSize > MAX_ROW_SIZE))
    {
        cout << "Error! Enter the number of rows   -> ";
        getline(cin, row);
    }

    cout << "Enter the number of columns -> ";
    getline(cin, column);
    while(!validNumber(column, columnSize) || (columnSize < MIN_COL_SIZE || columnSize > MAX_COL_SIZE))
    {
        cout << "Error! Enter the number of columns -> ";
        getline(cin, column);
    }

    for(int row = 0; row < rowSize; row++)
    {
        for(int col = 0; col < columnSize; col++)
        {
           board[row][col] = ' ';
        }
    }
}

//===========================================================================
//Let's a player make a move
//===========================================================================

void getMove(char board[][MAX_COL_SIZE], int rowSize, int columnSize, char movingTurn, Player players[])
{
    int rowChoice, colChoice;
    string choice;

    for(int i = 0; players[movingTurn - 'a'].firstName[i] != 0; i++)
        cout << players[movingTurn - 'a'].firstName[i];
    cout << " (playing piece " << players[movingTurn - 'a'].piece << "), make your move: ";
    getline(cin, choice);

    while(!validMove(choice, board, rowSize, columnSize, rowChoice, colChoice))
    {
        cout << "Error! ";
        for(int i = 0; players[movingTurn - 'a'].firstName[i] != 0; i++)
            cout << players[movingTurn - 'a'].firstName[i];
        cout << " (playing piece " << players[movingTurn - 'a'].piece << "), make your move: ";
        getline(cin, choice);
    }

    board[rowChoice][colChoice] = movingTurn;
}

//===========================================================================
//Returns true if a move is valid
//Returns false otherwise
//Parses a string input
//===========================================================================

bool validMove(string choice, char board[][MAX_COL_SIZE], int rowSize, int columnSize, int&  rowChoice, int& colChoice)
{
    int choiceLength = 0;

    choice[0] = toUpper(choice[0]);

    for(int i = 0; choice[i] != 0; i++)
    {
        choiceLength++;
    }

    if(choiceLength == 2)
    {
        if(choice[0] < 'A' || choice[0] > 'Z' || choice[1] < '1' || choice[1] > '9')
        {
            return false;
        }
        else
        {
            rowChoice = choice[0] - 'A';
            colChoice = choice[1] - '1';
        }
    }
    else if(choiceLength == 3)
    {
        if(choice[0] < 'A' || choice[0] > 'Z' || choice[1] < '1' || choice[1] > '9' || choice[2] < '0' || choice[2] > '9')
        {
            return false;
        }
        else
        {
            rowChoice = choice[0] - 'A';
            colChoice = (choice[1] - '0') * 10 + (choice[2] - '1');
        }
    }
    else
    {
        return false;
    }

    if(rowChoice < 0 || rowChoice >= rowSize || colChoice < 0 || colChoice >= columnSize)
    {
        return false;
    }
    else if(board[rowChoice][colChoice] != ' ')
    {
        return false;
    }

    return true;
}

//===========================================================================
//Returns true if someone has won
//Returns false otherwise
//===========================================================================

bool won(char board[][MAX_COL_SIZE], int rowSize, int columnSize)
{
    for(int row = 0; row < rowSize - 2; row++)
    {
        for(int col = 0; col < columnSize; col++)
        {
           if(board[row][col] != ' ')
           {
                if(board[row][col] == board[row + 1][col] && board[row][col] == board[row + 2][col])
                {
                    board[row][col] += 'A' - 'a';
                    board[row + 1][col] += 'A' - 'a';
                    board[row + 2][col] += 'A' - 'a';
                    return true;
                }
           }
        }
    }

    for(int row = 0; row < rowSize; row++)
    {
        for(int col = 0; col < columnSize - 2; col++)
        {
           if(board[row][col] != ' ')
           {
                if(board[row][col] == board[row][col + 1] && board[row][col] == board[row][col + 2])
                {
                    board[row][col] += 'A' - 'a';
                    board[row][col + 1] += 'A' - 'a';
                    board[row][col + 2] += 'A' - 'a';
                    return true;
                }
           }
        }
    }


    for(int row = 0; row < rowSize - 2; row++)
    {
        for(int col = 0; col < columnSize - 2; col++)
        {
           if(board[row][col] != ' ')
           {
                if(board[row][col] == board[row + 1][col + 1] && board[row][col] == board[row + 2][col + 2])
                {
                    board[row][col] += 'A' - 'a';
                    board[row + 1][col + 1] += 'A' - 'a';
                    board[row + 2][col + 2] += 'A' - 'a';
                    return true;
                }
           }
        }
    }

    for(int row = 0; row < rowSize - 2; row++)
    {
        for(int col = columnSize; col > 1; col--)
        {
           if(board[row][col] != ' ')
           {
                if(board[row][col] == board[row + 1][col - 1] && board[row][col] == board[row + 2][col - 2])
                {
                    board[row][col] += 'A' - 'a';
                    board[row + 1][col - 1] += 'A' - 'a';
                    board[row + 2][col - 2] += 'A' - 'a';
                    return true;
                }
           }
        }
    }


    return false;
}

//===========================================================================
//Returns true if the game is draw
//Returns false otherwise
//===========================================================================

bool tie(bool gameWon, int rowSize, int columnSize, int numMoves)
{
    if(gameWon)
        return false;
    else if(numMoves == rowSize * columnSize)
        return true;

    return false;
}

//===========================================================================
//Updates the statistics of each player as necessary
//===========================================================================

void updateStats(bool gameWon, bool gameDraw, char movingTurn, Player players[], int numPlayers)
{
    if(gameWon)
    {
        players[movingTurn - 'a'].numWon += 1;

        cout << "within gameWon\n";
        cout << movingTurn << "  " << movingTurn - 'a' << endl;

        for(int i = 0; i < numPlayers; i++)
        {
            players[i].numLost += 1;
        }
        players[movingTurn - 'a'].numLost -= 1;
    }
    else if(gameDraw)
    {
        for(int i = 0; i < numPlayers; i++)
        {
            players[i].numDraw += 1;
        }
    }
}

//===========================================================================
//Displays the statistics of the game in the  required format
//===========================================================================

void dispalyStats(Player players[], int numPlayers, int gamesPlayed, int longestName)
{

    cout << "\nTotal game(s) played = " << gamesPlayed << "\n\n"
         << setw(longestName) << " " << "  ------ ------ ------\n"
         << setw(longestName) << " " << " |  WIN | LOSS | DRAW |\n"
         << setw(longestName) << " " << "  ------ ------ ------\n";
    for(int i = 0; i < numPlayers; i++)
    {
        cout << setw(longestName) << players[i].fullName << " | " << setw(4) << players[i].numWon
             << " | " << setw(4) << players[i].numLost << " | " << setw(4) << players[i].numDraw << " |\n"
             << setw(longestName) << " " << "  ------ ------ ------\n";
    }
    cout << endl;
}

//===========================================================================
//Displays the board in the  required format
//===========================================================================

void displayBoard(char board[][MAX_COL_SIZE], int rowSize, int columnSize)
{
    char letter;

    cout << "\n ";
    for(int col = 1; col <= columnSize; col++)
    {
        cout << "  " << right << setw(2) << col;
    }

    cout << "\n  ";
    for(int col = 1; col <= columnSize; col++)
    {
        cout << " ---";
    }
    cout << endl;

    for(int row = 0; row < rowSize; row++)
    {
        letter = 'A' + row;
        cout << letter << " | ";

        for(int col = 0; col < columnSize; col++)
        {
           cout << board[row][col] << " | ";
        }
        cout << letter << endl;

        cout << "  ";
        for(int col = 1; col <= columnSize; col++)
        {
            cout << " ---";
        }
        cout << endl;
    }

    cout << " ";
    for(int col = 1; col <= columnSize; col++)
    {
        cout << "  " << right << setw(2) << col;
    }
    cout << "\n\n";
}

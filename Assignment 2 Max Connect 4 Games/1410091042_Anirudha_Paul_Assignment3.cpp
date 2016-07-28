// MaxConnect-4 code

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
//#include <bits/stdc++.h>
#include <string>
#include <vector>
#include <stack>
#include <queue>
#include <map>
#include <iostream>
#include <algorithm>


using namespace std;

/******************** x to the power y **********************************/

long long power(long long x ,long long y)
{
    long long result = 1 ;

    for(long long  i = 0 ; i < y ; i++)
    {
        result*=x;
    }
    return result;
}

/******************* String To Int **************************************/
int stringToInt(string x)
{
    int length = x.length();

    int ans = 0;

    for(int i = length-1 , j= 0 ; i>=0 ; i--,j++)
    {
        ans += (x[i]-'0') * power(10,j);
        //cout << "Ans = " << ans << endl;
    }
    return ans;
}


class gameStatus
{
private:
    long * gameData;

public:
    long * gameBoard[6];
    long currentTurn;
    int player1Score;
    int player2Score;
    int pieceCount;
    FILE *gameFile;

    gameStatus()
    {
        gameData = new long[42];
        gameBoard[0] = &(gameData[0]);
        gameBoard[1] = &(gameData[7]);
        gameBoard[2] = &(gameData[14]);
        gameBoard[3] = &(gameData[21]);
        gameBoard[4] = &(gameData[28]);
        gameBoard[5] = &(gameData[35]);

        int i;

        for (i = 0; i < 42; i++)
        {
            gameData[i] = 0;
        }

        currentTurn = 1;
        player1Score = 0;
        player2Score = 0;
        pieceCount = 0;
        gameFile = 0;
    }

    ~gameStatus()
    {
        delete [] gameData;
    }
};

// Output current game status to console
void printGameBoard(gameStatus &currentGame)
{
    int i = 0;
    int j = 0;
    printf(" -----------------\n");
    for(i = 0; i < 6; i++)
    {
        printf(" | ");
        for(j = 0; j < 7; j++)
        {
            printf("%li ", currentGame.gameBoard[i][j]);
        }
        printf("| \n");
    }
    printf(" -----------------\n");
}

// Output current game status to file
void printGameBoardToFile(gameStatus &currentGame)
{
    int i = 0;
    int j = 0;
    for(i = 0; i < 6; i++)
    {
        for(j = 0; j < 7; j++)
        {
            fprintf(currentGame.gameFile, "%li", currentGame.gameBoard[i][j]);
        }
        fprintf(currentGame.gameFile, "\r\n");
    }
    fprintf(currentGame.gameFile, "%ld\r\n", currentGame.currentTurn);
}

// Given a column and which player it is,
// place that player's piece in the requested column
int playPiece(int column, gameStatus &currentGame)
{
    /*******Modified*******/

    column--;

    /*******Modified*******/

    // if column full, return 0
    if(currentGame.gameBoard[0][column] != 0)
    {
        return 0;
    }

    int i;
    // starting at the bottom of the board, place the piece into the
    // first empty spot
    for(i = 5; i >= 0; i--)
    {
        if(currentGame.gameBoard[i][column] == 0)
        {
            currentGame.gameBoard[i][column] = currentGame.currentTurn;
            currentGame.pieceCount++;
            return 1;
        }
    }
    return 0;
}

/*************************** MY CODE TO RUN AI Starts Here******************************/

    int INF = 999999999;
    int globalColumn = 3;
    int globalCounter = 0;

    int alpha , beta ;

    int minPoint(gameStatus &currentGame , int depth );
    void countScore(gameStatus &currentGame);
    int maxPoint(gameStatus &currentGame , int depth );
    int mini_max_with_alpha_beta(gameStatus &currentGame , int depth);

    int maxPoint(gameStatus &currentGame , int depth )
    {
        if(depth <= 0 || currentGame.pieceCount >= 42)
        {
            countScore(currentGame);

            if(currentGame.currentTurn == 1)
            {
                return currentGame.player1Score;
            }
            else if(currentGame.currentTurn == 2)
            {
                return currentGame.player2Score;
            }
        }
        int MAX = -1 * INF ;

        int empty_row = 4;
        for(int i = 0 ; i < 6 ; i++)
        {
            for(int j = 0 ; j < 7 ; j++)
            {
                if(currentGame.gameBoard[i][j] != 0)
                {
                    if(i>=1)
                    {
                        empty_row = i-1;
                    }
                    break;
                }
            }
            if(empty_row!=4)
            {
                break;
            }
        }

        for(int i = empty_row ; i < 6 ; i++)
        {
            for(int j = 3 ; j < 7 ; j++)
            {
                if(currentGame.gameBoard[i][j] == 0)
                {
                    currentGame.gameBoard[i][j] = currentGame.currentTurn;
                    currentGame.pieceCount++;
                    if(currentGame.currentTurn == 1)
                        currentGame.currentTurn = 2;
                    else if (currentGame.currentTurn == 2)
                        currentGame.currentTurn = 1;
                    int tempMAX  = MAX ;
                    MAX = max(MAX, minPoint(currentGame , depth-1 ));

                    if(tempMAX != MAX)
                    {
                        globalColumn = j+1;
                    }

                    currentGame.gameBoard[i][j] = 0;
                    currentGame.pieceCount--;
                    if(currentGame.currentTurn == 1)
                        currentGame.currentTurn = 2;
                    else if (currentGame.currentTurn == 2)
                        currentGame.currentTurn = 1;

                    if(MAX >= beta)
                    {
                        /**Uncomment this if you need to Unit test the value of Max , current Empty Row selction and number of function call **/
//                        globalCounter++;
//                        cout << "Max = " << MAX << endl;
//                        cout << "Empty Row = " << empty_row << endl;
                        return MAX ;
                    }
                    alpha = max(alpha , MAX);
                }
            }

            for(int j = 2 ; j >= 0 ; j--)
            {
                if(currentGame.gameBoard[i][j] == 0)
                {
                    currentGame.gameBoard[i][j] = currentGame.currentTurn;
                    currentGame.pieceCount++;
                    if(currentGame.currentTurn == 1)
                        currentGame.currentTurn = 2;
                    else if (currentGame.currentTurn == 2)
                        currentGame.currentTurn = 1;

                    int tempMAX  = MAX ;

                    MAX = max(MAX, minPoint(currentGame , depth-1 ));

                    if(tempMAX != MAX)
                    {
                        globalColumn = j+1;
                    }

                    currentGame.gameBoard[i][j] = 0;
                    currentGame.pieceCount--;
                    if(currentGame.currentTurn == 1)
                        currentGame.currentTurn = 2;
                    else if (currentGame.currentTurn == 2)
                        currentGame.currentTurn = 1;

                    if(MAX >= beta)
                    {
                        /**Uncomment this if you want unit test the max value and current empty row selection and number of function call **/
//                        globalCounter++;
//                        cout << "Max = " << MAX << endl;
//                        cout << "Empty Row = " << empty_row << endl;
                        return MAX ;
                    }
                    alpha = max(alpha , MAX);
                }
            }
        }
        /**Uncomment this if you want unit test the max value and current empty row selection and number of function call **/
//        globalCounter++;
//        cout << "Max = " << MAX << endl;
//        cout << "Empty Row = " << empty_row << endl;
        return MAX ;
    }

    int minPoint(gameStatus &currentGame , int depth )
    {
        if(depth <= 0 || currentGame.pieceCount >= 42)
        {
            countScore(currentGame);

            if(currentGame.currentTurn == 1)
            {
                return currentGame.player1Score;
            }
            else if(currentGame.currentTurn == 2)
            {
                return currentGame.player2Score;
            }
        }
        int MIN = INF ;

        int empty_row = 4;
        for(int i = 0 ; i < 6 ; i++)
        {
            for(int j = 0 ; j < 7 ; j++)
            {
                if(currentGame.gameBoard[i][j] != 0)
                {
                    if(i>=1)
                    {
                        empty_row = i-1;
                    }
                    break;
                }
            }
            if(empty_row!=4)
            {
                break;
            }
        }
        for(int i = empty_row ; i < 6 ; i++)
        {
            for(int j = 3 ; j < 7 ; j++)
            {
                if(currentGame.gameBoard[i][j] == 0)
                {
                    currentGame.gameBoard[i][j] = currentGame.currentTurn;
                    currentGame.pieceCount++;
                    if(currentGame.currentTurn == 1)
                        currentGame.currentTurn = 2;
                    else if (currentGame.currentTurn == 2)
                        currentGame.currentTurn = 1;

                    MIN = min(MIN, maxPoint(currentGame , depth-1 ));

                    currentGame.gameBoard[i][j] = 0;
                    currentGame.pieceCount--;
                    if(currentGame.currentTurn == 1)
                        currentGame.currentTurn = 2;
                    else if (currentGame.currentTurn == 2)
                        currentGame.currentTurn = 1;

                    if(MIN <= alpha)
                    {
                        return MIN ;
                    }
                    beta = min(beta , MIN);
                }
            }

            for(int j = 2 ; j >= 0 ; j--)
            {
                if(currentGame.gameBoard[i][j] == 0)
                {
                    currentGame.gameBoard[i][j] = currentGame.currentTurn;
                    currentGame.pieceCount++;
                    if(currentGame.currentTurn == 1)
                        currentGame.currentTurn = 2;
                    else if (currentGame.currentTurn == 2)
                        currentGame.currentTurn = 1;

                    MIN = min(MIN, maxPoint(currentGame , depth-1 ));

                    currentGame.gameBoard[i][j] = 0;
                    currentGame.pieceCount--;
                    if(currentGame.currentTurn == 1)
                        currentGame.currentTurn = 2;
                    else if (currentGame.currentTurn == 2)
                        currentGame.currentTurn = 1;

                    if(MIN <= alpha)
                    {
                        return MIN ;
                    }
                    beta = min(beta , MIN);
                }
            }
        }
        return MIN ;
    }

    int mini_max_with_alpha_beta(gameStatus &currentGame , int depth)
    {
        alpha = -1 * INF;
        beta = INF ;
        maxPoint(currentGame , depth );
        int bestColumn = globalColumn;
        return bestColumn;
    }

/*************************** MY CODE TO RUN AI Ends Here******************************/

// The AI section.  Currently plays randomly.
void aiPlay(gameStatus &currentGame , int depth)
{
    //cout << "Depth" << depth << endl;
    //int randColumn = (int) rand() % 7;

    /**************Modification Started************/

    if(currentGame.pieceCount >=42)
    {
        return;
    }
    //printGameBoard(currentGame);                    //Uncomment for unit testing and see the outcome after each function call
    int randColumn = mini_max_with_alpha_beta(currentGame , depth);

    //cout << "RandCol = " << randColumn << endl;     //Uncomment to print the column selected by AI
    //cout << "Counter = " << globalCounter << endl;  //Uncomment to see the number of function called performed

    globalCounter = 0;

    /**************Modification Ended************/


    int result = 0;
    result = playPiece(randColumn, currentGame);
    if(result == 0)
    {

        aiPlay(currentGame , depth);
    }
    else
    {
        printf("\n\nmove %d: Player %li, column %d\n",
               currentGame.pieceCount, currentGame.currentTurn, randColumn);
        if(currentGame.currentTurn == 1)
            currentGame.currentTurn = 2;
        else if (currentGame.currentTurn == 2)
            currentGame.currentTurn = 1;
    }
}

void countScore(gameStatus &currentGame)
{
    currentGame.player1Score = 0;
    currentGame.player2Score = 0;

    //check horizontally
    int i;
    for(i = 0; i < 6; i++)
    {
        //check player 1
        if(currentGame.gameBoard[i][0] == 1 && currentGame.gameBoard[i][1] == 1
           && currentGame.gameBoard[i][2] == 1 && currentGame.gameBoard[i][3] == 1)
        {currentGame.player1Score++;}
        if(currentGame.gameBoard[i][1] == 1 && currentGame.gameBoard[i][2] == 1
           && currentGame.gameBoard[i][3] == 1 && currentGame.gameBoard[i][4] == 1)
        {currentGame.player1Score++;}
        if(currentGame.gameBoard[i][2] == 1 && currentGame.gameBoard[i][3] == 1
           && currentGame.gameBoard[i][4] == 1 && currentGame.gameBoard[i][5] == 1)
        {currentGame.player1Score++;}
        if(currentGame.gameBoard[i][3] == 1 && currentGame.gameBoard[i][4] == 1
           && currentGame.gameBoard[i][5] == 1 && currentGame.gameBoard[i][6] == 1)
        {currentGame.player1Score++;}
        //check player 2
        if(currentGame.gameBoard[i][0] == 2 && currentGame.gameBoard[i][1] == 2
           && currentGame.gameBoard[i][2] == 2 && currentGame.gameBoard[i][3] == 2)
        {currentGame.player2Score++;}
        if(currentGame.gameBoard[i][1] == 2 && currentGame.gameBoard[i][2] == 2
           && currentGame.gameBoard[i][3] == 2 && currentGame.gameBoard[i][4] == 2)
        {currentGame.player2Score++;}
        if(currentGame.gameBoard[i][2] == 2 && currentGame.gameBoard[i][3] == 2
           && currentGame.gameBoard[i][4] == 2 && currentGame.gameBoard[i][5] == 2)
        {currentGame.player2Score++;}
        if(currentGame.gameBoard[i][3] == 2 && currentGame.gameBoard[i][4] == 2
           && currentGame.gameBoard[i][5] == 2 && currentGame.gameBoard[i][6] == 2)
        {currentGame.player2Score++;}
    }

    //check vertically
    int j;
    for(j = 0; j < 7; j++)
    {
        //check player 1
        if(currentGame.gameBoard[0][j] == 1 && currentGame.gameBoard[1][j] == 1
           && currentGame.gameBoard[2][j] == 1 && currentGame.gameBoard[3][j] == 1)
        {currentGame.player1Score++;}
        if(currentGame.gameBoard[1][j] == 1 && currentGame.gameBoard[2][j] == 1
           && currentGame.gameBoard[3][j] == 1 && currentGame.gameBoard[4][j] == 1)
        {currentGame.player1Score++;}
        if(currentGame.gameBoard[2][j] == 1 && currentGame.gameBoard[3][j] == 1
           && currentGame.gameBoard[4][j] == 1 && currentGame.gameBoard[5][j] == 1)
        {currentGame.player1Score++;}
        //check player 2
        if(currentGame.gameBoard[0][j] == 2 && currentGame.gameBoard[1][j] == 2
           && currentGame.gameBoard[2][j] == 2 && currentGame.gameBoard[3][j] == 2)
        {currentGame.player2Score++;}
        if(currentGame.gameBoard[1][j] == 2 && currentGame.gameBoard[2][j] == 2
           && currentGame.gameBoard[3][j] == 2 && currentGame.gameBoard[4][j] == 2)
        {currentGame.player2Score++;}
        if(currentGame.gameBoard[2][j] == 2 && currentGame.gameBoard[3][j] == 2
           && currentGame.gameBoard[4][j] == 2 && currentGame.gameBoard[5][j] == 2)
        {currentGame.player2Score++;}
    }

    //check diagonally

    //check player 1
    if(currentGame.gameBoard[2][0] == 1 && currentGame.gameBoard[3][1] == 1
       && currentGame.gameBoard[4][2] == 1 && currentGame.gameBoard[5][3] == 1)
    {currentGame.player1Score++;}
    if(currentGame.gameBoard[1][0] == 1 && currentGame.gameBoard[2][1] == 1
       && currentGame.gameBoard[3][2] == 1 && currentGame.gameBoard[4][3] == 1)
    {currentGame.player1Score++;}
    if(currentGame.gameBoard[2][1] == 1 && currentGame.gameBoard[3][2] == 1
       && currentGame.gameBoard[4][3] == 1 && currentGame.gameBoard[5][4] == 1)
    {currentGame.player1Score++;}
    if(currentGame.gameBoard[0][0] == 1 && currentGame.gameBoard[1][1] == 1
       && currentGame.gameBoard[2][2] == 1 && currentGame.gameBoard[3][3] == 1)
    {currentGame.player1Score++;}
    if(currentGame.gameBoard[1][1] == 1 && currentGame.gameBoard[2][2] == 1
       && currentGame.gameBoard[3][3] == 1 && currentGame.gameBoard[4][4] == 1)
    {currentGame.player1Score++;}
    if(currentGame.gameBoard[2][2] == 1 && currentGame.gameBoard[3][3] == 1
       && currentGame.gameBoard[4][4] == 1 && currentGame.gameBoard[5][5] == 1)
    {currentGame.player1Score++;}
    if(currentGame.gameBoard[0][1] == 1 && currentGame.gameBoard[1][2] == 1
       && currentGame.gameBoard[2][3] == 1 && currentGame.gameBoard[3][4] == 1)
    {currentGame.player1Score++;}
    if(currentGame.gameBoard[1][2] == 1 && currentGame.gameBoard[2][3] == 1
       && currentGame.gameBoard[3][4] == 1 && currentGame.gameBoard[4][5] == 1)
    {currentGame.player1Score++;}
    if(currentGame.gameBoard[2][3] == 1 && currentGame.gameBoard[3][4] == 1
       && currentGame.gameBoard[4][5] == 1 && currentGame.gameBoard[5][6] == 1)
    {currentGame.player1Score++;}
    if(currentGame.gameBoard[0][2] == 1 && currentGame.gameBoard[1][3] == 1
       && currentGame.gameBoard[2][4] == 1 && currentGame.gameBoard[3][5] == 1)
    {currentGame.player1Score++;}
    if(currentGame.gameBoard[1][3] == 1 && currentGame.gameBoard[2][4] == 1
       && currentGame.gameBoard[3][5] == 1 && currentGame.gameBoard[4][6] == 1)
    {currentGame.player1Score++;}
    if(currentGame.gameBoard[0][3] == 1 && currentGame.gameBoard[1][4] == 1
       && currentGame.gameBoard[2][5] == 1 && currentGame.gameBoard[3][6] == 1)
    {currentGame.player1Score++;}

    if(currentGame.gameBoard[0][3] == 1 && currentGame.gameBoard[1][2] == 1
       && currentGame.gameBoard[2][1] == 1 && currentGame.gameBoard[3][0] == 1)
    {currentGame.player1Score++;}
    if(currentGame.gameBoard[0][4] == 1 && currentGame.gameBoard[1][3] == 1
       && currentGame.gameBoard[2][2] == 1 && currentGame.gameBoard[3][1] == 1)
    {currentGame.player1Score++;}
    if(currentGame.gameBoard[1][3] == 1 && currentGame.gameBoard[2][2] == 1
       && currentGame.gameBoard[3][1] == 1 && currentGame.gameBoard[4][0] == 1)
    {currentGame.player1Score++;}
    if(currentGame.gameBoard[0][5] == 1 && currentGame.gameBoard[1][4] == 1
       && currentGame.gameBoard[2][3] == 1 && currentGame.gameBoard[3][2] == 1)
    {currentGame.player1Score++;}
    if(currentGame.gameBoard[1][4] == 1 && currentGame.gameBoard[2][3] == 1
       && currentGame.gameBoard[3][2] == 1 && currentGame.gameBoard[4][1] == 1)
    {currentGame.player1Score++;}
    if(currentGame.gameBoard[2][3] == 1 && currentGame.gameBoard[3][2] == 1
       && currentGame.gameBoard[4][1] == 1 && currentGame.gameBoard[5][0] == 1)
    {currentGame.player1Score++;}
    if(currentGame.gameBoard[0][6] == 1 && currentGame.gameBoard[1][5] == 1
       && currentGame.gameBoard[2][4] == 1 && currentGame.gameBoard[3][3] == 1)
    {currentGame.player1Score++;}
    if(currentGame.gameBoard[1][5] == 1 && currentGame.gameBoard[2][4] == 1
       && currentGame.gameBoard[3][3] == 1 && currentGame.gameBoard[4][2] == 1)
    {currentGame.player1Score++;}
    if(currentGame.gameBoard[2][4] == 1 && currentGame.gameBoard[3][3] == 1
       && currentGame.gameBoard[4][2] == 1 && currentGame.gameBoard[5][1] == 1)
    {currentGame.player1Score++;}
    if(currentGame.gameBoard[1][6] == 1 && currentGame.gameBoard[2][5] == 1
       && currentGame.gameBoard[3][4] == 1 && currentGame.gameBoard[4][3] == 1)
    {currentGame.player1Score++;}
    if(currentGame.gameBoard[2][5] == 1 && currentGame.gameBoard[3][4] == 1
       && currentGame.gameBoard[4][3] == 1 && currentGame.gameBoard[5][2] == 1)
    {currentGame.player1Score++;}
    if(currentGame.gameBoard[2][6] == 1 && currentGame.gameBoard[3][5] == 1
       && currentGame.gameBoard[4][4] == 1 && currentGame.gameBoard[5][3] == 1)
    {currentGame.player1Score++;}

    //check player 2
    if(currentGame.gameBoard[2][0] == 2 && currentGame.gameBoard[3][1] == 2
       && currentGame.gameBoard[4][2] == 2 && currentGame.gameBoard[5][3] == 2)
    {currentGame.player2Score++;}
    if(currentGame.gameBoard[1][0] == 2 && currentGame.gameBoard[2][1] == 2
       && currentGame.gameBoard[3][2] == 2 && currentGame.gameBoard[4][3] == 2)
    {currentGame.player2Score++;}
    if(currentGame.gameBoard[2][1] == 2 && currentGame.gameBoard[3][2] == 2
       && currentGame.gameBoard[4][3] == 2 && currentGame.gameBoard[5][4] == 2)
    {currentGame.player2Score++;}
    if(currentGame.gameBoard[0][0] == 2 && currentGame.gameBoard[1][1] == 2
       && currentGame.gameBoard[2][2] == 2 && currentGame.gameBoard[3][3] == 2)
    {currentGame.player2Score++;}
    if(currentGame.gameBoard[1][1] == 2 && currentGame.gameBoard[2][2] == 2
       && currentGame.gameBoard[3][3] == 2 && currentGame.gameBoard[4][4] == 2)
    {currentGame.player2Score++;}
    if(currentGame.gameBoard[2][2] == 2 && currentGame.gameBoard[3][3] == 2
       && currentGame.gameBoard[4][4] == 2 && currentGame.gameBoard[5][5] == 2)
    {currentGame.player2Score++;}
    if(currentGame.gameBoard[0][1] == 2 && currentGame.gameBoard[1][2] == 2
       && currentGame.gameBoard[2][3] == 2 && currentGame.gameBoard[3][4] == 2)
    {currentGame.player2Score++;}
    if(currentGame.gameBoard[1][2] == 2 && currentGame.gameBoard[2][3] == 2
       && currentGame.gameBoard[3][4] == 2 && currentGame.gameBoard[4][5] == 2)
    {currentGame.player2Score++;}
    if(currentGame.gameBoard[2][3] == 2 && currentGame.gameBoard[3][4] == 2
       && currentGame.gameBoard[4][5] == 2 && currentGame.gameBoard[5][6] == 2)
    {currentGame.player2Score++;}
    if(currentGame.gameBoard[0][2] == 2 && currentGame.gameBoard[1][3] == 2
       && currentGame.gameBoard[2][4] == 2 && currentGame.gameBoard[3][5] == 2)
    {currentGame.player2Score++;}
    if(currentGame.gameBoard[1][3] == 2 && currentGame.gameBoard[2][4] == 2
       && currentGame.gameBoard[3][5] == 2 && currentGame.gameBoard[4][6] == 2)
    {currentGame.player2Score++;}
    if(currentGame.gameBoard[0][3] == 2 && currentGame.gameBoard[1][4] == 2
       && currentGame.gameBoard[2][5] == 2 && currentGame.gameBoard[3][6] == 2)
    {currentGame.player2Score++;}

    if(currentGame.gameBoard[0][3] == 2 && currentGame.gameBoard[1][2] == 2
       && currentGame.gameBoard[2][1] == 2 && currentGame.gameBoard[3][0] == 2)
    {currentGame.player2Score++;}
    if(currentGame.gameBoard[0][4] == 2 && currentGame.gameBoard[1][3] == 2
       && currentGame.gameBoard[2][2] == 2 && currentGame.gameBoard[3][1] == 2)
    {currentGame.player2Score++;}
    if(currentGame.gameBoard[1][3] == 2 && currentGame.gameBoard[2][2] == 2
       && currentGame.gameBoard[3][1] == 2 && currentGame.gameBoard[4][0] == 2)
    {currentGame.player2Score++;}
    if(currentGame.gameBoard[0][5] == 2 && currentGame.gameBoard[1][4] == 2
       && currentGame.gameBoard[2][3] == 2 && currentGame.gameBoard[3][2] == 2)
    {currentGame.player2Score++;}
    if(currentGame.gameBoard[1][4] == 2 && currentGame.gameBoard[2][3] == 2
       && currentGame.gameBoard[3][2] == 2 && currentGame.gameBoard[4][1] == 2)
    {currentGame.player2Score++;}
    if(currentGame.gameBoard[2][3] == 2 && currentGame.gameBoard[3][2] == 2
       && currentGame.gameBoard[4][1] == 2 && currentGame.gameBoard[5][0] == 2)
    {currentGame.player2Score++;}
    if(currentGame.gameBoard[0][6] == 2 && currentGame.gameBoard[1][5] == 2
       && currentGame.gameBoard[2][4] == 2 && currentGame.gameBoard[3][3] == 2)
    {currentGame.player2Score++;}
    if(currentGame.gameBoard[1][5] == 2 && currentGame.gameBoard[2][4] == 2
       && currentGame.gameBoard[3][3] == 2 && currentGame.gameBoard[4][2] == 2)
    {currentGame.player2Score++;}
    if(currentGame.gameBoard[2][4] == 2 && currentGame.gameBoard[3][3] == 2
       && currentGame.gameBoard[4][2] == 2 && currentGame.gameBoard[5][1] == 2)
    {currentGame.player2Score++;}
    if(currentGame.gameBoard[1][6] == 2 && currentGame.gameBoard[2][5] == 2
       && currentGame.gameBoard[3][4] == 2 && currentGame.gameBoard[4][3] == 2)
    {currentGame.player2Score++;}
    if(currentGame.gameBoard[2][5] == 2 && currentGame.gameBoard[3][4] == 2
       && currentGame.gameBoard[4][3] == 2 && currentGame.gameBoard[5][2] == 2)
    {currentGame.player2Score++;}
    if(currentGame.gameBoard[2][6] == 2 && currentGame.gameBoard[3][5] == 2
       && currentGame.gameBoard[4][4] == 2 && currentGame.gameBoard[5][3] == 2)
    {currentGame.player2Score++;}
}


int main(int argc, char ** argv)
{
      char ** command_line = argv;


      if (argc != 5)
      {
        printf("Four command-line arguments are needed:\n");
        printf("Usage: %s interactive [input_file] [computer-next/human-next] [depth]\n", command_line [0]);
        printf("or:  %s one-move [input_file] [output_file] [depth]\n", command_line [0]);

        return 0;
      }

      /**reference file system for OSx **/

    //    freopen("/Users/anirudha/Desktop/input1.txt","r",stdin);
    //    freopen("/Users/anirudha/Desktop/output.txt","w",stdout);

    /*** Manual testing code starts (commented out) ***/

//    string file_name , gameMode , input_file , who_next_or_output_file , depth;
//
//    cin >> gameMode >> input_file >> who_next_or_output_file >> depth;
//
//    cout << gameMode << endl ;
//    cout << input_file << endl;
//    cout << who_next_or_output_file << endl;
//    cout << depth << endl;
//    int depth_in_int = stringToInt(depth);
//    cout << depth_in_int << endl;
//    const char * input = input_file.c_str();
//    const char * output = who_next_or_output_file.c_str();
//    const char * game_mode = gameMode.c_str();

    /*** Manual testing code ends***/


    char * game_mode = command_line [1];

    int GameModeIndex ;
    if (strcmp(game_mode, "interactive") == 0)
    {

        GameModeIndex = 1;
//        printf("interactive mode is currently not implemented\n");
//        return 0;
    }
    else if (strcmp (game_mode, "one-move") != 0)
    {
        printf("%s is an unrecognized game mode\n", game_mode);
        return 0;
    }
    else
    {
        GameModeIndex = 2;
    }


    char * input = command_line[2];
    char * output = command_line [3];

    string depth_in_string(command_line[4]);

    int depth_in_int  = stringToInt(depth_in_string);

    gameStatus currentGame;	 // Declare current game
    printf("\nMaxConnect-4 game\n");

    currentGame.gameFile = fopen(input, "r");
    printf("game state before move:\n");


    //set currentTurn
    char current = 0;
    int i, j;

    if (currentGame.gameFile != 0)
    {
        for(i = 0; i < 6; i++)
        {
            for(j = 0; j < 7; j++)
            {
                do
                {
                    current = getc(currentGame.gameFile);
                }
                while ((current == ' ') || (current == '\n') || (current == '\r'));

                currentGame.gameBoard[i][j] = current - 48;
                if(currentGame.gameBoard[i][j] > 0)
                {
                    currentGame.pieceCount++;
                }
            }
        }

        do
        {
            current = getc(currentGame.gameFile);
        }
        while ((current == ' ') || (current == '\n') || (current == '\r'));

        currentGame.currentTurn = current - 48;
        fclose(currentGame.gameFile);
    }

    printGameBoard(currentGame);
    countScore(currentGame);
    printf("Score: Player 1 = %d, Player 2 = %d\n\n", currentGame.player1Score, currentGame.player2Score);

    // Seed random number generator
    int seed = time(NULL);
    srand(seed);

    if(currentGame.pieceCount == 42)
    {
        printf("\nBOARD FULL\n");
        printf("Game over!\n\n");

        return 1;
    }


    /**** Unit testing code for AI starts****/
//    while(currentGame.pieceCount <42)
//    {
//        aiPlay(currentGame , depth_in_int);
//        printf("game state after move:\n");
//        printGameBoard(currentGame);
//        countScore(currentGame);
//        printf("Score: Player 1 = %d, Player 2 = %d\n\n", currentGame.player1Score, currentGame.player2Score);
//    }
    /**** Unit testing code for AI ends****/


    // If using the upper unit testing code please comment out the below part
    if(GameModeIndex == 2)
    {
        aiPlay(currentGame , depth_in_int);
        printf("game state after move:\n");
        printGameBoard(currentGame);
        countScore(currentGame);
        printf("Score: Player 1 = %d, Player 2 = %d\n\n", currentGame.player1Score, currentGame.player2Score);


        currentGame.gameFile = fopen(output, "w");
        if (currentGame.gameFile != 0)
        {
            printGameBoardToFile(currentGame);
            fclose(currentGame.gameFile);
        }
        else
        {
            printf("error: could not open output file %s\n", output);
        }
    }
    else if(GameModeIndex == 1)
    {
        char *first_player = command_line[3];

        int first_player_index ;

        if(strcmp (output, "computer-next") == 0)
        {
            while(currentGame.pieceCount<42)
            {
                aiPlay(currentGame , depth_in_int);
                printf("game state after move:\n");
                printGameBoard(currentGame);
                countScore(currentGame);
                printf("Score: Player 1 = %d, Player 2 = %d\n\n", currentGame.player1Score, currentGame.player2Score);


                currentGame.gameFile = fopen("computer.txt", "w");
                if (currentGame.gameFile != 0)
                {
                    printGameBoardToFile(currentGame);
                    fclose(currentGame.gameFile);
                }
                else
                {
                    printf("error: could not open computer.txt file %s\n", output);
                }

                if(currentGame.pieceCount >= 42)
                {
                    return 0 ;
                }

                printf("Please type a row between 1 to 7 and press enter to make your move\n");

                int moves;

                scanf("%d",&moves);

                while(moves<1 || moves > 7)
                {
                    printf("This is not a valid move . Please type a row between 1 to 7 and press enter to make your move\n");
                    scanf("%d",&moves);
                }
                while(playPiece(moves , currentGame) == 0)
                {
                    printf("Your selected column is full please enter another value\n");
                    scanf("%d",&moves);
                }

                printf("\n\nmove %d: Player %li, column %d\n",currentGame.pieceCount, currentGame.currentTurn, moves);

                if(currentGame.currentTurn == 1)
                    currentGame.currentTurn = 2;
                else if (currentGame.currentTurn == 2)
                    currentGame.currentTurn = 1;


                printf("game state after move:\n");
                printGameBoard(currentGame);
                countScore(currentGame);
                printf("Score: Player 1 = %d, Player 2 = %d\n\n", currentGame.player1Score, currentGame.player2Score);


                currentGame.gameFile = fopen("human.txt", "w");
                if (currentGame.gameFile != 0)
                {
                    printGameBoardToFile(currentGame);
                    fclose(currentGame.gameFile);
                }
                else
                {
                    printf("error: could not open human.txt file %s\n", output);
                }
                if(currentGame.pieceCount >= 42)
                {
                    return 0 ;
                }
            }

        }
        else if(strcmp (output, "human-next") == 0)
        {
            while(currentGame.pieceCount<42)
            {
                printf("Please type a row between 1 to 7 and press enter to make your move\n");

                int moves;

                scanf("%d",&moves);

                while(moves<1 || moves > 7)
                {
                    printf("This is not a valid move . Please type a row between 1 to 7 and press enter to make your move\n");
                    scanf("%d",&moves);
                }
                while(playPiece(moves , currentGame) == 0)
                {
                    printf("Your selected column is full please enter another value\n");
                    scanf("%d",&moves);
                }

                printf("\n\nmove %d: Player %li, column %d\n",currentGame.pieceCount, currentGame.currentTurn, moves);

                if(currentGame.currentTurn == 1)
                    currentGame.currentTurn = 2;
                else if (currentGame.currentTurn == 2)
                    currentGame.currentTurn = 1;


                printf("game state after move:\n");
                printGameBoard(currentGame);
                countScore(currentGame);
                printf("Score: Player 1 = %d, Player 2 = %d\n\n", currentGame.player1Score, currentGame.player2Score);


                currentGame.gameFile = fopen("human.txt", "w");
                if (currentGame.gameFile != 0)
                {
                    printGameBoardToFile(currentGame);
                    fclose(currentGame.gameFile);
                }
                else
                {
                    printf("error: could not open human.txt file %s\n", output);
                }
                if(currentGame.pieceCount >= 42)
                {
                    return 0 ;
                }

                aiPlay(currentGame , depth_in_int);
                printf("game state after move:\n");
                printGameBoard(currentGame);
                countScore(currentGame);
                printf("Score: Player 1 = %d, Player 2 = %d\n\n", currentGame.player1Score, currentGame.player2Score);


                currentGame.gameFile = fopen("computer.txt", "w");
                if (currentGame.gameFile != 0)
                {
                    printGameBoardToFile(currentGame);
                    fclose(currentGame.gameFile);
                }
                else
                {
                    printf("error: could not open computer.txt file %s\n", output);
                }

                if(currentGame.pieceCount >= 42)
                {
                    return 0 ;
                }
            }
        }
    }


    return 1;
}


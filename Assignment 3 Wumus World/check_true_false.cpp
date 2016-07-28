#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <stack>
#include <queue>
#include <vector>

using namespace std;

#define FLAG "FLAG"
#define ZERO "ZERO"
#define ONE "ONE"

int truthTable[65536][67];
int constantKBmarker[67];

/************************************* Find Column ************************************/

//This function find the column for any expression in the Knowledge Base Grid

int findColumn(string input)
{
    if(input == ZERO)
    {
        return 66;
    }
    else if(input == ONE)
    {
        return 65;
    }
    int row , column ;
    int foundColumn ;

    row = input[2]-'0' - 1;
    column = input[4] - '0';

    if(input[0]== 'M')
    {
        foundColumn = row * 4 + column;
    }
    else if(input[0] == 'S')
    {
        foundColumn = row * 4 + column + 16;
    }
    else if(input[0] == 'P')
    {
        foundColumn = row * 4 + column + 32;
    }
    else if(input[0] == 'B')
    {
        foundColumn = row * 4 + column + 48;
    }
    return foundColumn;
}

/********************** Evaluate Expression Without If/IFF **************************/

//This function evaluate functions without if or iff and update the Knowledge Base

string evaluateExpression(string input , int row)
{
    unsigned long input_length = input.length();

    stack <string> value_stack;
    stack <string> expression_stack;

    string expression = "";
    bool openBracketBefore = false;
    bool closeBracketBefore = false;

    for(int i = 0 ; i < input_length ; i++)
    {
        if(input[i]=='(')
        {
            value_stack.push(FLAG);
            openBracketBefore = true;
            expression = "";
        }
        else if (input[i] == ' ')
        {
            if(openBracketBefore)
            {
                expression_stack.push(expression);
                openBracketBefore = false;
                expression = "";
            }
            else if(closeBracketBefore)
            {
                expression = "";
                closeBracketBefore = false;
            }
            else
            {
                value_stack.push(expression);
                expression = "";
            }
        }
        else if(input[i] == ')')
        {
            if(expression == "and" || expression == "or" || expression == "xor")
            {
                expression_stack.push(expression);
                expression = "";
            }
            else if(expression != "")
            {
                value_stack.push(expression);
                expression = "";
            }
            string command = expression_stack.top();
            expression_stack.pop();

            closeBracketBefore = true;

            if(command =="and")
            {
                int ans = 1 ;

                if(value_stack.top() == FLAG)
                {
                    value_stack.pop();
                    value_stack.push(ONE);
                }
                else
                {
                    while(value_stack.top()!= FLAG)
                    {
                        string value = value_stack.top();
                        value_stack.pop();

                        if(truthTable[row][findColumn(value)] == 0)
                        {
                            ans = 0;
                        }
                    }
                    if(value_stack.top() == FLAG)
                    {
                        value_stack.pop();
                    }

                    if(ans == 0)
                    {
                        value_stack.push(ZERO);
                    }
                    else
                    {
                        value_stack.push(ONE);
                    }
                }

            }
            else if(command =="or")
            {
                int ans = 0 ;

                if(value_stack.top() == FLAG)
                {
                    value_stack.pop();
                    value_stack.push(ZERO);
                }
                else
                {
                    while(value_stack.top()!= FLAG)
                    {
                        string value = value_stack.top();
                        value_stack.pop();

                        if(truthTable[row][findColumn(value)] == 1)
                        {
                            ans = 1;
                        }
                    }
                    if(value_stack.top() == FLAG)
                    {
                        value_stack.pop();
                    }
                    if(ans == 0)
                    {
                        value_stack.push(ZERO);
                    }
                    else
                    {
                        value_stack.push(ONE);
                    }
                }


            }
            else if(command =="xor")
            {
                int ans = 0 ;
                int counter  = 0;

                if(value_stack.top() == FLAG)
                {
                    value_stack.pop();
                    value_stack.push(ZERO);
                }
                else
                {
                    while(value_stack.top()!= FLAG)
                    {
                        string value = value_stack.top();
                        value_stack.pop();

                        if(truthTable[row][findColumn(value)] == 1)
                        {
                            counter++;
                        }
                    }
                    if(counter == 1)
                    {
                        ans = 1;
                    }
                    if(value_stack.top() == FLAG)
                    {
                        value_stack.pop();
                    }
                    if(ans == 0)
                    {
                        value_stack.push(ZERO);
                    }
                    else
                    {
                        value_stack.push(ONE);
                    }
                }
            }
        }
        else
        {
            expression += input[i];
        }

    }
    if(value_stack.top() == ZERO)
    {
        return ZERO;
    }
    else
    {
        return ONE;
    }

}

/*********************** Evaluate function with If and Iff ***************/

//This function evaluate function with if or iff

string evaluateExpressionWithIf(string input , int row)
{
    string expression1 ;
    string expression2 ;

    bool iff = false;


    int i ;
    if(input[3] == ' ')
    {
        i = 4;
    }
    else if(input[3] == 'f')
    {
        iff = true ;
        i = 5;
    }
    int counter = 0;


    if(input[i] == '(')
    {
        expression1 += input[i];
        counter++;
        i++;
        while(counter>0)
        {
            if(input[i] == '(')
            {
                counter++;
            }
            else if(input[i] == ')')
            {
                counter--;
            }
            expression1 += input[i];
            i++;
        }
    }
    else
    {
        while(input[i] != ' ')
        {
            expression1 += input[i];
            i++;
        }
    }

    i++;
    counter = 0;

    if(input[i] == '(')
    {
        expression2 += input[i];
        counter++;
        i++;
        while(counter>0)
        {
            if(input[i] == '(')
            {
                counter++;
            }
            else if(input[i] == ')')
            {
                counter--;
            }
            expression2 += input[i];
            i++;
        }
    }
    else
    {
        while(input[i] != ' '&&input[i] !=')')
        {
            expression2 += input[i];
            i++;
        }
    }

    int condition , result ;

    if(expression1[0] != '(')
    {
        condition = truthTable[row][findColumn(expression1)];
    }
    else
    {
        string cond = evaluateExpression(expression1, row);
        if(cond == ZERO)
        {
            condition = 0;
        }
        else
        {
            condition = 1;
        }
    }

    if(expression2[0] != '(')
    {
        result = truthTable[row][findColumn(expression2)];
    }
    else
    {
        string res = evaluateExpression(expression2, row);
        if(res == ZERO)
        {
            result = 0;
        }
        else
        {
            result = 1;
        }
    }

    if(iff)
    {
        if((condition == 1 && result == 0) || (condition == 0 && result == 1))
        {
            return ZERO;
        }
        else
        {
            return ONE;
        }
    }
    else
    {
        if((condition == 1 && result == 0))
        {
            return ZERO;
        }
        else
        {
            return ONE;
        }
    }

}

/***************************************** Main Function ***********************************/

int main(int argc, char ** argv)
{
    char ** command_line = argv;
    if(argc != 4)
    {
        // take two arguments
        printf("Usage: %s [wumpus-rules-file] [additional-knowledge-file] [input_file]\n", command_line [0]);
        return 0;
    }

    memset(truthTable, -1, sizeof(truthTable));
    memset(constantKBmarker , 0 , sizeof(constantKBmarker));

    queue <string> unprocessedExpression;
    queue <string> unprocessedExpressionWithIf;

    ifstream KBin(command_line[2]);
    ifstream wumpusRules(command_line[1]);
    ifstream statementFile(command_line[3]);
    ofstream resultFile("result.txt");

    string input;

    for(int i = 0 ; i < 65536 ; i++)
    {
        truthTable[i][0] = 1;
        truthTable[i][65] = 1;
        truthTable[i][66] = 0;

    }
    printf("Loading additional knowledge base....\n");
    while(getline(KBin , input))
    {
        unsigned long inputLength = input.length();


        if(input[0] == '#'||input == "")
        {
            continue;
        }
        else if(inputLength == 5)
        {
            string expression = "";

            for(int j = 0 ; j < inputLength ; j++)
            {
                expression += input[j];
            }

            int column = findColumn(expression);
            constantKBmarker[column] = 1;

            for(int  j = 0 ; j < 65536 ; j++)
            {
                if(truthTable[j][column] == -1 || truthTable[j][column] == 1)
                {
                    truthTable[j][column] = 1;
                }
                else
                {
                    truthTable[j][0] = 0;
                }
            }
        }

        else if(inputLength == 11 && input[0] == '(' && input[1] == 'n')
        {
            string expression = "";

            for(int j = 5 ; j < inputLength-1 ; j++)
            {
                expression += input[j];
            }

            int column = findColumn(expression);
            constantKBmarker[column] = 1;

            for(int  j = 0 ; j < 65536 ; j++)
            {
                if(truthTable[j][column] == -1 || truthTable[j][column] == 0)
                {
                    truthTable[j][column] = 0;
                }
                else
                {
                    truthTable[j][0] = 0;
                }
            }
        }
        else if(input[1] == 'i')
        {
            string expression = "";
            for(int i = 0 ; i < inputLength ; i++)
            {
                expression += input[i];
            }
            unprocessedExpressionWithIf.push(expression);
        }
        else
        {
            string expression = "";
            for(int i = 0 ; i < inputLength ; i++)
            {
                expression += input[i];
            }
            unprocessedExpression.push(expression);
        }

    }
    printf("Finished loading additional knowledge base....\n");
    printf("Loading Wompus Rules....\n");
    while(getline(wumpusRules , input))
    {
        unsigned long inputLength = input.length();


        if(input[0] == '#'||input == "")
        {
            continue;
        }
        else if(inputLength == 5)
        {
            string expression = "";

            for(int j = 0 ; j < inputLength ; j++)
            {
                expression += input[j];
            }

            int column = findColumn(expression);
            constantKBmarker[column] = 1;


            for(int  j = 0 ; j < 65536 ; j++)
            {
                truthTable[j][column] = 1;
            }
        }

        else if(inputLength == 11 && input[0] == '(' && input[1] == 'n')
        {
            string expression = "";

            for(int j = 5 ; j < inputLength-1 ; j++)
            {
                expression += input[j];
            }

            int column = findColumn(expression);
            constantKBmarker[column] = 1;

            for(int  j = 0 ; j < 65536 ; j++)
            {
                truthTable[j][column] = 0;
            }
        }
        else if(input[1] == 'i')
        {
            string expression = "";
            for(int i = 0 ; i < inputLength ; i++)
            {
                expression += input[i];
            }
            unprocessedExpressionWithIf.push(expression);
        }
        else
        {
            string expression = "";
            for(int i = 0 ; i < inputLength ; i++)
            {
                expression += input[i];
            }
            unprocessedExpression.push(expression);
        }
    }

    printf("Finished loading Wompus Rules....\n");

    constantKBmarker[0] = 1;
    constantKBmarker[65] = 1;
    constantKBmarker[66] = 1;

    int constantFactor = 1;

    printf("Processing all the data....\n");
    for(int i = 0 ; i < 67 ; i++)
    {
        if(constantKBmarker[i] == 0)
        {
            int counter = 0;
            int tempCon = constantFactor;
            while(counter < 65536)
            {
                while(tempCon>0 && counter<65536)
                {
                    truthTable[counter][i] = 0;
                    counter++;
                    tempCon--;
                }
                tempCon = constantFactor;
                while(tempCon>0 && counter < 65536)
                {
                    truthTable[counter][i] = 1;
                    counter++;
                    tempCon--;
                }
                tempCon = constantFactor;
            }
            constantFactor *=2;
        }
    }

    while(unprocessedExpression.empty()!=true)
    {
        string current_expression = unprocessedExpression.front();

        string value ;

        unprocessedExpression.pop();

        for(int i = 0 ; i < 65536 ; i++)
        {
            if(truthTable[i][0] == 1)
            {
                string result = evaluateExpression(current_expression , i);

                if(result == ZERO)
                {
                    truthTable[i][0] = 0;
                }
            }
        }
    }
    while(unprocessedExpressionWithIf.empty()!=true)
    {

        string current_expression = unprocessedExpressionWithIf.front();
        string value ;

        unprocessedExpressionWithIf.pop();

        for(int i = 0 ; i < 65536 ; i++)
        {
            if(truthTable[i][0] == 1)
            {
                string result = evaluateExpressionWithIf(current_expression , i);

                if(result == ZERO)
                {
                    truthTable[i][0] = 0;
                }
            }
        }

    }

    printf("Loading statement....\n");
        getline(statementFile , input);

        bool foundTrue = false;
        bool foundFalse = false;

        unsigned long inputLength = input.length();



        if(inputLength == 5)
        {
            string expression = "";

            for(int j = 0 ; j < inputLength ; j++)
            {
                expression += input[j];
            }

            int column = findColumn(expression);
            constantKBmarker[column] = 1;

            for(int  j = 0 ; j < 65536 ; j++)
            {
                if(truthTable[j][0] == 1)
                {
                    if(truthTable[j][column] == 1)
                    {
                        foundTrue = true;
                    }
                    else if(truthTable[j][column] == 0)
                    {
                        foundFalse = true;
                    }
                }
            }
        }

        else if(inputLength == 11 && input[0] == '(' && input[1] == 'n')
        {
            string expression = "";

            for(int j = 5 ; j < inputLength-1 ; j++)
            {
                expression += input[j];
            }

            int column = findColumn(expression);
            constantKBmarker[column] = 1;

            for(int  j = 0 ; j < 65536 ; j++)
            {
                if(truthTable[j][0] == 1)
                {
                    if(truthTable[j][column] == 0)
                    {
                        foundTrue = true;
                    }
                    else
                    {
                        foundFalse = true;
                    }
                }
            }
        }
        else if(input[1] == 'i')
        {
            for(int i = 0 ; i < 65536 ; i++)
            {
                if(truthTable[i][0] == 1)
                {
                    string result = evaluateExpressionWithIf(input , i);

                    if(result == ZERO)
                    {
                        foundFalse = true;
                    }
                    else
                    {
                        foundTrue = true;
                    }
                }
            }
        }
        else
        {
            for(int i = 0 ; i < 65536 ; i++)
            {
                if(truthTable[i][0] == 1)
                {
                    string result = evaluateExpression(input , i);

                    if(result == ZERO)
                    {
                        foundFalse = true;
                    }
                    else
                    {
                        foundTrue = true;
                    }
                }
            }

        }

//        ofstream TT("truthTable.txt");
//
//        for(int i = 0 ; i < 65536 ; i++)
//            {
//                if(truthTable[i][0] == 1)
//                {
//                    for(int j = 0 ; j < 67 ; j++)
//                    {
//                        TT << truthTable[i][j] ;
//                    }
//                    TT << endl;
//                }
//            }
        if(foundTrue == false && foundFalse == false)
        {
            resultFile << "both true and false" << endl;
        }
        else if(foundTrue ==true && foundFalse == true)
        {
            resultFile << "possibly true , possibly false" << endl;
        }
        else if(foundTrue == true)
        {
            resultFile << "definitely true" << endl;
        }
        else if(foundFalse == true)
        {
            resultFile << "definitely false" << endl;
        }


    printf("Finished writing answer to the text file!\n");
    return 0;

}

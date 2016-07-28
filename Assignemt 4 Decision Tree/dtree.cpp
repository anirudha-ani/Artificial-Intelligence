#include <bits/stdc++.h>

using namespace std;

#define PRUNING_THR 50
int COUNTER = 0;
struct distributionNode
{
    double class_no;
    int occurance;
};

struct answerNode
{
    double ans;
    int occurance;
};
struct node
{
	double threshold;
	int attribute;
	bool isLeaf;
	int decidedClass;
	vector <distributionNode> distListForThisNode;
	node* leftChild;
	node* rightChild;
};

bool sortByOccurance(distributionNode x , distributionNode y)
{
    return x.occurance > y.occurance;
}

bool sortResultByOccurance(answerNode x , answerNode y)
{
    return x.occurance > y.occurance;
}

void readFile(vector<vector <double> > &dataTable , string fileName );
void defaultClassFinder(const vector<vector <double> > &dataTable , vector <distributionNode> &distributionList);
void findMinmax(vector<vector <double> > &dataTable, int attributeIndex, double &Low, double &Max);
node* buildDecisionTree(vector<vector <double> > &dataTable, vector <distributionNode> &distributionList);
void CHOOSE_ATTRIBUTE(vector<vector <double> > &dataTable, int& best_attribute, double &best_threshold);
double INFORMATION_GAIN(vector<vector <double> >& dataTable, int attribute, double threshold);
double testData(vector <double> &singleLine, node* tree);
int tree_ID = 0;
double MAX_GAIN ;
int mode_no ;

/********************************* MAIN FUNCTION *************************************/

int main(int argc, char ** argv)
{
    srand (time(NULL));

    char ** command_line = argv;


      if (argc != 4)
      {
        printf("Three command-line arguments are needed:\n");
        printf("Usage: %s [training_file] [test_fil] [optimized/randomized/forest3/forest15]\n", command_line [0]);
        return 0;
      }



    char * game_mode = command_line [3];

    if (strcmp(game_mode, "optimized") == 0)
    {
        mode_no = 1;
    }
    else if (strcmp(game_mode, "randomized") == 0)
    {
        mode_no = 2;
    }
    else if (strcmp(game_mode, "forest3") == 0)
    {
        mode_no = 3;
    }
    else if (strcmp(game_mode, "forest15") == 0)
    {
        mode_no = 4;
    }
    else
    {
        printf("Please enter valid command  [optimized/randomized/forest3/forest15]\n");
        printf("Three command-line arguments are needed:\n");
        printf("Usage: %s [training_file] [test_fil] [optimized/randomized/forest3/forest15]\n", command_line [0]);
        return 0;
    }
    vector <vector<double> > training_data ;
    string training_file(command_line[1]);
    readFile(training_data , training_file);

    vector<vector <double> > test_data;
    string test(command_line[2]);
    readFile(test_data, test);

    if(mode_no == 1 || mode_no == 2)
    {
        vector <distributionNode> distributionList;
        defaultClassFinder(training_data , distributionList);

        node* tree = new node;
        tree = buildDecisionTree(training_data, distributionList);



        int column = test_data[0].size();
        int row = test_data.size() ;
        int counter = 0;

        double percentage ;

        for (int i = 0; i < row ; i++)
        {
            double ans = testData(test_data[i], tree);

            if(test_data[i][column - 1] - ans == 0)
            {
                counter ++ ;
                printf("ID = %d, predicted = %f, true = %f , accuracy = 1.0\n" , i , ans ,test_data[i][column - 1]);
            }
            else
            {
                 printf("ID = %d, predicted = %f, true = %f , accuracy = 0.0\n" , i , ans ,test_data[i][column - 1] );
            }
        }

        percentage = 1.0 * counter / row ;

        printf("classification accuracy = %f\n" , percentage);
    }

    else if(mode_no == 3)
    {
        vector <distributionNode> distributionList;
        defaultClassFinder(training_data , distributionList);

        node* tree[3] ;
        tree[0] = buildDecisionTree(training_data, distributionList);
        tree_ID++;

        tree[1] = buildDecisionTree(training_data, distributionList);
        tree_ID++;

        tree[2] = buildDecisionTree(training_data, distributionList);

        int column = test_data[0].size();
        int row = test_data.size() ;
        int counter = 0;

        double percentage ;
        double l;

        int class_counter = 1;


        for (int i = 0; i < row ; i++)
        {
            map <int , int > result_in_index;
            vector <answerNode> resultList;

            for(int j = 0 ; j < 3 ; j++)
            {
                 l = testData(test_data[i], tree[j]);

                 if(result_in_index[l] == 0)
                 {
                     result_in_index[l] = class_counter;
                     answerNode newNode;

                     newNode.ans = l;
                     newNode.occurance = 1;

                     resultList.push_back(newNode);
                 }
                 else
                 {
                     resultList[l].occurance++;
                 }
            }
            sort(resultList.begin() , resultList.end() , sortResultByOccurance);

            if(resultList[0].ans ==(int) test_data[i][column - 1])
            {
                counter ++ ;
                printf("ID = %d, predicted = %f, true = %f , accuracy = 1.0\n" , i , resultList[0].ans ,test_data[i][column - 1] );
            }
            else
            {
                 printf("ID = %d, predicted = %f, true = %f , accuracy = 0.0\n" , i ,resultList[0].ans ,test_data[i][column - 1] );
            }
        }
        percentage = 1.0 * counter / row ;

        printf("classification accuracy = %f\n" , percentage);
    }
    else if(mode_no == 4)
    {
        vector <distributionNode> distributionList;
        defaultClassFinder(training_data , distributionList);

        node* tree[15] ;

        for(int i = 0 ; i < 15 ; i++)
        {
            tree[i] = buildDecisionTree(training_data, distributionList);
            tree_ID++;
        }

        int column = test_data[0].size();
        int row = test_data.size() ;
        int counter = 0;

        double percentage ;
        double l;

        int class_counter = 1;


        for (int i = 0; i < row ; i++)
        {
            map <int , int > result_in_index;
            vector <answerNode> resultList;

            for(int j = 0 ; j < 15 ; j++)
            {
                 l = testData(test_data[i], tree[j]);
                 //cout << "Result = " << l << endl;
                 if(result_in_index[l] == 0)
                 {
                     result_in_index[l] = class_counter;
                     answerNode newNode;

                     newNode.ans = l;
                     newNode.occurance = 1;

                     resultList.push_back(newNode);
                 }
                 else
                 {
                     resultList[l].occurance++;
                 }
            }
            sort(resultList.begin() , resultList.end() , sortResultByOccurance);

            if(resultList[0].ans ==(int) test_data[i][column - 1])
            {
                counter ++ ;
                printf("ID = %d, predicted = %f, true = %f , accuracy = 1.0\n" , i , resultList[0].ans ,test_data[i][column - 1] );
            }
            else
            {
                 printf("ID = %d, predicted = %f, true = %f , accuracy = 0.0\n" , i ,resultList[0].ans ,test_data[i][column - 1] );
            }
        }
        percentage = 1.0 * counter / row ;

        printf("classification accuracy = %f\n" , percentage);
    }

    return 0;
}

/************************************* READ FILE ***************************************/

void readFile(vector<vector<double> > &dataTable , string fileName )
{
    vector<string> dataFileString;

    ifstream code(fileName.c_str());

    string inputLine;

    if(code.is_open() == true)
    {
        while(getline(code, inputLine))
        {
            dataFileString.push_back(inputLine);
        }
    }
    else
    {
        printf("%s file not found\n");
    }

    code.close();

    double number = 0;
    for(int i =0 ; i < dataFileString.size(); i++)
    {
        vector<double> training_data_rows;
        stringstream ss(dataFileString[i]);

        int j = 0;
        while(ss >> number)
        {
            training_data_rows.push_back(number);
        }

        dataTable.push_back(training_data_rows);
    }

    dataFileString.clear();
}

/************************************** DEFAULT CLASS FINDER *********************************/

/*
      Take an empty vector of "distributionNode" data type
      and return it sorted with the highest occurred class at index 0
*/


void defaultClassFinder(const vector<vector <double> > &dataTable , vector <distributionNode> &distributionList)
{

    int classIndex = dataTable[0].size()-1;

    int row = dataTable.size() ;

    int current_max = 0;

    int default_class = dataTable[0][classIndex];
    int counter = 1;


    map<double , int> class_in_index;

    for(int i = 0 ; i < row ; i++)
    {
        if(class_in_index[dataTable[i][classIndex]] == 0)
        {
            class_in_index[dataTable[i][classIndex]] = counter;

            distributionNode newNode;

            newNode.class_no =  dataTable[i][classIndex];
            newNode.occurance = 1;

            distributionList.push_back(newNode);
            counter++;
        }
        else
        {
            distributionList[class_in_index[dataTable[i][classIndex]] - 1].occurance++;
        }
    }
    sort(distributionList.begin() , distributionList.end() , sortByOccurance);
}


/*********************************** Find MIN & MAX of attribute ********************************/

void findMinmax(vector<vector <double> > &dataTable, int attributeIndex, double &Low, double &Max)
{
    Max = -999999999.0;
    Low = 999999999.0;

    int row = dataTable.size() ;

    for(int i = 0; i < row ; i++)
    {
        if(dataTable[i][attributeIndex] > Max)
        {
            Max = dataTable[i][attributeIndex];
        }

        if(dataTable[i][attributeIndex] < Low)
        {
            Low = dataTable[i][attributeIndex];
        }
    }
}


/************************************ DECISION TREE Building ***********************************/

node* buildDecisionTree(vector<vector <double> > &dataTable, vector <distributionNode> &distributionList)
{


    node* tree = (node*) new node;

    int class_no  = distributionList.size();
    int otherClass = 0;
    for(int j = 1 ; j < class_no ;j++)
    {
        otherClass += distributionList[j].occurance;
    }
    if(dataTable.size() < PRUNING_THR || otherClass == 0)
    {
        tree->decidedClass = distributionList[0].class_no;
        tree->isLeaf = true;
        tree->distListForThisNode = distributionList;
        tree->leftChild = NULL;
        tree->rightChild = NULL;
        tree->threshold = 123456;
        tree->attribute = 123456;
    }
    else
    {

        int best_attribute;
        double best_threshold;


        CHOOSE_ATTRIBUTE(dataTable, best_attribute, best_threshold);


        tree->attribute = best_attribute;
        tree->threshold = best_threshold;
        tree->distListForThisNode = distributionList;

        vector<vector <double> > examples_left;
        vector<vector <double> > examples_right;

        int row = dataTable.size();

        for(int i = 0; i < row ; i++)
        {
            if(dataTable[i][best_attribute] < best_threshold)
                examples_left.push_back(dataTable[i]);
            else
                examples_right.push_back(dataTable[i]);
        }

        if(examples_left.size() != 0)
        {
            vector <distributionNode> distListLeft ;
            defaultClassFinder(examples_left , distListLeft);
            tree->leftChild = buildDecisionTree(examples_left, distListLeft);
        }
        if(examples_right.size() != 0)
        {
            vector <distributionNode> distListRight;
            defaultClassFinder(examples_right , distListRight);
            tree->rightChild = buildDecisionTree(examples_right,distListRight);
        }


        printf("tree %d , feature %d , thr = %6.2f , gain = %f\n", tree_ID , tree->attribute , tree->threshold ,MAX_GAIN);
	}


	return tree;

}

/************************************** CHOOSE ATTRIBUTE ********************/


void CHOOSE_ATTRIBUTE(vector<vector <double> > &dataTable, int& best_attribute, double &best_threshold )
{
    double max_gain = -1;
    MAX_GAIN = max_gain;
    best_attribute = -1;
    best_threshold = -1;
    int attributes = dataTable[0].size() - 1;

    if(mode_no == 1)
    {
        for(int i = 0; i < attributes; i++)
        {

            double L, M;
            findMinmax(dataTable, i, L, M);

            for (int K = 1; K <= 50; K++)
            {
                double threshold = 1.0 * L + K*(M-L)/51.0;

                double gain = INFORMATION_GAIN(dataTable, i, threshold);

                if (gain >= max_gain)
                {
                    MAX_GAIN = max_gain = gain;

                    best_attribute = i;
                    best_threshold = threshold;
                }
            }
        }
    }
    else
    {
        int i = rand()%attributes;

            double L, M;
            findMinmax(dataTable, i, L, M);

            for (int K = 1; K <= 50; K++)
            {
                double threshold = 1.0 * L + K*(M-L)/51.0;

                double gain = INFORMATION_GAIN(dataTable, i, threshold);

                if (gain >= max_gain)
                {
                    MAX_GAIN = max_gain = gain;

                    best_attribute = i;
                    best_threshold = threshold;
                }
            }

    }

}


/**************************************** INFORMATION GAIN *************************/

double INFORMATION_GAIN(vector<vector <double> >& dataTable, int attribute, double threshold)

{

        vector<vector <double> > examples_left;
        vector<vector <double> > examples_right;

        int row = dataTable.size();

        int left_counter = 0;
        int right_counter = 0;
        int total = 0;
        double information_gain = 0.0;
        for(int i = 0; i < row ; i++)
        {
            if(dataTable[i][attribute] < threshold)
            {
                 examples_left.push_back(dataTable[i]);
                 left_counter+= 1;
            }
            else
            {
                 examples_right.push_back(dataTable[i]);
                 right_counter+=1;
            }
        }
        total = left_counter + right_counter ;

        if(left_counter == 0)
        {
            vector <distributionNode> distListRight;
            defaultClassFinder(examples_right , distListRight);


            vector<distributionNode> distNow;
            defaultClassFinder(dataTable , distNow);

            int current_node_Class_no = distNow.size();
            int right_node_Class_no = distListRight.size();

            information_gain = 0.0;
            double entropy_current = 0.0;
            double entropy_right = 0.0;
            double weight_right = 0.0;


            for(int i = 0 ; i < current_node_Class_no ; i++)
            {
                entropy_current += (1.0 * (-distNow[i].occurance) / total * log2(1.0 * (distNow[i].occurance) / total));
            }
            for(int i = 0 ; i < right_node_Class_no ; i++)
            {
                entropy_right += (1.0 * (-distListRight[i].occurance) / right_counter * log2(1.0 * (distListRight[i].occurance) / right_counter));
            }

            information_gain = entropy_current - 1.0 * right_counter / total * entropy_right ;

        }
        else if(right_counter == 0)
        {
            vector <distributionNode> distListLeft ;
            defaultClassFinder(examples_left , distListLeft);

            vector<distributionNode> distNow;
            defaultClassFinder(dataTable , distNow);

            int current_node_Class_no = distNow.size();
            int left_node_Class_no = distListLeft.size();


            information_gain = 0.0;
            double entropy_current = 0.0;
            double entropy_left = 0.0;
            double entropy_right = 0.0;
            double weight_left = 0.0;
            double weight_right = 0.0;


            for(int i = 0 ; i < current_node_Class_no ; i++)
            {
                entropy_current += (1.0 * (-distNow[i].occurance) / total * log2(1.0 * (distNow[i].occurance) / total));
            }
            for(int i = 0 ; i < left_node_Class_no ; i++)
            {
                entropy_left += (1.0 * (-distListLeft[i].occurance) / left_counter * log2(1.0 * (distListLeft[i].occurance) / left_counter));
            }

            information_gain = entropy_current - 1.0 * left_counter / total * entropy_left;

        }

        else
        {
            vector <distributionNode> distListLeft ;
            defaultClassFinder(examples_left , distListLeft);


            vector <distributionNode> distListRight;
            defaultClassFinder(examples_right , distListRight);


            vector<distributionNode> distNow;
            defaultClassFinder(dataTable , distNow);

            int current_node_Class_no = distNow.size();
            int left_node_Class_no = distListLeft.size();
            int right_node_Class_no = distListRight.size();

            information_gain = 0.0;
            double entropy_current = 0.0;
            double entropy_left = 0.0;
            double entropy_right = 0.0;
            double weight_left = 0.0;
            double weight_right = 0.0;


            for(int i = 0 ; i < current_node_Class_no ; i++)
            {
                entropy_current += (1.0 * (-distNow[i].occurance) / total * log2(1.0 * (distNow[i].occurance) / total));

            }
            for(int i = 0 ; i < left_node_Class_no ; i++)
            {
                entropy_left += (1.0 * (-distListLeft[i].occurance) / left_counter * log2(1.0 * (distListLeft[i].occurance) / left_counter));
            }
            for(int i = 0 ; i < right_node_Class_no ; i++)
            {
                entropy_right += (1.0 * (-distListRight[i].occurance) / right_counter * log2(1.0 * (distListRight[i].occurance) / right_counter));
            }

            information_gain = entropy_current - 1.0 * left_counter / total * entropy_left - 1.0 * right_counter / total * entropy_right ;

        }

        return information_gain;
}


/********************************* TEST DATA ON DECISION TREE *********************/


double testData(vector <double> &singleLine, node* tree)
{
    double predicted_class ;

    while(tree->isLeaf != true)
    {
        double value = singleLine[tree->attribute];

		if(value < tree->threshold)
        {
            if(tree->leftChild != NULL)
            {
                tree = tree->leftChild;
            }
            else
            {

                return tree->distListForThisNode[0].class_no;
            }
        }

		else
        {
            if(tree->rightChild != NULL)
            {
                tree = tree->rightChild;
            }
            else
            {

                return tree->distListForThisNode[0].class_no;
            }
        }
    }
    predicted_class =  tree->decidedClass;

    return predicted_class;
}










































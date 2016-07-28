#include <cstdio>
#include <iostream>
#include <cstring>
#include <string>
#include <stack>
#include <queue>
#include <vector>
#include <fstream>
#include <map>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ios>
#include <set>
#include <utility>

using namespace std;

#define SIZE 55                           //This value indicates the maximum number of city , change if needed (Beware of stack size)
#define INF 99999999                      //Max distance (It assumes that no distance will be greater than this value

/*
    find_route function takes 3 parameter
    inputfilename = which contains the connections and distance between cities . Last line will be END OF INPUT
    origin_city = starting city
    destination_city = ending city

    This function calculates the shortest path between two cities
*/
void find_route(string inputfilename , string  origin_city , string destination_city);


/*
    Edge has two attribute , City index meaning which city is connected and distance
    It has a constructor named Edge () which takes two int and construct an Edge type node
    For the sake of priority queue it has an operator overloading function . It basically overload '<' sign so that the min value stays in the front of priority queue
*/
struct Edge
{
    int city_index;
    int city_distance;

    Edge(int a , int b)
    {
        city_index = a;
        city_distance = b;
    }

    bool operator < (const Edge &other) const
    {
        return (other.city_distance < city_distance);
    }
};


/*
    main function scan three data 1.Input File Name , 2. Origin City 3.Destination City and call the find_route function

    Input format :  <inputfilename> <origincity> <destinationcity>

    Example Input:

    input1.txt Bremen Frankfurt

    Warning : This program is case sensitive . So please input names according to your given inputfile . Otherwise it won't work
*/
int main(int argc, const char * argv[])
{
    string inputfilename;

    string origin_city  , destination_city;

    while(cin >> inputfilename >> origin_city >> destination_city)
    {
        find_route(inputfilename , origin_city , destination_city);
    }

    return 0;
}








void find_route(string inputfilename , string  origin_city , string destination_city)
{

    int distance;
    string city1 , city2 ;

    int no_of_city = 0;                      //Stores number of unique cities in the data set

    vector <Edge> adjacency_list[SIZE];

    //Dijakstra's Property
    int dist[SIZE];                          //This array is for storing the distance from origin in dijakstra's algorithm
    int father[SIZE];                        //This array stores from which node dist array is updated
    priority_queue<Edge> priorityQueue;

    map <string , int> indexing_of_city;      //It return a int index value against a city name
    map <int  , string > index_to_city;       //It returns a city string against a city index

    //opening file from user input string
    ifstream inputfile;
    inputfile.open(inputfilename.c_str(),ios::in | ios::binary);

    if(inputfile.is_open() != true)
    {
        cout << inputfilename << " not found" << endl;
        return;
    }

    while(inputfile >> city1)
    {
        inputfile >> city2;
        if(city1 == "END" && city2 == "OF")
        {
            inputfile >> city2;
            break;
        }

        inputfile >> distance;

        if(indexing_of_city[city1] == 0)        //Checking if the city1 is already indexed
        {
            no_of_city++;
            indexing_of_city[city1] = no_of_city;
            index_to_city[no_of_city] = city1;
        }
        if(indexing_of_city[city2] == 0)        //Checking if the city2 is already indexed
        {
            no_of_city++;
            indexing_of_city[city2] = no_of_city;
            index_to_city[no_of_city] = city2;
        }
        adjacency_list[indexing_of_city[city1]].push_back(Edge(indexing_of_city[city2] , distance));     //Inserting city2 to city1's adjacency list
        adjacency_list[indexing_of_city[city2]].push_back(Edge(indexing_of_city[city1] , distance));     //Inserting city1 to city2's adjacency list

    }


    if(indexing_of_city[origin_city] == 0)        //Checking if the origin_city is already indexed
    {
        no_of_city++;
        indexing_of_city[origin_city] = no_of_city;
        index_to_city[no_of_city] = origin_city;
    }
    if(indexing_of_city[destination_city] == 0)        //Checking if the destination_city is already indexed
    {
        no_of_city++;
        indexing_of_city[destination_city] = no_of_city;
        index_to_city[no_of_city] = destination_city;
    }

    int start = indexing_of_city[origin_city] ;
    int finish = indexing_of_city[destination_city];

    for(int i = 0 ; i < SIZE ; i++)
    {
        dist[i] = INF;                   //Initially every node has a distance of Infinity(A big value ) from the origin_city
        father[i] = i;                   //Initially all nodes are updated by own
    }

    dist[start] = 0;                     //Distance from origin city to origin city is Zero

    priorityQueue.push(Edge(start,0));    //Pushing the origin city or starting node to the priority queue

    while(priorityQueue.empty() == false)
    {
        Edge current_edge = priorityQueue.top();
        priorityQueue.pop();

        int elements = adjacency_list[current_edge.city_index].size();  //Number of adjacent nodes in the current node

        for(int i = 0 ; i < elements ; i++)
        {
            Edge adj = adjacency_list[current_edge.city_index][i];

            if(current_edge.city_distance + adj.city_distance < dist[adj.city_index])
            {
                dist[adj.city_index] = current_edge.city_distance + adj.city_distance;
                father[adj.city_index] = current_edge.city_index;
                priorityQueue.push(Edge(adj.city_index,  current_edge.city_distance + adj.city_distance));
            }
        }
    }

    if(dist[finish] == INF)                //if the values stays Infinity it means it has not been updated by running the Dijakstra from the origin city. So no path existed
    {
        printf("distance : infinity\n");
        printf("route:\n");
        printf("none\n");
    }
    else
    {
        printf("distance : %d km\n", dist[finish]);
        printf("route:\n");

        stack<int> route;
        stack<int> cummulative_distance;

        while(father[finish] != finish)
        {
            route.push(finish);
            cummulative_distance.push(dist[finish]);
            finish = father[finish];
        }

        int current_node = start;
        int current_distance = 0;
        int next_node;

        while(route.empty() != true)
        {
            cout << index_to_city[current_node] << " to " << index_to_city[route.top()] << ", " << cummulative_distance.top() - current_distance << " km"<< endl;
            current_node = route.top();
            current_distance = cummulative_distance.top();

            route.pop();
            cummulative_distance.pop();
        }
    }
}


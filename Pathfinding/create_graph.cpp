#include <cassert>
#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>
using namespace std;

#define INF ((int)(1 << (8 * sizeof(int) - 3)))


void generate_dot(string path, string dot){
    fstream myFile(path, ios_base::in);
    ofstream myDot(dot);
    int input;
    vector<int> data;
    int i = 0;
    while(myFile >> input){
        data.push_back(input);
    }
    myDot << "graph {\n \trankdir=LR;\n";
    while(i < data.size()){
        myDot << "\t" << data[i] << " -- " << data[i+1] << " [label=\"" << data[i+2] << "\"];\n";
        i += 3;
    }
    myDot << "}\n";
}

void create_graph(string dot, string graph){
    string command = "dot -Tpng " + dot + " -o " + graph;
    const char* cmd = command.c_str();
    system(cmd);
}

int getNumberOfNodes(vector<vector<int>> data){
    vector<int> nodes;
    for(int i = 0; i<data.size();i++){
        nodes.push_back(data[i].at(0));
    }
    int numberOfNodes;
    for(int i = 1; i<nodes.size();i++){
        if(nodes[i] > numberOfNodes){
            numberOfNodes = nodes[i];
        }
    }
    return numberOfNodes+1;
}

vector<vector<int>> convertToAdjacencyMatrix(string path){
    
    fstream myFile(path, ios_base::in);
    int input;
    vector<vector<int>> data;
    int i=0, j=0;
    
    while(myFile >> input){
        data[i].push_back(input);
        j++;
        if(j == 3){
            i++;
            j=0;
        }
    }
    
    vector<vector<int>> graph;
    int numberOfNodes = getNumberOfNodes(data);
    vector<int> row_zero;
    
    for(int b=0; b<numberOfNodes; b++){
        row_zero.push_back(0);
    }
    

    for(int b=0; b<numberOfNodes; b++){
        graph.push_back(row_zero);
    }
    
    for(int n=0; n<numberOfNodes; n++){
        int nodeFrom = data[n].at(0);
        int nodeTo = data[n].at(1);
        int weight = data[n].at(2);
        graph[nodeFrom].at(nodeTo) = weight;
        graph[nodeTo].at(nodeFrom) = weight;
    }

    return graph;
}

int dijkstra(vector<vector<int>> graph, int startNode, int endNode){
    vector<int> distance(graph[0].size());
    vector<bool> unvisited(graph[0].size());

    for(int i=0; i<distance.size();i++){
        distance[i] = INF;
        unvisited[i] = false;
    }

    distance[startNode] = 0;

    for(int i=0; i<distance.size();i++){
        int minDistance = INF;
        int minDistanceIndex;
        for(int j=0;j<distance.size();j++){
            if(distance[j]<=minDistance && unvisited[j] == false){
                minDistance = distance[j];
                minDistanceIndex = j;
            }
        }
        unvisited[minDistanceIndex] = true;
        for(int j=0;j<distance.size();j++){
            if(!unvisited[j] 
            && graph[minDistanceIndex][j] 
            && distance[minDistanceIndex]!=INF 
            && distance[minDistanceIndex]+graph[minDistanceIndex][j]<distance[j]){
                distance[j]=distance[minDistanceIndex]+graph[minDistanceIndex][j];
            }
        }
    }

    return distance[endNode];
}



int main(int argc, char* argv[]){
    string path = argv[1];
    //auto dot = "graph_dot.dot"; 
    //string graph = argv[2];
    //generate_dot(path, dot);
    //create_graph(dot, graph);
    auto graph_adjacency = convertToAdjacencyMatrix(path);
    vector<vector<int>> graph = {
        {0, 1, 2, 0, 0, 0},
        {1, 0, 0, 5, 1, 0},
        {2, 0, 0, 2, 3, 0},
        {0, 5, 2, 0, 2, 2},
        {0, 1, 3, 2, 0, 1},
        {0, 0, 0, 2, 1, 0}};
    auto distance = dijkstra(graph_adjacency, 0, 3);
    cout << distance;
    return 0;
}
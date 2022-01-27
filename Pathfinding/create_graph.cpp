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

int getNumberOfNodes(vector<int> data){
    vector<int> nodes;
    for(int i = 0; i<data.size(); i+=3){
        nodes.push_back(data.at(i+1));
    }
    int numberOfNodes = nodes[0];
    for(int i = 1; i<nodes.size();i++){
        if(nodes.at(i) > numberOfNodes){
            numberOfNodes = nodes.at(i);
        }
    }
    return numberOfNodes+1;
}

vector<vector<int>> convertToAdjacencyMatrix(string path){
    
    fstream myFile(path, ios_base::in);
    int inputd;
    vector<int> d;
    while(myFile >> inputd){
        d.push_back(inputd);
    }
    int numberOfNodes = getNumberOfNodes(d);
    
    vector<vector<int>> graph;
    vector<int> row_zero;
    
    for(int b=0; b<numberOfNodes; b++){
        row_zero.push_back(0);
    }
    

    for(int b=0; b<numberOfNodes; b++){
        graph.push_back(row_zero);
    }
    
    for(int n=0; n<d.size();n+=3){
        int nodeFrom = d.at(n);
        int nodeTo = d.at(n+1);
        int weight = d.at(n+2);
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
    auto path = argv[1];
    //auto dot = "graph_dot.dot"; 
    //string graph = argv[2];
    //generate_dot(path, dot);
    //create_graph(dot, graph);
    auto graph_adjacency = convertToAdjacencyMatrix(path);
    int startNode = stoi(argv[2]);
    int endNode = stoi(argv[3]);
    auto distance = dijkstra(graph_adjacency, startNode, endNode);
    cout << distance;
    return 0;
}
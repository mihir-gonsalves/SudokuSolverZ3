#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <fstream>
using namespace std;

#include "graph.h"

int main(int argc, const char * argv[]) {
    cout << "Filename of graph to load: ";

    // TODO: next five code snippets
    // Snippet 1: read filename and open file
    string filename;
    cin >> filename;
    ifstream file(filename);

    // Snippet 2: get number graph size from file
    int numVertices, numEdges;
    file >> numVertices >> numEdges;
    cout << "Vertices: " << numVertices << " Edges: " << numEdges << endl;
    
    // Snippet 3: create graph
    Graph* graph = new Graph();

    // Snippet 4: read edges
    int l1, l2;
    float weight;
    for (int i = 0; i < numEdges; i++) {
        file >> l1 >> l2 >> weight;
        graph->insertVertex(l1);
        graph->insertVertex(l2);
        graph->insertEdge(l1, l2, weight);
    }

    // Snippet 5: read start and end of path
    int startVertex, endVertex;
    file >> startVertex >> endVertex;
    cout << "Start vertex: " << startVertex << " End vertex: " << endVertex << endl;
    
    cout << "Shortest path from " << startVertex << " to " << endVertex << ":" << endl;

    vector<Vertex*> path;

    // TODO: call shortest path on your graph for the sstart and end verices and save result to path
    path = graph->shortestPath(startVertex, endVertex);
    
    for (auto i : path) { // this is a for-each loop
        cout << i->label << " ";
    }
    // cout endline at very end
    cout << endl;
    return 0;
}

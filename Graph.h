#ifndef GRAPH_H
#define GRAPH_H

#include "LocatorHeap.h"

#include <vector>
using std::vector;


class Edge;

class Vertex
{
public:
    int label;
    vector<Edge*> edges;

    // helpers stored inside a vertex to help with path finding
    // you can use different auxilliary structures if desired
    bool visited;
    float distanceTo;
    vector<Vertex*> pathTo;
    Heap<Vertex*>::Locator locator;
    bool operator < ( const Vertex &v ) const 
    { 
        // TODO for locator heap implementation
        return this->distanceTo < v.distanceTo;
    }

    Vertex ( int l ) : label (l) { }
};

class Edge
{
public:
    Vertex *v1, *v2;
    float weight;

    Edge ( Vertex *nv1, Vertex *nv2, float newWeight ) : v1 ( nv1 ), v2 ( nv2 ), weight ( newWeight ) { }
};


class Graph
{
protected:
    vector<Vertex*> v;
    vector<Edge*> e;

public:
    Graph () {}

    ~Graph ()
    {
        // TODO: destructor
        for (auto vertex : v) {
            delete vertex;
        }
        for (auto edge : e) {
            delete edge;
        }
    }

    // TODO: complete rule of 3
    Graph (const Graph& other) { // copy constructor
        for (Vertex* vertex : other.v) {
            insertVertex(vertex->label);
        }

        for (Edge* edge : other.e) {
            insertEdge(edge->v1->label, edge->v2->label, edge->weight);
        }
    }

    Graph& operator=(const Graph& other) { // copy assignment constructor
        if (this != &other) {
            // essentially the destructor, but I have no idea how to call the destructor within the class
            for (Vertex* vertex : v) {
                delete vertex;
            }
            for (Edge* edge : e) {
                delete edge;
            }
            // clear vectors jic
            v.clear();
            e.clear();

            // Copy vertices
            for (Vertex* vertex : other.v) {
                insertVertex(vertex->label);
            }

            // Copy edges
            for (Edge* edge : other.e) {
                insertEdge(edge->v1->label, edge->v2->label, edge->weight);
            }
        }
        return *this;
    }

    void insertVertex ( int label )
    {
        // TODO: allocate a new vertex with the given label and save it in the graph lost of vertices
        for (Vertex* vertex : v){
            if (vertex->label == label)
                return;
        }
        Vertex *vertex = new Vertex(label);
        this->v.push_back(vertex);
    }

    void insertEdge ( int l1, int l2, float weight )
    {
        // TODO: allocate the new edge and add it to both the graph list of edge pointers and the edge list for the originating vertex.
        Vertex *v1 = nullptr;
        Vertex *v2 = nullptr;

        for (Vertex* vertex : v) {
            if (vertex->label == l1) {
                v1 = vertex;
            }
            if (vertex->label == l2) {
                v2 = vertex;
            }
        }

        if (v1 == nullptr || v2 == nullptr) {
            return;
        }
        
        Edge *edge = new Edge(v1, v2, weight);

        v1->edges.push_back(edge);
        v2->edges.push_back(edge);
        this->e.push_back(edge);
    }

    vector<Vertex*> shortestPath ( int start, int end )
    {
        // TODO: Dijkstra's algorithm -- return a list of vertices to traverse along hte optimal path to the end node
        // *** USED FOR EACH LOOPS -- THE SYNTAX IS SO MUCH EASIER TO UNDERSTAND ESPECIALLY WITH THE NEXTEDGES LOOP ***
        Vertex *currentNode = nullptr;
        Vertex *endNode = nullptr;

        for (Vertex* vertex : v) {
            vertex->visited = false;
            vertex->pathTo.clear();
            vertex->distanceTo = 999999999.0; // std::numeric_limits<float>::infinity() does not work for some reason, replaced with a large number
            if (vertex->label == start) {
                currentNode = vertex;
            }
            if (vertex->label == end) {
                endNode = vertex;
            }
        }

        // if the runtime_error is highlighted with a redbar in VSCode, restart the entire application and it should go away
        if (currentNode == nullptr) {
            throw std::runtime_error("Start node not found in graph");
        }
        if (endNode == nullptr) {
            throw std::runtime_error("End node not found in graph");
        }

        currentNode->distanceTo = 0.0;
        currentNode->pathTo.push_back(currentNode);

        Heap<Vertex*> queue = Heap<Vertex*>();
        for (Vertex* vertex : v) {
            vertex->locator = queue.insertElement(vertex);
        }

        while (!(endNode->visited) && !(queue.isEmpty())){
            currentNode = queue.removeMin();
            currentNode->visited = true;
            
            vector<Edge*> nextEdges;
            for (Edge* edge : e) {
                if (edge->v1 == currentNode || edge->v2 == currentNode) {
                    nextEdges.push_back(edge);
                }
            }
            
            for (Edge* edge : nextEdges) {
                Vertex *nextNode = nullptr;
                if (edge->v1 == currentNode) {
                    nextNode = edge->v2;
                } 
                else {
                    nextNode = edge->v1;
                }
                
                if (!(nextNode->visited)) {
                    float candidateDistance = currentNode->distanceTo + edge->weight;
                    
                    if (candidateDistance < nextNode->distanceTo) {
                        nextNode->distanceTo = candidateDistance;
                        nextNode->pathTo = currentNode->pathTo;
                        nextNode->pathTo.push_back(nextNode);
                        queue.update(nextNode->locator);
                    }
                }
            }
        }
        return endNode->pathTo;
    }
};

#endif

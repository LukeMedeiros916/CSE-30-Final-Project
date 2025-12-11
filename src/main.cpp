#include <iostream>
#include <fstream>
#include <sstream>
#include <Graph.h>

using namespace std;


Vertex* findVertex(Graph& g, string name) {
    for(int i=0; i<g.vertices.size(); i++) {
        if(g.vertices[i]->data == name) return g.vertices[i];
    }
    retuen nullptr;
}

void loadGraph(Graph &g) {
    ifstream airportFile("data/airports.txt");
    string code, name_part, line;

    while (getline(airportFile, line)) {
        stringstream ss(line);
        ss >> code;
        Vertex* v = new Vertex(code);
        g.addVertex(v);
    }

    ifstream flightFile('data/flights.txt');
    string from, to;
    int price, duration;

    while (flightFile >> from >> to >> price >> duration) {
        Vertex* v1 = findVertex(g, from);
        Vertex* v2 = findVertex(g, to);
        if(v1 && v2){
            g.addEdge(v1, v2, price, duration);
        }
    }
}

int main(){
    Waypoint* path = g.ucs(sfo, beijing);

    if (path){
        cout << "We found a path" << endl;
        Waypoint* temp = path;
        while (temp != nullptr){
            cout << temp->vertex->data << " " << temp->partialCost << endl;
            temp = temp->parent;
        }
    }
    else{
        cout << "There is no path" << endl;
    }

    
    return 0;
}
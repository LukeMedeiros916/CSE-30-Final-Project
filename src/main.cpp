#include <iostream>
#include <fstream>
#include <sstream>
#include <Graph.h>

using namespace std;


Vertex* findVertex(Graph& g, string name) {
    for(int i=0; i<g.vertices.size(); i++) {
        if(g.vertices[i]->data == name) return g.vertices[i];
    }
    return nullptr;
}

void loadGraph(Graph &g) {
    ifstream airportFile("data/airports.txt");
    string code, name_part, line;

    if (!airportFile.is_open()) {
        cout << "Error: Could not open data/airports.txt" << endl;
        return;
    }

    while (getline(airportFile, line)) {
        stringstream ss(line);
        ss >> code;
        Vertex* v = new Vertex(code);
        g.addVertex(v);
    }
airportFile.close();

    ifstream flightFile("data/flights.txt");
    string from, to;
    int price, duration;

    if (!flightFile.is_open()) {
        cout << "Error: Could not open data/flights.txt" << endl;
        return;
    }

    while (flightFile >> from >> to >> price >> duration) {
        Vertex* v1 = findVertex(g, from);
        Vertex* v2 = findVertex(g, to);
        if(v1 && v2){
            g.addEdge(v1, v2, price, duration);
        }
    }
flightFile.close();
}

int main(){
    Graph g;
    loadGraph(g);

    Vertex* start = findVertex(g, "SFO");
    Vertex* end = findVertex(g, "PEK");

    if(!start || !end) {
        cout << "Could not find SFO or PEK in the graph." << endl;
        return 0;
    }

    Waypoint* path = g.ucs(start, end, COST_PRICE);

    if (path){
        cout << "We found a path" << endl;

        
        ArrayList<Waypoint*> route;
        Waypoint* temp = path;
        while (temp != nullptr){
            route.append(temp);
            temp = temp->parent;
        }

        for(int i = route.size() - 1; i >= 0; i--) {
            cout << route[i]->vertex->data;
            if (i > 0) cout << " -> ";
        }
        cout << endl;
        cout << "Total Cost: " << path->partialCost << endl;
    }
    else{
        cout << "There is no path" << endl;
    }

    
    return 0;
}
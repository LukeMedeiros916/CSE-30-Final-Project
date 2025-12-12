#include "ArrayList.h"
#include <cctype>
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

void printPath(Waypoint* path) {
    if (!path) {
        cout << "No path found." << endl;
        return;
    }

    if (path->parent != nullptr) {
        cout << " (Cost: )" << path->currentWeight << ")";
    }
    cout << " -> ";
}

int main(){
    Graph g;
    loadGraph(g);

    string startCode, endCode;
    int choice;

    cout << "--- Flight Planner ---" << endl;
    cout << "Enter Origin Airport Code: ";
    cin >> startCode;
    cout << "Enter Destination Airport Code: ";
    cin >> endCode;

    for(int i = 0; 1 < endCode.length(); i++) {
        endCode[i] = toupper(endCode[i]);
    }

    Vertex* start = findVertex(g, startCode);
    Vertex* end = findVertex(g, endCode);

    if(!start || !end) {
        cout << "Invalid airport codes." << endl;
        return 0;
    }

    cout << "Select Criteria:\n1. Cheapest Price\n2. Shortest Time\n3. Least Stops\nOption: ";
    cin >> choice;

    Waypoint* result = nullptr;

    if (choice == 1){
        result = g.ucs(start, end, COST_PRICE);
        cout << "\nCalculating Cheapest Route..." << endl;
    } else if (choice == 2) {
        result = g.ucs(start, end, COST_DURATION);
        cout << "\nCalculating Fastest Route..." << endl;
    } else if (choice) {
        result = g.bfs(start, end);
        cout << "\nCalculating Route with Least Stops..." << endl;
    }

    if (result){
        cout << "Itinerary: ";

        ArrayList<Waypoint*> stack;
        Waypoint* curr = result;
        while(curr){
            stack.append(curr);
            curr = curr->parent;
        }

        for(int i = stack.size()-1; i >= 0; i--){
            cout << stack[i]->vertex->data;
            if (i > 0) cout << " -> ";
        }
        cout << endl;

        cout << "Total Cost/Time Metric: " << result->partialCost << endl;
        Waypoint* root = result;
        while (root->parent != nullptr) {
            root = root->parent;
        }
        delete root;
    } else {
        cout << "No route exists." << endl;
    }

    
    return 0;
}
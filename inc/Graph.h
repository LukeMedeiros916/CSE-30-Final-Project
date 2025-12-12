#ifndef GRAPH_H
#define GRAPH_H

#include "LinkedList.h"
#include <ArrayList.h>
#include <HashTable.h>
#include <Queue.h>
#include <Stack.h>
#include <cstddef>
#include <ostream>
#include <string>

enum SearchCriteria { COST_PRICE, COST_DURATION };

struct Edge;

struct Vertex {
    std::string data;
    ArrayList<Edge *> edgeList;

    Vertex(std::string data) { this->data = data; }

    ~Vertex() {
        for(int i = 0; i < edgeList.size(); i++){
            delete edgeList[i];
        }
    }
};

inline std::ostream &operator<<(std::ostream &os, Vertex *v) {
    os << v->data;

    return os;
}

struct Edge {
    Vertex *from;
    Vertex *to;
    int price;
    int duration;

    Edge(Vertex *from, Vertex *to, int price, int duration) {
        this->from = from;
        this->to = to;
        this->price = price;
        this->duration = duration;
    }
};

inline std::ostream &operator<<(std::ostream &os, Edge *e) {
    os << "(" << e->from->data << "-> " << e->to->data << ") $ " << e->price << ", " << e->duration << "m";

    return os;
}

struct Waypoint {
    Waypoint *parent;
    Vertex *vertex;
    ArrayList<Waypoint *> children;
    int partialCost;
    int currentWeight;

    Waypoint(Vertex *v) {
        parent = nullptr;
        vertex = v;
        currentWeight = 0;
        partialCost = 0;
    }

    ~Waypoint(){
        for(int i = 0; i < children.size(); i++)
        delete children[i];
    }

    void expand(SearchCriteria criteria) {
        for (int i = 0; i < vertex->edgeList.size(); i++) {
            Waypoint *temp = new Waypoint(vertex->edgeList[i]->to);
            temp->parent = this;

            int weightVal = 0;
            if (criteria == COST_PRICE) {
                weightVal = vertex->edgeList[i]->price;
            } else {
                weightVal = vertex->edgeList[i]->duration;
            }




            temp->currentWeight = weightVal;
            temp->partialCost = partialCost + weightVal;
            children.append(temp);
        }
    }
};

inline std::ostream &operator<<(std::ostream &os, Waypoint *wp) {
    std::string p = "null";
    if (wp->parent != nullptr) {
        p = wp->parent->vertex->data;
    }

    os << p << " -> " << wp->vertex->data;

    return os;
}

struct Graph {
    ArrayList<Vertex *> vertices;

    void addVertex(Vertex *v) { vertices.append(v); }

    void addEdge(Vertex *x, Vertex *y, int price, int duration) {
        x->edgeList.append(new Edge(x, y, price, duration));
        y->edgeList.append(new Edge(y, x, price, duration));
    }

    ~Graph() {
        for(int i = 0; i < vertices.size(); i++){
            delete vertices[i];
        }
    }


    Waypoint *bfs(Vertex *start, Vertex *destination) {
        std::cout << "Running Breadth-First Search" << std::endl;
        Queue<Waypoint *> frontier;
        HashTable<std::string> seen;

        Waypoint *first = new Waypoint(start);

        frontier.enqueue(first);
        seen.insert(first->vertex->data);

        Waypoint *result = nullptr;

        while (!frontier.isEmpty()) {
            result = frontier.dequeue();

            if (result->vertex == destination) {
                return result;
            }

            result->expand(COST_PRICE); // Default criteria for BFS
            std::cout << std::endl << "Expanding " << result->vertex->data << std::endl;

            for (int i = 0; i < result->children.size(); i++) {
                if (!seen.search(result->children[i]->vertex->data)) {
                    std::cout << "Adding " << result->children[i]->vertex->data << std::endl;
                    frontier.enqueue(result->children[i]);
                    seen.insert(result->children[i]->vertex->data);
                }
            }
        }
        delete first;

        return nullptr;
    }

    Waypoint *dfs(Vertex *start, Vertex *destination) {
        std::cout << "Running Depth-First Search" << std::endl;

        Stack<Waypoint *> frontier;
        HashTable<std::string> seen;

        Waypoint *first = new Waypoint(start);

        frontier.push(first);
        seen.insert(first->vertex->data);

        Waypoint *result = nullptr;

        while (!frontier.isEmpty()) {
            result = frontier.pop();

            if (result->vertex == destination) {
                return result;
            }

            result->expand(COST_PRICE);

            std::cout << std::endl << "Expanding " << result->vertex->data << std::endl;

            for (int i = 0; i < result->children.size(); i++) {
                if (!seen.search(result->children[i]->vertex->data)) {
                    std::cout << "Adding " << result->children[i]->vertex->data << std::endl;
                    frontier.push(result->children[i]);
                    seen.insert(result->children[i]->vertex->data);
                }
            }
        }
        delete first;
        return nullptr;
    }

    Waypoint *ucs(Vertex *start, Vertex *destination, SearchCriteria criteria) {
std::cout << "Running Uniform Cost Search" << std::endl;

        ArrayList<Waypoint *> frontier;
        HashTable<std::string> seen;

        Waypoint *first = new Waypoint(start);

        frontier.append(first);
        seen.insert(first->vertex->data);

        Waypoint *result = nullptr;

        while (frontier.size() != 0) {
            result = frontier.removeLast();

            if (result->vertex == destination) {
                return result;
            }

            result->expand(criteria);

            std::cout << "Expanding " << result->vertex->data << std::endl;

            for (int i = 0; i < result->children.size(); i++) {
                if (!seen.search(result->children[i]->vertex->data)) {
                    std::cout << "Adding " << result->children[i]->vertex->data << std::endl;
                    frontier.append(result->children[i]);


                    int j = frontier.size() - 1;
                    while (j > 0 && frontier.data[j]->partialCost > frontier.data[j - 1]->partialCost) {
                        Waypoint *temp = frontier.data[j];
                        frontier.data[j] = frontier.data[j - 1];
                        frontier.data[j - 1] = temp;
                        j--;
                    }

                    seen.insert(result->children[i]->vertex->data);
                } else {

                    Waypoint *worsePath = nullptr;

                    for (int k = 0; k < frontier.size(); k++) {
                        if (frontier[k]->vertex->data == result->children[i]->vertex->data) {
                            if (frontier[k]->partialCost > result->children[i]->partialCost) {
                                worsePath = frontier[k];
                                break;
                            }
                        }
                    }

                    if (worsePath) {
                        std::cout << "Found better path to " << result->children[i]->vertex->data 
                                  << ". Was " << worsePath->partialCost << ", now "
                            << result->children[i]->partialCost << std::endl;


                        for (int k = 0; k < frontier.size(); k++) {
                            if (frontier[k]->vertex->data == result->children[i]->vertex->data) {
                                delete frontier[k];
                                frontier[k] = result->children[i];
                                break;
                            }
                        }

                        for (int a = 1; a < frontier.size(); a++) {
                            int b = a;
                            while (b > 0 && frontier[b]->partialCost > frontier[b - 1]->partialCost) {
                                Waypoint *x = frontier[b];
                                frontier[b] = frontier[b - 1];
                                frontier[b - 1] = x;
                                b--;
                            }
                        }
                    }
                }
            }
}       delete first;
        return nullptr;
    }
};

inline std::ostream &operator<<(std::ostream &os, const Graph &g) {
    for (int i = 0; i < g.vertices.size(); i++) {
        os << g.vertices[i]->edgeList << std::endl;
    }

    return os;
};

#endif
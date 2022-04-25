
//libraries includes
#include <iostream>
#include <string>
#include <ctime>
#include <vector>
#include <limits>
#include <tuple>
#include <map>

//using a number of calls from the std library
using namespace std;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//GRAPH CLASS
class graph {
public:
    graph(int s): size(s){}   //constructor requires input size s 
    void rand_graph(int dens = 50, float max_wt = 0, float min_wt = 0);
    void print();
    int size_of();
    friend class prim;

private:
    graph();    //default constructor
    int size;

    //use map to represent adjacency list - this can allow us to use chars or names to store the list
    map< int, vector<pair<int, float>> > adj_list;
    const float NAE = -1.0;     //const parameter for Not An Edge

};

void graph::rand_graph(int dens, float max_wt, float min_wt) {
    float wt, rand_mult;

    for (int m = 0; m < size; m++) {
        for (int n = m; n < size; n++) {
            if (n != m) {
                if (rand() % 100 < dens) {
                    rand_mult = static_cast<float> (rand() % 1000) / 1000.0;
                    wt = min_wt + rand_mult*(max_wt - min_wt);
                    adj_list[m].push_back({ n, wt });
                    adj_list[n].push_back({ m, wt });
                }
            }
        }
    }
}

int graph::size_of() {
    return size;
}

void graph :: print() {
    cout << "Adjacency list for Graph" << endl;
    for (auto it = adj_list.begin(); it != adj_list.end(); ++it) {
        const auto& children = it->second;
        cout << it->first << "\t|\t";
        for (int i = 0; i < children.size(); ++i) {
            cout << children[i].first << "(WT: " << children[i].second <<  ") ";
        }
        cout << "\n\t|" << endl;
    }
}




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Priority Queue for shortest unvisited points (open set)
// This queue is important for shortest path algorithm outlined in https://www.youtube.com/watch?v=pVfj6mxhdMw&t=476s&ab_channel=ComputerScience
// This holds node values and associated distances from a start node - also holds the parent node - where we would come from to get to the node
// Queue is ordered based on distance values associated with each node


class pq {
public:
    void insert(int node, int parent = -1, float dist = 9e+010);
    void repinsert(int node,  int parent = -1, float dist = 9e+010 );
    float contains(int node);
    void remove(int node);
    void print();
    void clear();
    int size_of();
    tuple<int, int, float> pop();

private:
    //queue is made up of these structs
    struct node_dist {
        int node;
        int parent;
        float dist;
    };
    vector<node_dist> minheap;
    const float NInc = -1.0F;  //Const variable for showing when a node is not included in PQ
};

//Remove top item in the queue adn return it as a tuple
tuple<int, int, float> pq::pop() {
    node_dist temp = minheap[minheap.size() - 1];
    minheap.pop_back();
    return { temp.node, temp.parent , temp.dist };
}

//insert an item into the priority queue in correct place regarding distance values
void pq::insert(int node,  int parent, float dist) {
    minheap.push_back({ 0, 0,-1.0F });

    for (int i = 0; i < minheap.size(); i++) {
        if (dist > minheap[i].dist) {
            minheap.insert(minheap.begin() + i, { node, parent , dist });
            minheap.pop_back();
            return;
        }
    }
}

//remove a certain item from queue based on node value
void pq::remove(int node) {
    for (int i = 0; i < minheap.size(); i++) {
        if (node == minheap[i].node)
            minheap.erase(minheap.begin() + i);
    }
}

//Returns negative value if node is not in PQ, otherwise returns its associated distance
float pq::contains(int node) {
    for (auto& p : minheap) {
        if (p.node == node)
            return p.dist;
    }
    return NInc;
}

//Replace-insert function
//checks to see if a node is already included and has higher dist value
//if true, delete from queue
//then inserts node if required 
void pq::repinsert(int node, int parent, float dist) {

    float exists = contains(node);
    if (exists > dist) {
        remove(node);
        exists = NInc;
    }

    if (exists < 0)
        insert(node, parent, dist);
}

//return size of the queue
int pq::size_of() {
    return minheap.size();
}

//clear the queue - used for resetting dij algo
void pq::clear() {
    minheap.clear();
}

//Print the priority queue
void pq::print() {
    cout << "Priority Queue" << endl;
    for (auto& p : minheap) {
        cout << "Node:\t" << p.node << "\tDistance: " << p.dist << "\tparent: " << p.parent << endl;
    }
}




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Prims Algorithm

class prim {
public:
    prim(graph g);
    void rst();
    bool run();
    void print_mst();

private:
    prim();
    graph g;

    vector<pair<int, float>> parent;
    vector<bool> visited;
    pq open;
};

prim::prim(graph g) :g(g) {
    rst();
}

bool prim::run() {

    rst();

    //add start value to open set
    open.repinsert(0, -1, 0.0F);
    float n_dist = 0.0F;
    tuple<int, int, float> temp;
    int new_child = 0;

    while (open.size_of() > 0) {
        open.print();
        temp = open.pop();
        new_child = get<0>(temp);
        parent[new_child] = { get<1>(temp) , get<2>(temp) };
        visited[new_child] = true;

        for (auto& n : g.adj_list[new_child]) {
            if (visited[n.first] == false)
                //cout << "Adding: " << n.first << "   " << n.second << endl;
                open.repinsert(n.first, new_child, n.second);
        }
    }

    for (const bool& check : visited)
        if (check == false) {
            cout << "Failed to find path to all nodes" << endl;
            return false;
        }

    return true;

}

void prim::print_mst() {
    float wt = 0;
    cout << "MST Overview \n\n";
    for (int n = 0; n < parent.size(); n++) {
        cout << "parent of " << n << " is " << parent[n].first << " - Edge Weight is " << parent[n].second << endl;
        wt += parent[n].second;
    }

    cout << endl << "Minimum spanning tree weight: " << wt << endl;
}


void prim::rst() {
    parent.resize(g.size_of());
    for (auto& n : parent) {
        n = { -1, 0.0 };
    }

    visited.resize(0);
    visited.resize(g.size_of(), false);

    open.clear();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Main function


int main(void) {

    srand((unsigned)time(0));

    graph g1(8);
    g1.rand_graph(50, 0, 100);
    g1.print();

    prim p1(g1);
    
    p1.run();
    p1.print_mst();

}
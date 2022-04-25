/*

DM 18Apr2022

Dijkstra’s Shortest path algorithm

The following code shoes the use of Dijkstra’s shortest path algorithm. In implementing this function 
there were three key classes that were to be created: 

The first of these is the graph class. A graph is a collection of nodes in which any node can be 
connected to any other node via an edge. Every edge has a certain weight or cost associated with it. 
This may represent distance if we are thinking in terms of navigation. An adjacency matrix has been 
used to represent the graph. In this two-dimensional matrix, we can show the edge weight between any 
two nodes of the graph. If no edge exits a weight of negative 1 is used.

This code also required implementing a priority queue. With this we create a stack of inputs which is
ordered using a certain variable – in this case it is the distance associated with each node. We can 
use this to keep track of the nodes which have shortest distances from a start point on the graph. This
is critical for use in the Dijkstra’s algorithm.

Finally, Dijkstra’s algorithm class has been made. In this we run the algorithm and save our results 
into a vector table. The algorithm functions using a visited and unvisited list of nodes. The algorithm 
looks at the closest unvisited node to the start (at first this will be the start node), and then checks
what further nodes can be moved to from here and calculates each their distance to the start node. The 
unvisited list is then updated if required (if a node is not already included, or if calculated distance
is lower than known distance). We then add the current node to visited list and move to the next closest
unvisited node the starting point. This continues until all nodes that we can visit are visited.

*/

//libraries includes
#include <iostream>
#include <string>
#include <ctime>
#include <vector>
#include <limits>
#include <tuple>

//using a number of calls from the std library
using std::vector;
using std::pair;
using std::cout;
using std::endl;
using std::tuple;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//GRAPH CLASS
class graph {
public:
    graph(int s) ;   //constructor requires input size s 
    void rand_graph(int dens = 50, float max_wt = 0, float min_wt = 0);
    void print(std::string title = "Graph Matrix");
    int size_of();
    int edges();
    bool adjacent(int m, int n);
    void neighbors(int m);
    void set(int m, int n, float wt = 0);
    void set_rand(int m, int n, float max_wt = 0, float min_wt = 0);
    float get_edge(int m, int n);

private:
    
    graph();    //default constructor
    int size;
    vector<vector<float>> matrix;
    const float NAE = -1.0;     //const parameter for Not An Edge
    
};

//Constructor - generates graph adjacency matrix with no edges - all edge values are -1
graph::graph(int s) : size(s) {
    matrix.resize(size);

    for (int m = 0; m < matrix.size(); m++)
        matrix[m].resize(size, NAE);            //resize vector array to size of graph and fill with no edges
}

//Generate random edges for graph
void graph::rand_graph(int dens, float max_wt, float min_wt) {
    float wt, rand_mult;
    
    for (int m = 0; m < matrix.size(); m++) {

        for (int n = m; n < matrix[m].size(); n++){

            if (m == n)
                matrix[m][n] = NAE;    //Ensure no loops - no node connected to itself
            else
                //density used to decide if edge is present or not
                if((rand() % 100) < dens){
                    //generate random value between 0-1
                    rand_mult = (rand() % 10000) / 10000.0;
                    //generate random value between min and max weight 
                    wt = (min_wt + rand_mult * (max_wt - min_wt));
                    matrix[m][n] = matrix[n][m] = wt;                   
                }                                                       
        }
    }

    return;
}

//print adjacency matrix for graph
void graph::print(std::string title) {
    
    cout << title <<"\n\t\t";
    for (int m = 0; m < matrix.size(); m++)
        cout << m << "\t";
    cout << "\n____";

    for (int m = 0; m < matrix.size(); m++)
        cout << "_________";
    cout << "\n\t|\n";
    
    for (int m = 0; m < matrix.size(); m++) {
        cout << m << "\t|\t";
        for (int n = 0; n < matrix[m].size(); n++) {
            cout << matrix[m][n] << '\t';
        }
        cout << "\n\t|\n";
    }
}

//return number of nodes in the graph
int graph::size_of() {
    return size;
}

//return number of edges in graph
int graph::edges() {
    int edges = 0;
    for (int m = 0; m < matrix.size(); m++) {
        for (int n = m; n < matrix[m].size(); n++) {
            if (matrix[m][n] >= 0.0)
                edges++;
        }
    }
    return edges;
}

//return true two nodes are connected
bool graph::adjacent(int m, int n) {
    if (m >= size || n >= size) {
        cout << "\nInvalid node(s) - node(s) do no exist\n";
        return false;
    }
    if (matrix[m][n] >= 0.0)
        return true;
    else
        return false;

}

//Print nodes that are connected to a cetain node n
void graph::neighbors(int m) {
    if (m >= size ) {
        cout << "\nInvalid node - node does no exist\n";
        return;
    }
    cout << "\nNeighbours of Node " << m << endl;
    for (int n = 0; n < matrix[m].size(); n++) {
        if (matrix[m][n] >= 0.0)
            cout << "Edge to node " << n << " with weight " << matrix[m][n] << endl;    
    }
    return;

}

//Set the weight of a certain edge in the graph
void graph::set(int m, int n, float wt) {
    if (m == n)
        matrix[m][n] = NAE;    //Ensure no loops - no node connected to itself
    else {
        matrix[m][n] = matrix[n][m] = wt;
    }
}

//Set a random weight to a certain edge in the graph
void graph::set_rand(int m, int n, float max_wt, float min_wt) {
    if (m == n)
        matrix[m][n] = NAE;    //Ensure no loops - no node connected to itself
    else {
        float wt = (min_wt + ((rand() % 10000) / 10000.0) * (max_wt - min_wt));
        matrix[m][n] = matrix[n][m] = wt;                   
    }
}

//return the weight of an edge in the graph - returns -1 if no edge - used later
float graph::get_edge(int m, int n) {
    return matrix[m][n];
}





/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Priority Queue for shortest unvisited points (open set)
// This queue is important for shortest path algorithm outlined in https://www.youtube.com/watch?v=pVfj6mxhdMw&t=476s&ab_channel=ComputerScience
// This holds node values and associated distances from a start node - also holds the previous - where we would come from to get to the node
// Queue is ordered based on distance values associated with each node


class pq {
public:
    void insert(int node, float dist = 9e+010, int prev = -1);
    void repinsert(int node, float dist = 9e+010, int prev = -1);
    float contains(int node);
    void remove(int node);
    void print();
    void clear();
    int size_of();
    tuple<int, float, int> pop();

private:
    //queue is made up of these structs
    struct node_dist {
        int node;
        float dist;
        int prev;
    };
    vector<node_dist> minheap;
    const float NInc = -1.0F;  //Const variable for showing when a node is not included in PQ
};

//Remove top item in the queue adn return it as a tuple
tuple<int, float, int> pq::pop() {
    node_dist temp = minheap[minheap.size() - 1];
    minheap.pop_back();
    return { temp.node, temp.dist, temp.prev };
}

//insert an item into the priority queue in correct place regarding distance values
void pq::insert(int node, float dist, int prev) {
    minheap.push_back( {0, -1.0F, 0} );

    for (int i = 0; i < minheap.size(); i++) {
        if (dist > minheap[i].dist) {
            minheap.insert(minheap.begin() + i, {node, dist, prev});
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
float pq::contains(int node){
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
void pq::repinsert(int node, float dist, int prev) {

    float exists = contains(node);
    if (exists > dist) {
        remove(node);
        exists = NInc;
    }

    if (exists < 0)
        insert(node, dist, prev);
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
        cout << "Node:\t" << p.node << "\tDistance: " << p.dist << "\tPrev: " << p.prev << endl;
    }
}




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//DIJKSTRA's ALGORITHM
//This is the use of dijkstra's algorithm outlined in https://www.youtube.com/watch?v=pVfj6mxhdMw&t=476s&ab_channel=ComputerScience

class dij_algo {
public:
    dij_algo(graph& gr, int n);
    void rst();
    void new_start(int n);
    bool run();
    void add_clsd(int n, float dist, int prev);
    vector<int> shortest(int n);
    float get_dist(int n);
    float avg_dist();
    void print_clsd();

private:
    struct visited {
        bool vis;
        float dist;
        int prev;
    };

    struct current {
        int node;
        float dist;
        int prev;
    } current;

    graph gr;
    int start;
    pq open;
    vector<visited> clsd;
};


//Run dijkstras algorithm
//fills the closed set - this shows if each node has been visited and what its ditance is from the start
//also holds previous node value - this indicates the previous node where we would have been if we take shortest path to get to a given node - used for decribing shortest path
bool dij_algo::run() {
    
    //add start value to open set
    open.repinsert(start, 0.0F, -1);
    float n_dist = 0.0F;

    //while the open set still holds values - we take the shortest known node form start point and see if we can move to any new nodes
    //if new nodes havent been visited
    while (open.size_of() > 0) {
        //we take the shortest known node form start point and add it to the closed set
        tuple<int, float, int> temp = open.pop();
        current = { std::get<0>(temp), std::get<1>(temp), std::get<2>(temp) };
        add_clsd(current.node, current.dist, current.prev);

        //then check what nodes we can move to from this
        for (int n = 0; n < gr.size_of(); n++) {
            //only add/update nodes that havent been visited
            if (clsd[n].vis == false) {
                n_dist = gr.get_edge(current.node, n);
                if (n_dist >= 0)
                    //for each node we can move to we update said node in the priority queue if required
                    open.repinsert(n, n_dist + current.dist, current.node);
            }
        }
    }

    //if all nodes havent been visited then we can say graph is not connected
    for (visited& check : clsd)
        if (check.vis == false) {
            cout << "Failed to find path to all nodes" << endl;
            return false;
        }

    return true;

}

//method to add node to closed set
void dij_algo::add_clsd(int n, float dist, int prev) {
    clsd[n] = { true, dist, prev };
}

//constructor - reset open and closed sets
dij_algo::dij_algo(graph& gr, int n) : gr(gr), start(n) {
    rst();
}

//reset method
void dij_algo::rst() {
    open.clear();
    clsd.clear();
    clsd.resize(gr.size_of(), {false, 9e+10, -1});
}

//method to change start point - also reset the algo
void dij_algo::new_start(int n) {
    start = n;
    rst();
}

//return a vector of integers indicates shortest path from start to a given node 
vector<int> dij_algo::shortest(int n) {
    int prev = 1;
    vector<int> ret;

    while (n > -1) {
        ret.insert(ret.begin(), n);
        n = clsd[n].prev;
    }
    return ret;
}

//get shortest distance from start to a given node
float dij_algo::get_dist(int n) {
    return clsd[n].dist;
}

//return the average path length from start to all other nodes - does not count unconnected nodes
float dij_algo::avg_dist() {
    float avg = 0;
    int div = 0;
    for (visited& n : clsd) {
        if (n.prev != -1) {
            avg += n.dist;
            div++;
        }
    }
    return avg /= div;
}

//Print the closed set - showing if each node has been visited, how far it is from start and whats its previous node is in the shortest path.
void dij_algo::print_clsd() {
    cout << "\nShortest path to each node from: " << start << endl << endl;
    cout << " node" << "\t|\tvis\tdistance\tprevious" << endl << endl;
    for (int n = 0; n < clsd.size(); n++)
        cout << "  " << n << "\t|\t" << clsd[n].vis << "\t" << clsd[n].dist << "        \t" << clsd[n].prev << endl;
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Main function


int main(void) {

    srand((unsigned)time(0));

    //Monte Carlo Simulation of dijkstra's algorith
    //first find average average path length for 100 graphs of size 50
    float avg20 = 0;
    for (int i = 0; i < 1000; i++) {
        graph graph1(50);
        graph1.rand_graph(20, 1.0, 10.0);
        dij_algo dij1(graph1, 0);
        dij1.run();
        avg20 += dij1.avg_dist();
    }
    avg20 /= 1000;


    float avg40 = 0;
    for (int i = 0; i < 1000; i++) {
        graph graph2(50);
        graph2.rand_graph(40, 1.0, 10.0);
        dij_algo dij2(graph2, 0);
        dij2.run();
        avg40 += dij2.avg_dist();
    }
    avg40 /= 1000;

    cout << "Graph 1\nNodes: 50\nMin Edge Weight: 1\nMax Edge Weight: 10\nEdge Density: 20\n--> Average path length: " << avg20 << endl << endl;

    cout << "Graph 1\nNodes: 50\nMin Edge Weight: 1\nMax Edge Weight: 10\nEdge Density: 40\n--> Average path length: " << avg40 << endl;
}



/*

Output printed to console during program run time:

        Graph 1
        Nodes: 50
        Min Edge Weight: 1
        Max Edge Weight: 10
        Edge Density: 20
        --> Average path length: 7.35012

        Graph 1
        Nodes: 50
        Min Edge Weight: 1
        Max Edge Weight: 10
        Edge Density: 40
        --> Average path length: 4.94552


*/
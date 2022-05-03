/*

DM 27Apr2022

- Program showing the use of kruskals algorithm to find Minimum Spanning Tree of graph
- Also shows method used to read values from a file to generate graph adjacnecy list
- NOTE: Ensure "graph.txt" is included in same directory as program file


03May2022
- Added use of priority queue structure instead of using a locally defined queue structure

*/


//libraries includes
#include <iostream>
#include <string>
#include <ctime>
#include <vector>
#include <list>
#include <fstream>
#include <sstream>
#include <queue>

//using a number of calls from the std library
using namespace std;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//GRAPH CLASS
class graph {
public:
    graph(int s) : size(s) {}   //constructor requires input size s 
    graph(ifstream& in_file);   //constructor requires input file to be read
    void rand_graph(int dens = 50, int max_wt = 0, int min_wt = 0);
    void print();

    //Declare krsukals as friend so it can access private functions and class variables
    friend class kruskals;

private:
    graph();    //default constructor
    int size;

    //Adjacency list
    vector<list<pair<int, int>> > adj_list;

};


//function for reading graph data from another file
//input ifstream pointer
graph::graph(ifstream& in_file) {
    //first check to ensure file pointer exists
    if (!in_file) {
        cout << "FAILED" << endl;
        return;
    }

    int i, j, cost;

    //We process one line of text file at a time
    string line;
    stringstream ss;
    getline(in_file, line);
    std::istringstream iss(line);

    //read first line of text file and store as size
    iss >> size;                        

    //resize list
    adj_list.resize(size);

    //read values and add to adjacency list
    while (getline(in_file, line)) {
        std::istringstream iss(line);
        iss >> i >> j >> cost;

        adj_list[i].push_back({ j, cost });
    }
    

}

//Function for generating random graph
void graph::rand_graph(int dens, int max_wt, int min_wt) {
    int wt;

    adj_list.resize(size);

    for (int m = 0; m < size; m++) {
        for (int n = m; n < size; n++) {
            if (n != m) {
                if (rand() % 100 < dens) {

                    wt = min_wt + (rand() % (max_wt - min_wt));
                    adj_list[m].push_back({ n, wt });
                    adj_list[n].push_back({ m, wt });

                }
            }
        }
    }
}


//Print adjacency list
void graph::print() {
    cout << "\nAdjacency list for Graph" << endl;
    int ind = 0;
    for (auto& par : adj_list) {

        cout << ind << "\t|\t";
        for (auto& child: par) {
            cout << child.first << "(WT: " << child.second << ") ";
        }
        cout << "\n\t|" << endl;
        ind++;
    }
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//kruskals Algorithm
class kruskals {
public:
    //public function declarations
    kruskals(graph gr);
    int run();
    void print_mst();


private:
    int size;
    int mst_wt;

    //Minimum spanning tree adjacency list
    vector<list<pair<int, int>> > mst;

    //Parent vector
    vector<int> parent;
    graph gr;

    //structure for storing edges 
    struct edge{
        int par;
        int child;
        int cost;
    };

    struct compare {
        bool operator()(edge const& e1, edge const& e2) {
            return e1.cost > e2.cost;
        }
    };

    //queue for use in algorithm
    priority_queue<edge, vector<edge>, compare> pq;
    
    //Private functions
    kruskals(){}
    void fillq();
    void rst();
    int par_find(int n);
    void new_parent(int node, int new_par);
};

//Kruskal constructor
kruskals::kruskals(graph gr) : gr(gr) {
    rst();
}

//Reset
void kruskals::rst() {
    pq = priority_queue<edge, vector<edge>, compare>();
    mst.clear();
    parent.clear();
    mst_wt = 0;
}

//Function to create queue of edges in graph
void kruskals::fillq() {
     
    //iterate through nodes in adjacency list
    for (int i = 0; i < gr.adj_list.size(); ++i) {
        
        //iterate through each of the connected nodes
        for (auto& child : gr.adj_list[i]) {
            edge temp = { i, child.first, child.second };
            pq.push(temp);

        }
    }
}

//finds top of a tree including node n
int kruskals::par_find(int n) {
    if (parent[n] == n)
        return n;
    
    int m = par_find(parent[n]);
    return m;
}

void kruskals::new_parent(int node, int new_par) {
    if (parent[node] != node) {
        new_parent(parent[node], node);
    }
    parent[node] = new_par;
}

//Kruskals Algorithm
int kruskals::run() {

    //reset algorithm and fill queue
    rst();
    fillq();

    int gr_size = gr.adj_list.size();
    mst.resize(gr_size);
    
    //Parent array needed for check loops
    parent.resize(gr_size);
    for (int i = 0; i < gr_size; i++)
        parent[i] = i;              //set all parent nodes to itself

    

    int n_edges = 0;

    //Add edges until we have 1 less edge than nodes in the graph
    while ((n_edges < gr_size - 1) && !pq.empty()) {
        auto check = pq.top();
        pq.pop();

        //Only add the edge if parent of each sub-tree is differenct
        //Ensure no loops added
        if (par_find(check.child) != par_find(check.par)) {
            //Add edge to adjacency list
            mst[check.par].push_back({ check.child, check.cost });
            mst[check.child].push_back({ check.par, check.cost });

            //if edge added than change parent on child node
            //this recursively changes parent of all required nodes in child tree as well
            new_parent(check.child, check.par);

            //Increment number of edges in the MST
            n_edges++;
            //Increment Weight of MST
            mst_wt += check.cost;
        }
    }
    //Count num of trees in MST - if more than one than graph is not connected
    int trees = 0;
    for (int n = 0; n < parent.size(); n++) {
        if (parent[n] == n) {
            trees++;
        }
    }

    if (trees > 1)
        return 0;

    return mst_wt;

}

//Function for printing MST 
void kruskals::print_mst() {
    cout << "\nMinimum Spanning Tree for Graph" << endl;
    int ind = 0;
    for (auto& par : mst) {

        cout << ind << "\t|\t";
        for (auto& child : par) {
            cout << child.first << "(WT: " << child.second << ") ";
        }
        cout << "\n\t|" << endl;
        ind++;
    }
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Main function

int main(void) {

    srand((unsigned)time(0));
    
    //generate file input stream for graph class
    ifstream data_file;
    data_file.open("graph.txt");

    //create graph
    graph g1(data_file);
    g1.print();

    //Create kruskals algorithm object
    kruskals k1(g1);

    //run algorithm and print results - prints adjacency list for the spanning tree
    int wt = k1.run();
    k1.print_mst();
    cout << "\nMST Weight: " << wt << endl;


}
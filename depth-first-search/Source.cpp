
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
    graph(int s);  //constructor requires input size s 
    void rand_graph(int dens = 50, float max_wt = 0, float min_wt = 0);
    void print();
    int size_of();
    friend class dfs;

private:
    graph();    //default constructor
    int size;
    struct edge {
        int n;
        float wt;
    };
    
    //use map to represent adjacency list - this can allow us to use chars or names to store the list
    vector<vector<edge>> adj_list;
    const float NAE = -1.0;     //const parameter for Not An Edge

};


graph::graph(int s) : size(s) {
    adj_list.resize(size);
}

void graph::rand_graph(int dens, float max_wt, float min_wt) {
    float wt, rand_mult;

    for (int m = 0; m < size; m++) {
        for (int n = m; n < size; n++) {
            if (n != m) {
                if (rand() % 100 < dens) {
                    rand_mult = static_cast<float> (rand() % 1000) / 1000.0;
                    wt = min_wt + rand_mult * (max_wt - min_wt);
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

void graph::print() {
    cout << "Adjacency list for Graph" << endl;
    for (int it = 0; it < adj_list.size(); ++it) {
        auto child = adj_list[it];
        cout << it << "\t|\t";
        for (int i = 0; i < child.size(); ++i) {
            cout << child[i].n << "(WT: " << child[i].wt << ") ";
        }
        cout << "\n\t|" << endl;
    }
}




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Depth First Search Algorithm

class dfs {
public:
    dfs(graph gr): gr(gr) {}
    bool run(int start = 0);
    void rec_run(int node);

private: 
    graph gr;
    vector<bool> visited;
};

bool dfs::run(int start) {
    
    visited.resize(gr.size, false);
    cout << "Starting at " << start << endl;
    rec_run(start);
    
    for (const bool& check : visited)
        if (check == false) {
            cout << "Failed to find path to all nodes" << endl;
            return false;
        }

    return true;

}

void dfs::rec_run(int vert) {
    
    visited[vert] = true;

    for (auto& node : gr.adj_list[vert]) {
        
        if (visited[node.n] == false) {
            cout << "Moving distance " << node.wt << " to get to Node " << node.n << endl;
            rec_run(node.n);
            cout << "Returning to Node " << vert << endl;
        }    
    }
    return;

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Main function


int main(void) {

    srand((unsigned)time(0));

    graph g1(8);
    g1.rand_graph(40, 0, 100);
    g1.print();

    dfs dfs1(g1);
    dfs1.run(0);

    

}
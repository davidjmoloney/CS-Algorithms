
//libraries includes
#include <iostream>
#include <string>
#include <ctime>
#include <vector>
#include <limits>
#include <list>


//using a number of calls from the std library
using namespace std;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//GRAPH CLASS
class graph {
public:
    graph(int size);  //constructor requires input size s 
    void rand_graph(int dens = 50, float max_wt = 0, float min_wt = 0);
    void print();
    int size_of();
    friend class bfs;

private:
    graph();    //default constructor
    int size;
    struct edge {
        int n;
        float wt;
    };

    //use list structure to represent adjacency list. Here we define the pointer the an array of lists -  will be defined in constructor of list
    list<edge> * adj_list;
    const float NAE = -1.0;     //const parameter for Not An Edge

};


graph::graph(int size) : size(size) {
    //define pointer to array of lists using dynamic memory allocation (new) 
    adj_list = new list<edge>[size];
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

    for (int i = 0; i < size; ++i) {
        cout << i << "\t|\t";

        //We must use iterator to move through list as it non-contigous memory allocated
        list<edge>::iterator it;

        for (it = adj_list[i].begin(); it != adj_list[i].end(); ++it) {
            cout << it->n << "(WT: " << it->wt << ") ";
        }
        cout << "\n\t|" << endl;
    }
}


// Breadth First Search Algorithm

class bfs {
public:
    bfs(graph gr) : gr(gr) {}
    bool run(int start = 0);

private:
    graph gr;
    vector<bool> visited;
};

bool bfs::run(int start) {

    visited.resize(gr.size, false);
    cout << "Starting at " << start << endl;

    list<int> queue = {start};
    visited[start]  = true;
    int current;

    while (!queue.empty()) {
        current = queue.front(); 
        queue.pop_front();

        cout << "Moving to node " << current << endl;

        auto adj = gr.adj_list[current];
        list<graph::edge>::iterator it;

        for (it = adj.begin(); it != adj.end(); ++it) {
            if (visited[it->n] == false) {
                queue.push_back(it->n);
                cout << "Adding " << it->n << " to queue" << endl;
                visited[it->n] = true;
            }
        }
        
    }

    for (const bool& check : visited)
        if (check == false) {
            cout << "Failed to find path to all nodes" << endl;
            return false;
        }

    return true;

}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Main function


int main(void) {

    srand((unsigned)time(0));

    graph g1(8);
    g1.rand_graph(40, 0, 100);
    g1.print();


    bfs bfs1(g1);
    bfs1.run(4);

    int x = 5, y = 2;
    if (x && y) {
        cout << "hi";
    }
    else {
        /*_part B_*/
    }

}

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

class quick {
public:
    quick(int size);  //constructor requires input size s 
    void rand_list();
    void print();
    void rec_sort(vector<int>::iterator first, vector<int>::iterator last);
    void sort();

private:
    quick();    //default constructor
    int size;
    vector<int> items;

};


quick::quick(int size): size(size) {

    items.clear();

    for (int i = 0; i < size; i++)
        items.push_back(rand() % 100);

}

void quick::print() {
    vector<int>::iterator it = items.begin();

    while (it != items.end()) {
        cout << *it << " "; 
        it++;
    }
    cout << endl << endl;
}

void quick::sort(){

    rec_sort(items.begin(), items.end() - 1);

}


void quick::rec_sort(vector<int>::iterator first, vector<int>::iterator last) {
    
    if (first < last ) {

        int temp = 0;
        vector<int>::iterator pos = first;
        vector<int>::iterator it;
        for ( it = first; it != last; it++) {
            //cout << *it << " ";
            if (*it < *last) {
                temp = *it;
                *it = *pos;
                *pos = temp;
                pos++;
            }
        }
        
        temp = *pos;
        *pos = *last;
        *last = temp;
        
        
        rec_sort(first, pos-1);
        rec_sort(pos + 1, last);
    
        
        
    }

    return;

}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Main function


int main(void) {

    srand(1);
    //srand((unsigned)time(0));

    quick q1(50);
    q1.print();

    q1.sort();
    q1.print();
}
#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <map>
#include <queue>
#include <stdlib.h>
#include <time.h> 
#include <cmath>

using namespace std;
typedef long long int ll;
typedef vector <int> vi;
typedef vector <vi> vvi;

template <typename Ty> 
class dlist{
    private:
        // Made by Max Balsells 2020
    
        struct Node {
            int pos;
            Ty v;
            Node *l, *r;
        };

        typedef Node* cnode;

        cnode position;
        int siz;
        
        void no_position(){
            cout << "The list is empty!" << endl;
        }

    public:

        dlist() {
            siz = 0;
            position = nullptr;
        }

        int size(){
            return siz;
        }

        bool first(){
            if (not position){
                no_position();
                return false;
            }

            return (not position -> l);
        }

        bool last(){
            if (not position){
                no_position();
                return false;
            }

            return (not position -> r);
        }

        void next(){
            if (not position){
                no_position();
                return;
            }

            if (last()) {
                cout << "this is already the last node" << endl;
                return;
            }

            position = position -> r;
            position -> pos = 1 + position -> l -> pos;
        }

        void previous(){
            if (not position){
                no_position();
                return;
            }

            if (first()) {
                cout << "this is already the first node" << endl;
                return;
            }

            position = position -> l;
        }

        Ty value(){
            if (not position){
                no_position();
                return 0; // change according to Type!
            }

            return position -> v;
        }

        void insert(Ty value){
            ++siz;

            if (not position) position = new Node {0, value, nullptr, nullptr};
            else {
                cnode a = new Node {(position -> pos) + 1, value, position, position -> r};
                

                if (not last()) position -> r -> l = a;
                position -> r = a;
            }
        }

        void move_to(int pos){
            if (pos < 0 or pos >= size()) {
                cout << "out of bounds" << endl;
                return;
            }

            while ((position -> pos) > pos) previous();
            while ((position -> pos) < pos) next();
        }

        void erase(){
            if (last()){
                cout << "this is the last node, there is none after" << endl;
                return;
            }

            --siz;

            if (not position -> r -> r){ //erasing last node
                delete position -> r;
                position -> r = nullptr;
            }
            else { // careful memory leaks !!!
                position -> r -> r -> l = position;
                position -> r = position -> r -> r;
            }
        }
};

int main() {
    dlist <int> D;

    cout << D.value() << endl;

    D.insert(1);
    D.insert(5);
    D.insert(4);
    D.insert(2);

    cout << D.value() << endl;

    while (not D.last()){
        D.next();
        cout << D.value() << endl;

        if (D.value() == 2) D.insert(3);  
        if (D.value() == 4) D.erase();
    }

    while (not D.first()){
        D.previous();
        cout << D.value() << endl;
    }
}


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
class indexed_multiset{
    private:
        // Made by Max Balsells 2020
    
        struct Node {
            Ty key;
            ll pr, childs, repetitions, deltas;
            Node *l, *r;
        };

        typedef Node* Treap;

        Treap root;

        int sum_childs(Treap T){
            if (not T) return 0;
            return T -> repetitions + T -> childs;
        }

        void propagate(Treap T){
            if (not T) return;

            T -> key += T -> deltas;
            if (T -> r) T -> r -> deltas += T -> deltas;
            if (T -> l) T -> l -> deltas += T -> deltas;
            
            T -> deltas = 0;
        }

        void update(Treap T){
            T -> childs = sum_childs(T -> r) + sum_childs(T -> l);
        }

        void split (Treap T, Ty k, Treap& L, Treap& R){
            propagate(T);

            if (not T) L = R = nullptr;

            else if (k < T -> key) {
                split(T -> l, k, L, T -> l);
                R = T;
                update(R);
            }

            else {
                split(T -> r, k, T -> r, R);
                L = T;
                update(L);
            }  
        }

        void merge (Treap& T, Treap L, Treap R) { // L < R (all values)
            propagate(T);

            if (not L) T = R;
            else if (not R) T = L;

            else if (L -> pr > R -> pr) {
                merge (L -> r, L -> r, R);
                T = L;
                update(T);

            }

            else {
                merge (R -> l, L, R -> l);
                T = R;
                update(T);
            }
        }

        void insert_treap (Treap& T, Treap a){
            propagate(T);

            if (not T) T = a;

            else if (a -> pr > T -> pr){
                split(T, a -> key, a -> l, a -> r);
                T = a;
            }

            else {
                if (a -> key < T -> key) insert_treap(T -> l, a);
                else insert_treap(T -> r, a);
            }

            update(T);
        }

        void erase_treap (Treap& T, Ty k) {
            propagate(T);

            if (T -> key == k) merge (T, T -> l, T -> r);
            
            else {
                if (k < T -> key) erase_treap (T -> l, k);
                else erase_treap (T -> r, k);
                update(T);
            }
        }

        Ty find_smaller(Treap T, int index){
            propagate(T);

            if (not T) return 0;

            int ch = sum_childs(T -> l);
            if (ch > index) return find_smaller(T -> l, index);
            index -= ch;

            if (index < T -> repetitions) return T -> key;
            return find_smaller(T -> r, index - T -> repetitions);
        }

        bool find_treap (Treap T, Ty k){
            propagate(T);

            if (not T) return false;

            if (T -> key > k) return find_treap(T -> l, k);
            if (T -> key < k) return find_treap(T -> r, k);

            return true;
        }

        bool add_repetition_treap (Treap T, Ty k, int times){
            propagate(T);

            if (not T) return false;

            if (T -> key > k) {
                bool ans = add_repetition_treap(T -> l, k, times);
                update(T);
                return ans;
            }
            if (T -> key < k) {
                bool ans = add_repetition_treap(T -> r, k, times);
                update(T);
                return ans;
            }

            T -> repetitions += times;

            update(T);
            return true;
        }

        bool subtract_repetition_treap (Treap T, Ty k, int times){
            propagate(T);

            if (not T) return false;

            if (T -> key > k) {
                bool ans = subtract_repetition_treap(T -> l, k, times);
                update(T);
                return ans;
            }
            if (T -> key < k) {
                bool ans = subtract_repetition_treap(T -> r, k, times);
                update(T);
                return ans;
            }

            T -> repetitions -= times;
            return T -> repetitions > 0;
        }

        int position_treap (Treap T, Ty k){
            propagate(T);

            if (not T) return -1; //should not happen;

            if (T -> key > k) return position_treap(T -> l, k);
            if (T -> key < k) return sum_childs(T -> l) + T -> repetitions + position_treap(T -> r, k);

            return sum_childs(T -> l);
        }

        int lower_bound_treap (Treap T, Ty k){
            propagate(T);

            if (not T) return 0;

            if (T -> key >= k) return min(sum_childs(T -> l), lower_bound_treap(T -> l, k));
            else return T -> repetitions + sum_childs(T -> l) + lower_bound_treap(T -> r, k);
        }

        void getvalues_treap(Treap T, vector <pair <Ty, int> >& V){
            propagate(T);

            if (not T) return;
            V.push_back({T -> key, T -> repetitions});

            getvalues_treap(T -> l, V);
            getvalues_treap(T -> r, V);
        }

    public:

        indexed_multiset () {root = nullptr;}

        void clear() {root = nullptr;}

        int size() {return sum_childs(root);} 

        bool find (Ty k){return find_treap(root, k);}

        void insert (Ty k, int times = 1){
            if (add_repetition_treap(root, k, times)) return;

            Treap a = new Node {k, rand(), 0, times, 0, nullptr, nullptr};
            insert_treap(root, a);
        }

        void erase (Ty k, int times = 1) {
            if (find(k)) {
                if (subtract_repetition_treap(root, k, times)) return;
                
                erase_treap(root, k);
            }
        }

        int lower_bound (Ty i) { //returns the pos of the first >= i
            return lower_bound_treap(root, i);
        }

        Ty operator[](int i) {
            if (i >= sum_childs(root)) {
                cout << " error! trying to access an element that does not exist" << endl;
                return -1e9;
            }

            return find_smaller(root, i);
        }

        int position(Ty k){
            if (not find(k)) return -1; //not found
            return position_treap(root, k);
        }

        void add(Ty k){
            if (not root) return;
            root -> deltas += k;
        }

        vector <pair <Ty, int> > getvalues(){
            vector <pair <Ty, int> > V;
            getvalues_treap(root, V);
            return V;
        }
};
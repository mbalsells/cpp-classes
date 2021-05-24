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
class indexed_set{
    private:
        struct Node {
            Ty key;
            ll pr, childs;
            Node *l, *r;
        };

        typedef Node* Treap;

        Treap root;

        int sum_childs(Treap T){
            if (not T) return 0;
            return 1 + T -> childs;
        }

        void update(Treap T){
            T -> childs = sum_childs(T -> r) + sum_childs(T -> l);
        }

        void split (Treap T, Ty k, Treap& L, Treap& R){
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

        void merge (Treap& T, Treap L, Treap R) {
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
            if (T -> key == k) merge (T, T -> l, T -> r);
            
            else {
                if (k < T -> key) erase_treap (T -> l, k);
                else erase_treap (T -> r, k);
                update(T);
            }
        }

        Ty find_smaller(Treap T, int index){
            if (not T) return 0;

            int ch = sum_childs(T -> l);
            if (ch > index) return find_smaller(T -> l, index);
            index -= ch;

            if (index == 0) return T -> key;
            return find_smaller(T -> r, index - 1);
        }

        bool find_treap (Treap T, Ty k){
            if (not T) return false;

            if (T -> key > k) return find_treap(T -> l, k);
            if (T -> key < k) return find_treap(T -> r, k);

            return T -> key == k;
        }

        int position_treap (Treap T, Ty k){
            if (not T) return -1; //should not happen;

            if (T -> key > k) return position_treap(T -> l, k);
            if (T -> key < k) return sum_childs(T -> l) + 1 + position_treap(T -> r, k);

            return sum_childs(T -> l);
        }

        int lower_bound_treap (Treap T, Ty k, bool& found){
            if (not T) return 0;

            if (T -> key >= k) {
                int value = lower_bound_treap(T -> l, k, found);
                if (found) return value;
                found = true;
                return sum_childs(T -> l);
            }
            else return 1 + sum_childs(T -> l) + lower_bound_treap(T -> r, k, found);
        }

        void copy_treap(Treap& T1, Treap T2){
            if (not T2) return;
            T1 = new Node{0, 0, 0, nullptr, nullptr};

            T1 -> key = T2 -> key;
            T1 -> pr = T2 -> pr;
            T1 -> childs = T2 -> childs;

            copy_treap(T1 -> l, T2 -> l);
            copy_treap(T1 -> r, T2 -> r);
        }

    public:

        indexed_set (Treap T = nullptr) {root = T;}

        void clear() {root = nullptr;}

        int size() {return sum_childs(root);} 

        bool find (Ty k){return find_treap(root, k);}

        void insert (Ty k){
            if (find(k)) return;

            Treap a = new Node {k, rand(), 0, nullptr, nullptr};
            insert_treap(root, a);
        }

        void erase (Ty k) {
            if (find(k)) erase_treap(root, k);
        }

        int lower_bound (Ty i) { //returns the pos of the first element >= i
            bool found = false;
            return lower_bound_treap(root, i, found);
        }

        Ty operator[](int i) {
            if (i >= sum_childs(root)) {
                cout << " error! trying to access an element that does not exist" << endl;
                return -1e9;
            }

            return find_smaller(root, i);
        }

        int position(Ty k){
            if (not find(k)) return -1;
            return position_treap(root, k);
        }

        indexed_set <Ty> copy(){
            Treap root2 = new Node {0, 0, 0, nullptr, nullptr};
            if (not root) return indexed_set <Ty>(nullptr);

            copy_treap(root2, root);
            return indexed_set <Ty>(root2);
        } 
};




int main (){
    ios::sync_with_stdio(0);
    cin.tie(0);    
    srand (time(NULL));
 
    int t;
    cin >> t;
 
    for (int w = 0; w < t; ++w){
        int n;
        cin >> n;
 
        string s;
        cin >> s;
 
        vi V(n, 0);
        for (int i = n-2; i >= 0; --i){
            if (s[i] == '>'){
                if (V[i+1] > 0) V[i] = V[i+1] + 1;
                else V[i] = 1; 
            }
            else {
                if (V[i+1] < 0) V[i] = V[i+1] - 1;
                else V[i] = -1;  
            }
        }   

        indexed_set <int> S;
        for (int i = 1; i <= n; ++i) S.insert(i);
 
        vi smaller;
        for (int i = 0; i < n; ++i){
            int index = -min(0, V[i]);
 
            int number = S[S.size() - index - 1];
            
            smaller.push_back(number);
            S.erase(number);
        }

        S.clear();

        for (int i = 1; i <= n; ++i) S.insert(i);
    
        vi greater;
        for (int i = 0; i < n; ++i){
            int index = max(0, V[i]);
            
            int number = S[index];

            greater.push_back(number);
            S.erase(number);
        }

        for (int i = 0; i < n; ++i) cout << smaller[i] << " ";
        cout << '\n';
        for (int i = 0; i < n; ++i) cout << greater[i] << " ";
        cout << '\n';
    }
}

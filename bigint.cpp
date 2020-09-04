#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <set>
#include <complex>

using namespace std;
typedef long long int ll;
typedef vector <int> vi;
typedef vector <vi> vvi;
typedef pair <int, int> ii;
typedef complex <double> cd;

const double pi = acos(-1);
const cd I(0, 1);
 
class bigint {
    private:
        string number;
        bool sign;

        void initFFT (vector <cd>& W, vector <int>& rbit, bool inverse) const {
            int n = W.size();

            for (int i = 0; i < n; ++i){
                int k = i, power = n/2, ans = 0;

                while (k > 0){
                    if (k % 2) ans += power;
                    power >>= 1;
                    k >>= 1;
                }

                rbit[ans] = i;
            }

            for (int i = 0; i < n; ++i) W[i] = exp(I*double(i*2*pi/n));

            if (inverse) for (int i = 1; i < n/2; ++i) swap(W[i], W[n-i]);
        }

        vector <cd> FFT (vector <cd> P, bool inverse) const {
            int n = P.size();

            vector <cd> W(n);
            vector <int> rbit(n);

            initFFT(W, rbit, inverse);

            vector <cd> Q(n);

            for (int i = 0; i < n; ++i) Q[i] = P[rbit[i]];

            for (int l = 1; l < n; l <<= 1){
                int step = n/(l << 1);

                for (int i = 0; i < n; i += l << 1){
                    int j = 0;

                    for (int k = 0; k < l; ++k){
                        cd u = Q[i+k], v = W[j] * Q[i+k+l];

                        Q[i+k] = u + v;
                        Q[i+k+l] = u - v;

                        j += step;
                    }
                }
            }

            if (inverse) for (int i = 0; i < n; ++i) Q[i] /= n;

            return Q;
        }

        string multiply (const string& a, const string& b) const {
            int m = 1 << (int) ceil(log2(a.size() + b.size() -1));

            vector <cd> A(m), B = A;

            for (int i = 0; i < a.size(); ++i) A[i] = a[i] - '0';
            for (int i = 0; i < b.size(); ++i) B[i] = b[i] - '0';

            vector <cd> DFTA = FFT(A, false), DFTB = FFT(B, false);
                
            vector <cd> result(m);
            for (int i = 0; i < m; ++i) result[i] = DFTA[i] * DFTB[i];
            result = FFT(result, true);
            
            string ans(m, '0');

            int carry = 0;
            for (int i = 0; i < m; ++i){
                int digit = (0.5 + real(result[m-i-1])) + carry;

                carry = digit/10;
                digit %= 10;

                ans[i] = '0' + digit;
            }

            if (carry) ans.push_back('0' + carry);

            while (ans.size() and ans.back() == '0') ans.pop_back();

            reverse(ans.begin(), ans.end());

            if (carry) carry = 1;

            while (ans.size() > a.size() + b.size() + carry - 1) ans.pop_back();
            if (ans.empty()) ans = "0";

            return ans;
        }

        string add(const string& a, const string& b) const {
            int na = a.size();
            int nb = b.size();

            int carry = 0;
            string ans;

            for (int i = 0; i < min(na, nb); ++i){
                int digit = carry + a[na-1-i] + b[nb-1-i] - 2 * '0';
                carry = digit / 10;
                digit %= 10;

                ans.push_back('0' + digit);
            }

            for (int i = min(na, nb); i < na; ++i){
                int digit = carry + a[na-1-i] - '0';
                carry = digit / 10;
                digit %= 10;

                ans.push_back('0' + digit);
            }

            for (int i = min(na, nb); i < nb; ++i){
                int digit = carry + b[nb-1-i] - '0';
                carry = digit / 10;
                digit %= 10;

                ans.push_back('0' + digit);
            }

            ans.push_back('0' + carry);

            while (ans.size() and ans.back() == '0') ans.pop_back(); 

            reverse(ans.begin(), ans.end());

            return ans;
        }

        string subtract(const string& a, const string& b) const {
            if (b.size() > a.size()) return "-" + subtract(b, a);
            if (b.size() == a.size() and a < b) return "-" + subtract(b, a);

            int na = a.size();
            int nb = b.size();

            int carry = 0;
            string ans;

            for (int i = 0; i < nb; ++i){
                int digit = a[na-1-i] - b[nb-1-i] - carry;
                
                if (digit >= 0) carry = 0;
                else {
                    digit += 10;
                    carry = 1;
                }

                ans.push_back('0' + digit);
            }

            for (int i = nb; i < na; ++i){
                int digit = a[na-1-i] - '0' - carry;
                
                if (digit >= 0) carry = 0;
                else {
                    digit += 10;
                    carry = 1;
                }

                ans.push_back('0' + digit);
            }

            while (ans.size() and ans.back() == '0') ans.pop_back(); 

            reverse(ans.begin(), ans.end());

            return ans;
        }


    public:
        bigint(string s = "0", bool negative = false){
            sign = negative;

            if (s[0] == '-'){
                sign = not sign;
                s.erase(s.begin());
            }

            number = s;
        }

        string print() const {
            if (sign and number != "0") return "-" + number;
            return number;
        }

        string to_string() const {
            return number;
        }

        bool negative() const {
            return sign;
        }

        bigint operator * (const bigint& a) const {
            return bigint(multiply(number, a.to_string()), sign ^ a.negative());
        }

        bigint operator + (const bigint& a) const {
            bool sign2 = a.negative();

            if (not sign and not sign2) return bigint(add(number, a.to_string()));
            if (not sign and sign2) return bigint(subtract(number, a.to_string()));
            if (sign and not sign2) return bigint(subtract(a.to_string(), number));
            return bigint(add(number, a.to_string()), 1);
        }

        bigint operator - (const bigint& a) const {
            bool sign2 = a.negative();

            if (not sign and sign2) return bigint(add(number, a.to_string()));
            if (not sign and not sign2) return bigint(subtract(number, a.to_string()));
            if (sign and sign2) return bigint(subtract(a.to_string(), number));
            return bigint(add(number, a.to_string()), 1);
        }

        bigint& operator *= (const bigint& a){
            bigint temp = *this * a;

            this -> number = temp.to_string();
            this -> sign = temp.negative();

            return *this;
        }

        bigint& operator += (const bigint& a){
            bigint temp = *this + a;

            this -> number = temp.to_string();
            this -> sign = temp.negative();

            return *this;
        }

        bigint& operator -= (const bigint& a){
            bigint temp = *this - a;

            this -> number = temp.to_string();
            this -> sign = temp.negative();

            return *this;
        }
};

int main(){
    string s, t;

    cin >> s >> t;

    bigint a(s), b(t);

    cout << (a * b).print() << endl;
    cout << (a + b).print() << endl;
    cout << (a - b).print() << endl;
}
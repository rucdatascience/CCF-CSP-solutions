#pragma GCC optimize(2)
#pragma GCC optimize(3)
#include<bits/stdc++.h>
#define ll long long
using namespace std;
inline void read(long long &x){
	x = 0;char ch = 0;
	while(ch < '0' || ch > '9'){ch = getchar();}
	while(ch >= '0' && ch <= '9') {x = x * 10 + (ch - '0'); ch = getchar();}
}
const long long mod = 998244353;
long long n, m, a[1000005];
long long f[1000005][12];
struct Matrix{
    ll** data;
    int n, m;
    Matrix(int n, int m):n(n), m(m){
        data = new ll * [n];
        for (int i = 0; i < n; i ++) {
            data[i] = new ll[m]();
        }
    }
    Matrix(const Matrix& other):n(other.n), m(other.m){
        data = new ll*[n];
        for (int i = 0; i < n; i ++) {
            data[i] = new ll[m];
            memcpy(data[i], other.data[i], m * sizeof(ll));
        }
    }
    ~Matrix(){
        for (int i = 0; i < n; i ++) {
            delete[] data[i];
        }
        delete[] data;
    }
    Matrix& operator =(const Matrix& other) {
        if (this == &other) return *this;
        for (int i = 0; i < n; i ++) {
            delete[] data[i];
        }
        delete[] data;
        n = other.n, m = other.m;
        data = new ll * [n];
        for (int i = 0; i < n; i ++) {
            data[i] = new ll[m];
            memcpy(data[i], other.data[i], m * sizeof(ll));
        }
        return *this;
    }
    Matrix operator *(const Matrix& other) const {
        Matrix res(n, other.m);
        for (int i = 0; i < n; i ++) {
            for (int k = 0; k < m; k ++) {
                if (data[i][k] == 0) continue;
				ll x = data[i][k];
                res.data[i][0] += x * other.data[k][0];
                res.data[i][1] += x * other.data[k][1];
                res.data[i][2] += x * other.data[k][2];
                res.data[i][3] += x * other.data[k][3];
                res.data[i][4] += x * other.data[k][4];
                res.data[i][5] += x * other.data[k][5];
                res.data[i][6] += x * other.data[k][6];
                res.data[i][7] += x * other.data[k][7];
                res.data[i][8] += x * other.data[k][8];
                res.data[i][9] += x* other.data[k][9];
                res.data[i][10] += x * other.data[k][10];
            }
        }
        for (int i = 0; i < n; i ++) {
		    for (int j = 0; j < other.m; j ++) {
		    	if (res.data[i][j] >= mod) res.data[i][j] = res.data[i][j] % mod;
		    }
        }
        return res;
    }
    static Matrix identity (int size) {
        Matrix I(size,size);
        for (int i = 0; i < size; i ++) {
            I.data[i][i] = 1;
        }
        return I;
    }
};
Matrix matrix_pow (Matrix base, ll exp) {
    int size = base.n;
    Matrix result = Matrix::identity(size);
    while (exp > 0) {
        if (exp & 1) {
            result = result * base;
        }
        base = base * base;
        exp >>= 1;
    }
    return result;
}
int main(){
//	freopen("10.in", "r", stdin);
//	freopen("10.out", "w", stdout);
	read(n); read(m);
	for (int i = 1; i <= m; i ++) {
		read(a[i]);
	}
	sort(a +1 , a + m + 1);
	
	Matrix M(11, 11);
	M.data[0][0] = 25, M.data[1][0] = 25, M.data[2][0] = 24;
	M.data[0][1] = 1;
	M.data[1][2] = 1, M.data[2][2] = 1;
	M.data[2][3] = 1;
	M.data[3][4] = M.data[4][4] = 25, M.data[5][4] = M.data[6][4] = M.data[7][4] = M.data[8][4] = M.data[9][4] = 24;
	M.data[3][5] = M.data[4][5] = M.data[5][5] = M.data[6][5] = M.data[7][5] = M.data[8][5] = M.data[9][5] = 1;
	M.data[5][6] = 1;
	M.data[6][7] = 1;
	M.data[7][8] = 1;
	M.data[8][9] = 1;
	M.data[9][10] = 1, M.data[10][10] = 26;
	
	Matrix B(11,11);
	B.data[0][0] = B.data[1][0] = B.data[2][0] = 1;
	B.data[3][4] = B.data[4][4] = B.data[5][4] = B.data[6][4] = B.data[7][4] = B.data[8][4] = B.data[9][4] = 1;
	B.data[10][10] = 1;
	
	Matrix Ans(1, 11);
	Ans.data[0][0] = 1;
	
	for (int i = 1; i <= m; i ++) {
		Ans = Ans * matrix_pow(M, a[i] - a[i - 1] - 1);
//		Ans*=matrix_pow(M,a[i]-a[i-1]-1);
		Ans = Ans * B;
//		Ans*=B;
	}
	Ans = Ans * matrix_pow(M, n - a[m]);
	printf("%lld", Ans.data[0][10]);
	return 0;
}
/*

9 0

12 1
11

16 1
10

*/

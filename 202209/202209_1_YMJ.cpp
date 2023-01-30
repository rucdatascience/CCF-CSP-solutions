/*
CSP202209_1 如此编码
得分：100
*/

#include <bits/stdc++.h>

#define fi first
#define se second
#define pb push_back
#define siz(x) ((int)x.size())
#define lowbit(x) ((x) & (-(x)))
#define lson (k<<1)
#define rson (k<<1|1)

#define rep(i,s,t) for (int i = (s), _t = (t); i < _t; ++i)
#define per(i,s,t) for (int i = (t) - 1, _s = (s); i >= _s; --i)

using namespace std;

typedef long long ll;
typedef unsigned long long ull;
typedef pair<int,int> pii;
typedef vector<int> veci;

template<class T> void Min(T &a, T b)
{
	if (b < a) a = b;
}

template<class T> void Max(T &a, T b)
{
	if (b > a) a = b;
}

const int N = 25;

int a[N], b[N], c[N];

int main()
{
	int n, m;
	cin >> n >> m;
	c[0] = 1;
	rep(i,1,n+1)
	{
		cin >> a[i];
		c[i] = c[i-1] * a[i];
	}
	rep(i,1,n+1)
	{
		b[i] = m % c[i] / c[i-1];
		m -= c[i-1] * b[i];
		cout << b[i] << " \n"[i==n];
	}
	return 0;
}

#include <bits/stdc++.h>

#define fi first
#define se second
#define pb push_back
#define siz(x) ((int)x.size())
#define lowbit(x) ((x) & (-(x)))
#define lson (k<<1)
#define rson (k<<1|1)
#define debug(x) cerr << #x << " = " << (x) << endl

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

const int N = (int)1e3 + 5;


double num[N];

int main()
{
	int n;
	cin >> n;
	double sum = 0;
	rep(i,0,n)
	{
		scanf("%lf", &num[i]);
		sum += num[i];
	}
	double ave = sum / n;
	double D = 0;
	rep(i,0,n)
	{
		D += (num[i] - ave) * (num[i]- ave);
	}
	D /= n;
	double sigma = sqrt(D);
	rep(i,0,n)
	{
		printf("%.10f\n", (num[i] - ave) / sigma);
	}
	return 0;
}

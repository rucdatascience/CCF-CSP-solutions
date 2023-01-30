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

const int N = (int)1e5 + 5;

int v[N], m[N];
int sum[N];
ll dp[N], x[N], y[N];
int que[N];

ll sqr(int x)
{
	return (ll)x * x;
}

ll calc(int id, int s)
{
	return y[id] - s * x[id];
}

int main()
{
	int n,L;
	cin >> n >> L;
	rep(i,1,n+1)
	{
		scanf("%d", &v[i]);
		sum[i] = sum[i-1] + v[i];
	}
	rep(i,1,n+1)
	{
		scanf("%d", &m[i]);
	}
	if (n <= 2000)
	{
		rep(i,1,n+1)
		{
			dp[i] = LLONG_MAX;
			rep(j,0,i) if(m[j] < m[i])
			{
				Min(dp[i], dp[j] + sqr(sum[i]-sum[j]-L));
			}
		}
	}
	else
	{
		int l = 0, r = 0;
		que[r++] = 0;
		rep(i,1,n+1)
		{
			while (l < r-1 && calc(que[l+1], sum[i]) < calc(que[l], sum[i])) ++l;
			int j = que[l];
			dp[i] = y[j] - (ll)sum[i] * x[j];
			dp[i] += sqr(sum[i]) - 2ll * L * sum[i] + sqr(L);
			x[i] = 2 * sum[i];
			y[i] = dp[i] + sqr(sum[i]) + 2ll * L * sum[i];
			while (l < r-1 && (__int128)(y[que[r-1]]-y[que[r-2]])*(x[i]-x[que[r-2]])>=(__int128)(y[i]-y[que[r-2]])*(x[que[r-1]]-x[que[r-2]])) --r;
			que[r++] = i;
		}
	}
	cout << dp[n] << endl;
	return 0;
}

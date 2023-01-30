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

const int N = 30;

int a[N], sum[1<<(N/2)];

int main()
{
	int n, x;
	cin >> n >> x;
	rep(i,0,n)
	{
		scanf("%d", &a[i]);
	}
	int m = n>>1, m_ = n-m;
	rep(mask,0,1<<m)
	{
		int s = 0;
		rep(i,0,m)if(mask&(1<<i))
		{
			s += a[i];
		}
		sum[mask] = s;
	}
	sort(sum, sum+(1<<m));
	int ans = INT_MAX;
	rep(mask,0,1<<m_)
	{
		int s = 0;
		rep(i,0,m_)if(mask&(1<<i))
		{
			s += a[i+m];
		}
		auto tmp = lower_bound(sum, sum+(1<<m), x-s);
		if(tmp != sum+(1<<m))
		{
			Min(ans, s + *tmp);
		}
	}
	cout << ans << endl;
	return 0;
}

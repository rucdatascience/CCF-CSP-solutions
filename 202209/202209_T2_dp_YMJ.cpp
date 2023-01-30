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

const int N = 35, M = (int)1e4 + 5;

int a[N];
bool dp[N * M];

int main()
{
	int n,x;
	cin >> n >> x;
	dp[0] = true;
	int ans = INT_MAX;
	int sum = 0;
	rep(i,0,n)
	{
		cin >> a[i];
		per(j,0,sum+1)if(dp[j])
		{
			dp[j+a[i]]=true;
			if(j+a[i]>=x) Min(ans,j+a[i]);
		}
		sum += a[i];
	}
	cout << ans << endl;
	return 0;
}

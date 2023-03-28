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

const int N = (int)1e3 + 5, S = 55;

int mp[S][S];
set<pii> trees;

int main()
{
	int n, L, S;
	cin >> n >> L >> S;
	rep(i,0,n)
	{
		pii tmp;
		scanf("%d%d", &tmp.fi, &tmp.se);
		trees.insert(tmp);
	}
	per(i,0,S+1)rep(j,0,S+1)
	{
		scanf("%d", &mp[i][j]);
	}
	int ans = 0;
	for(pii tree : trees) if(tree.fi+S <= L && tree.se+S <= L)
	{
		bool flag = true;
		rep(x,0,S+1)rep(y,0,S+1)
		{
			if(mp[x][y]!=trees.count(pii(tree.fi+x,tree.se+y)))
			{
				flag = false;
				break;
			}
		}
		ans += flag;
	}
	cout << ans << endl;
	return 0;
}

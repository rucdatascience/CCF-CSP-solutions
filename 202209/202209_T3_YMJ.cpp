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

const int N = (int)1e3 + 15;

int m[N];
set<int> risk[N];

struct Unit
{
	int d, u, r;
	void input()
	{
		scanf("%d%d%d", &d, &u, &r);
	}
};

Unit dat[N][N];

int main()
{
	int n;
	cin >> n;
	rep(i,0,n)
	{
		int r;
		scanf("%d%d", &r, &m[i]);
		for (int x; r--; )
		{
			scanf("%d", &x);
			rep(d,i,i+7)
			{
				risk[d].insert(x);
			}
		}
		rep(j,0,m[i])
		{
			dat[i][j].input();
		}
		printf("%d", i);
		set<int> ans;
		rep(j,max(0,i-6),i+1)
		{
			rep(k,0,m[j])
			{
				Unit tmp = dat[j][k];
				int d = tmp.d, u = tmp.u, r = tmp.r;
				if (d < i-6) continue;
				bool mark = true;
				rep(t,d,i+1) if (!risk[t].count(r))
				{
					mark = false;
					break;
				}
				if(mark) ans.insert(u);
			}
		}
		for (int u : ans)
		{
			printf(" %d", u);
		}
		putchar('\n');
	}
	return 0;
}

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

int n, L, v[N], m[N];
int sum[N];
ll dp[N], x[N], y[N];
int que[N];

struct Ope
{
	int id, m;
	Ope() {}
	Ope(int id_, int m_): id(id_), m(m_) {}
}
ope[N], tmp[N];

bool cmp_id(Ope &A, Ope &B)
{
	return A.id < B.id;
}

bool cmp_m(Ope &A, Ope &B)
{
	return A.m < B.m;
}

ll sqr(int x)
{
	return (ll)x * x;
}

ll calc(int id, int s)
{
	return y[id] - s * x[id];
}

void solve(int m_L, int m_R, int pos_L, int pos_R)
{
	if (m_L >= m_R || pos_L >= pos_R) return;
	int m_mid = (m_L + m_R) >> 1;
	int pos_mid = pos_L - 1;
	rep(i,pos_L,pos_R+1)
	{
		if (ope[i].m <= m_mid)
		{
			pos_mid = i;
		}
	}
	solve(m_L, m_mid, pos_L, pos_mid);
	int cnt = 0;
	rep(i,pos_L,pos_R+1)
	{
		tmp[cnt++] = ope[i];
	}
	sort(tmp, tmp + cnt, cmp_id);
	int l = 0, r = 0; // for queue
	rep(i,0,cnt)
	{
		int id = tmp[i].id;
		if (tmp[i].m <= m_mid)
		{
			while (l < r-1 &&
				   (__int128)(y[que[r-1]]-y[que[r-2]]) * (x[id]-x[que[r-2]]) >=
				   (__int128)(y[id]-y[que[r-2]]) * (x[que[r-1]]-x[que[r-2]]))
			{
				--r;
			}
			que[r++] = id;
		}
		else
		{
			while (l < r-1 && calc(que[l+1], sum[id]) < calc(que[l], sum[id])) ++l;
			int j = que[l];
			Min(dp[id], y[j] - (ll)sum[id] * x[j] + sqr(sum[id]) - 2ll * L * sum[id] + sqr(L));
			x[id] = 2 * sum[id];
			y[id] = dp[id] + sqr(sum[id]) + 2ll * L * sum[id];
		}
	}
	solve(m_mid + 1, m_R, pos_mid + 1, pos_R);
}

int main()
{
	cin >> n >> L;
	rep(i,1,n+1)
	{
		scanf("%d", &v[i]);
		sum[i] = sum[i-1] + v[i];
		dp[i] = LLONG_MAX;
	}
	rep(i,1,n+1)
	{
		scanf("%d", &m[i]);
		ope[i] = Ope(i, m[i]);
	}
	ope[0] = Ope(0, 0);
	sort(ope, ope + n + 1, cmp_m);
	solve(0, N, 0, n);
	cout << dp[n] << endl;
	return 0;
}

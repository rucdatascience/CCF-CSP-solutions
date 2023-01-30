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

const int M = (int)2e5 + 5;

int allc;
int id2alias[M], alias2id[M];
int par[M];
int ans[M];

struct Itv
{
	int L, R, alias;
	Itv(int L_,int R_,int alias_):L(L_),R(R_),alias(alias_){}
	bool operator <(const Itv &rhs)const
	{
		return L < rhs.L;
	}
};

struct Res
{
	int num, id;
	Res(int num_,int id_):num(num_),id(id_){}
	bool operator <(const Res &rhs)const
	{
		if(num != rhs.num) return num < rhs.num;
		return id > rhs.id;
	}
};

set<Itv> st_itv;
set<Res> st_res;

int get_root(int u)
{
	return par[u] == u ? u : par[u] = get_root(par[u]);
}

void upd_itv(int L,int R,int id)
{
	vector<Itv> tmp_ins, tmp_del;
	auto ite = st_itv.lower_bound(Itv(R,R,0));
	if (ite == st_itv.end() || ite->L > R)
	{
		--ite;
	}
	for (; ; --ite)
	{
		if (ite->R < L) break;
		tmp_del.pb(*ite);
		if (ite->L < L)
		{
			tmp_ins.pb(Itv(ite->L, L-1, ite->alias));
		}
		if (ite->R > R)
		{
			tmp_ins.pb(Itv(R+1, ite->R, ite->alias));
		}
		if (ite == st_itv.begin()) break;
	}
	for (auto tmp : tmp_del)
	{
		st_itv.erase(tmp);
		int tmp_id = alias2id[get_root(tmp.alias)];
		st_res.erase(Res(ans[tmp_id], tmp_id));
		ans[tmp_id] -= tmp.R - tmp.L + 1;
		if (tmp_id && ans[tmp_id])
		{
			st_res.insert(Res(ans[tmp_id], tmp_id));
		}
	}
	for (auto tmp : tmp_ins)
	{
		st_itv.insert(tmp);
		int tmp_id = alias2id[get_root(tmp.alias)];
		st_res.erase(Res(ans[tmp_id], tmp_id));
		ans[tmp_id] += tmp.R - tmp.L + 1;
		if (tmp_id)
		{
			st_res.insert(Res(ans[tmp_id], tmp_id));
		}
	}
	int alias = id2alias[id];
	Itv itv=Itv(L,R,alias);
	st_itv.insert(itv);
	st_res.erase(Res(ans[id], id));
	ans[id] += R-L+1;
	st_res.insert(Res(ans[id], id));
}

void upd_union(int x,int w)
{
	st_res.erase(Res(ans[w],w));
	ans[w] += ans[x];
	if (w && ans[w])
	{
		st_res.insert(Res(ans[w],w));
	}
	st_res.erase(Res(ans[x],x));
	ans[x] = 0;
	par[id2alias[x]] = id2alias[w];
	id2alias[x] = ++allc;
	alias2id[allc] = x;
}

void swap_id(int x,int y)
{
	st_res.erase(Res(ans[x],x));
	st_res.erase(Res(ans[y],y));
	swap(ans[x], ans[y]);
	if (ans[x])
	{
		st_res.insert(Res(ans[x],x));
	}
	if (ans[y])
	{
		st_res.insert(Res(ans[y],y));
	}
	swap(id2alias[x], id2alias[y]);
	swap(alias2id[id2alias[x]], alias2id[id2alias[y]]);
}

int get_winner()
{
	if (st_res.empty())
	{
		return 0;
	}
	auto ite = st_res.end();
	int ans = (--ite)->id;
	return ans;
}

int main()
{
	int n, m, q;
	cin >> n >> m >> q;
	rep(i,0,m+1)
	{
		id2alias[i] = alias2id[i] = i;
	}
	rep(i,0,M) par[i] = i;
	allc = m;
	ans[0] = n;
	st_itv.insert(Itv(1,n,0));
	for (int ope, L, R, x, y, w; q--; )
	{
		scanf("%d", &ope);
		switch(ope)
		{
		case 1:
			scanf("%d%d%d", &L, &R, &x);
			upd_itv(L, R, x);
			break;
		case 2:
			scanf("%d%d", &x, &w);
			upd_union(x, w);
			break;
		case 3:
			scanf("%d%d", &x, &y);
			swap_id(x, y);
			break;
		case 4:
			scanf("%d", &w);
			printf("%d\n", ans[w]);
			break;
		case 5:
			printf("%d\n", get_winner());
			break;
		}
	}
	return 0;
}

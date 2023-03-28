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

const int M = (int)1e5 + 5;

struct Mirror
{
	int x1, y1, x2, y2;
	double a;
	Mirror() {}
	Mirror(int x1_, int y1_, int x2_, int y2_, double a_):
		x1(x1_), y1(y1_), x2(x2_), y2(y2_), a(a_) {}
}
mirrors[M];

struct Ref
{
	int x, y, slope;
	double a;
	Ref(int x_, int y_):
		x(x_), y(y_) {}
	Ref(int x_, int y_, int slope_, double a_):
		x(x_), y(y_), slope(slope_), a(a_) {}
};

struct Ref_cmp_row
{
	bool operator() (const Ref &a, const Ref &b)
	{
		if (a.x != b.x) return a.x < b.x;
		return a.y < b.y;
	}
};

struct Ref_cmp_col
{
	bool operator() (const Ref &a, const Ref &b)
	{
		if (a.y != b.y) return a.y < b.y;
		return a.x < b.x;
	}
};

set<Ref, Ref_cmp_row> refs_row;
set<Ref, Ref_cmp_col> refs_col;

void add_mirror(int id, int x1, int y1, int x2, int y2, double a)
{
	if (x1 > x2)
	{
		swap(x1, x2);
		swap(y1, y2);
	}
	mirrors[id] = Mirror(x1, y1, x2, y2, a);
	int slope = (y2-y1) / (x2-x1);
	rep(x,x1+1,x2)
	{
		int y = y1 + (x - x1) * slope;
		refs_row.insert(Ref(x, y, slope, a));
		refs_col.insert(Ref(x, y, slope, a));
	}
}

void del_mirror(int id)
{
	auto mirror = mirrors[id];
	int x1 = mirror.x1, y1 = mirror.y1, x2 = mirror.x2, y2 = mirror.y2;
	double a = mirror.a;
	int slope = (y2-y1) / (x2-x1);
	rep(x,x1+1,x2)
	{
		int y = y1 + (x - x1) * slope;
		refs_row.erase(Ref(x, y, slope, a));
		refs_col.erase(Ref(x, y, slope, a));
	}
}

void query(int x, int y, int d, double I, int t)
{
	while (t)
	{
		if (d == 0)
		{
			auto ite = refs_col.upper_bound(Ref(x, y));
			if (ite != refs_col.end() && ite->y == y && ite->x - x <= t)
			{
				t -= ite->x - x;
				x = ite->x;
				I *= ite->a;
				d = (ite->slope==1 ? 1 : 3);
			}
			else
			{
				x += t;
				t = 0;
			}
		}
		else if (d == 1)
		{
			auto ite = refs_row.upper_bound(Ref(x, y));
			if (ite != refs_row.end() && ite->x == x && ite->y - y <= t)
			{
				t -= ite->y - y;
				y = ite->y;
				I *= ite->a;
				d = (ite->slope==1 ? 0 : 2);
			}
			else
			{
				y += t;
				t = 0;
			}
		}
		else if (d == 2)
		{
			auto ite = refs_col.lower_bound(Ref(x, y));
			if (ite == refs_col.begin())
			{
				x -= t;
				t = 0;
			}
			else
			{
				--ite;
				if (ite->y == y && x - ite->x <= t)
				{
					t -= x - ite->x;
					x = ite->x;
					I *= ite->a;
					d = (ite->slope==1 ? 3 : 1);
				}
				else
				{
					x -= t;
					t = 0;
				}
			}
		}
		else
		{
			auto ite = refs_row.lower_bound(Ref(x, y));
			if (ite == refs_row.begin())
			{
				y -= t;
				t = 0;
			}
			else
			{
				--ite;
				if (ite->x == x && y - ite->y <= t)
				{
					t -= y - ite->y;
					y = ite->y;
					I *= ite->a;
					d = (ite->slope==1 ? 2 : 0);
				}
				else
				{
					y -= t;
					t = 0;
				}
			}
		}
		if (I < 1)
		{
			printf("0 0 0\n");
			return;
		}
	}
	printf("%d %d %d\n", x, y, (int)I);
}

int main()
{
	int m;
	cin >> m;
	rep(i,1,m+1)
	{
		int type, x1, y1, x2, y2, k, x, y, d, t;
		double a, I;
		scanf("%d", &type);
		switch(type)
		{
		case 1:
			scanf("%d%d%d%d%lf", &x1, &y1, &x2, &y2, &a);
			add_mirror(i, x1, y1, x2, y2, a);
			break;
		case 2:
			scanf("%d", &k);
			del_mirror(k);
			break;
		case 3:
			scanf("%d%d%d%lf%d", &x, &y, &d, &I, &t);
			query(x, y, d, I, t);
			break;
		}
	}
	return 0;
}

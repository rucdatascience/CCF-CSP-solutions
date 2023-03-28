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

typedef pair<double, double> Point;
typedef Point Vec;

Vec operator + (const Point &A, const Point &B)
{
	return Vec(A.fi + B.fi, A.se + B.se);
}

Vec operator - (const Vec &A, const Vec &B)
{
	return Vec(A.fi - B.fi, A.se - B.se);
}

double Dot(const Vec &A, const Vec &B)
{
	return A.fi * B.fi + A.se * B.se;
}

double length(const Vec &A)
{
	return sqrt(Dot(A, A));
}

Vec operator * (double k, const Vec &A)
{
	return Vec(A.fi * k, A.se * k);
}

Vec operator / (const Vec &A, double k)
{
	return Vec(A.fi / k, A.se / k);
}

const int N = (int)2e3 + 5;

Point dat[N];

double sqr(double x)
{
	return x * x;
}

void upd_add(int L, int R)
{
	double a, b;
	scanf("%lf%lf", &a, &b);
	rep(i,L,R+1)
	{
		dat[i].fi += a;
		dat[i].se += b;
	}
}

void upd_rotate(int L, int R)
{
	double a, b, theta;
	scanf("%lf%lf%lf", &a, &b, &theta);
	double s = sin(theta), c = cos(theta);
	rep(i,L,R+1)
	{
		double x = dat[i].fi - a, y = dat[i].se - b;
		dat[i].fi = a + c * x - s * y;
		dat[i].se = b + s * x + c * y;
	}
}

void upd_zoom(int L, int R)
{
	double a, b, lambda;
	scanf("%lf%lf%lf", &a, &b, &lambda);
	rep(i,L,R+1)
	{
		dat[i].fi = (dat[i].fi - a) * lambda + a;
		dat[i].se = (dat[i].se - b) * lambda + b;
	}
}

void upd_reflect(int L, int R)
{
	double theta, y_0;
	scanf("%lf%lf", &theta, &y_0);
	double slope = tan(theta);
	Point P1 = Point(0, y_0), P2 = Point(1, y_0 + slope);
	Vec v = P2 - P1;
	v = v / length(v);
	rep(i,L,R+1)
	{
		Point proj = P1 + Dot(v, dat[i] - P1) * v;
		dat[i] = 2 * proj - dat[i];
	}
}

void upd_project(int L, int R)
{
	double theta, y_0;
	scanf("%lf%lf", &theta, &y_0);
	double slope = tan(theta);
	Point P1 = Point(0, y_0), P2 = Point(1, y_0 + slope);
	Vec v = P2 - P1;
	v = v / length(v);
	rep(i,L,R+1)
	{
		dat[i] = P1 + Dot(v, dat[i] - P1) * v;
	}
}

void query_average(int L, int R)
{
	double sum_x = 0, sum_y = 0;
	rep(i,L,R+1)
	{
		sum_x += dat[i].fi;
		sum_y += dat[i].se;
	}
	int len = R - L + 1;
	printf("%.10f %.10f\n", sum_x / len, sum_y / len);
}

void query_dist_sum(int L, int R)
{
	double a, b;
	scanf("%lf%lf", &a, &b);
	double sum = 0;
	rep(i,L,R+1)
	{
		sum += sqr(dat[i].fi - a) + sqr(dat[i].se - b);
	}
	printf("%.10f\n", sum);
}

int main()
{
	int n, q;
	cin >> n >> q;
	rep(i,1,n+1)
	{
		scanf("%lf%lf", &dat[i].fi, &dat[i].se);
	}
	while (q--)
	{
		int ope, L, R;
		scanf("%d%d%d", &ope, &L, &R);
		switch (ope)
		{
		case 1:
			upd_add(L, R);
			break;
		case 2:
			upd_rotate(L, R);
			break;
		case 3:
			upd_zoom(L, R);
			break;
		case 4:
			upd_reflect(L, R);
			break;
		case 5:
			upd_project(L, R);
			break;
		case 6:
			query_average(L, R);
			break;
		case 7:
			query_dist_sum(L, R);
			break;
		}
	}
	return 0;
}

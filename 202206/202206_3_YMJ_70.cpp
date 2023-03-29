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

const int N = (int)505;

struct Role
{
	set<string> operations, resource_types, resource_names;
};

map<string, Role> roles;
map<string, set<string> > username2roles, usergroup2roles;
string groups[N];

int main()
{
	ios::sync_with_stdio(false);
	cin.tie(0);
	int n, m, q;
	cin >> n >> m >> q;
	rep(i,0,n)
	{
		string name, tmp;
		int nv, no, nn;
		Role role;
		cin >> name;
		for(cin >> nv; nv--; )
		{
			cin >> tmp;
			role.operations.insert(tmp);
		}
		for(cin >> no; no--; )
		{
			cin >> tmp;
			role.resource_types.insert(tmp);
		}
		for(cin >> nn; nn--; )
		{
			cin >> tmp;
			role.resource_names.insert(tmp);
		}
		roles[name] = role;
	}
	rep(i,0,m)
	{
		string name, tmp1, tmp2;
		int ns;
		cin >> name >> ns;
		while (ns--)
		{
			cin >> tmp1 >> tmp2;
			if (tmp1 == "u")
			{
				username2roles[tmp2].insert(name);
			}
			else
			{
				usergroup2roles[tmp2].insert(name);
			}
		}
	}
	while (q--)
	{
		string username, operation, resource_type, resource_name;
		int ng;
		cin >> username >> ng;
		rep(i,0,ng)
		{
			cin >> groups[i];
		}
		cin >> operation >> resource_type >> resource_name;
		set<string> valid;
		for (auto tmp : roles)
		{
			auto role = tmp.se;
			if ((role.operations.count(operation) || role.operations.count("*"))
				&& (role.resource_types.count(resource_type) || role.resource_types.count("*"))
				&& (role.resource_names.count(resource_name) || role.resource_names.size() == 0))
			{
				valid.insert(tmp.fi);
			}
		}
		bool flag = false;
		for (auto role_name : username2roles[username])
		{
			if (valid.count(role_name))
			{
				flag = true;
				break;
			}
		}
		rep(i,0,ng)
		{
			if (flag) break;
			for (auto role_name : usergroup2roles[groups[i]])
			{
				if (valid.count(role_name))
				{
					flag = true;
					break;
				}
			}
		}
		cout<< flag << endl;
	}
	return 0;
}

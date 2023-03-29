/*可能超时得到90分*/
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
}
roles[N];

map<string, int> rolename2id, username2id, groupname2id;
bitset<N> user2roles[N*N], group2roles[N*N];
string groups[N];

int main()
{
	ios::sync_with_stdio(false);
	cin.tie(0);
	int n, m, q, allc_user = 0, allc_group = 0;
	cin >> n >> m >> q;
	rep(i,0,n)
	{
		string name, tmp;
		int nv, no, nn;
		Role role;
		cin >> name;
		rolename2id[name] = i;
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
		roles[i] = role;
	}
	rep(i,0,m)
	{
		string rolename, type, name;
		int ns;
		cin >> rolename >> ns;
		int role_id = rolename2id[rolename];
		while (ns--)
		{
			cin >> type >> name;
			if (type == "u")
			{
				if (!username2id.count(name))
				{
					username2id[name] = allc_user++;
				}
				user2roles[username2id[name]].set(role_id);
			}
			else
			{
				if (!groupname2id.count(name))
				{
					groupname2id[name] = allc_group++;
				}
				group2roles[groupname2id[name]].set(role_id);
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
		bitset<N> valid;
		rep(i,0,n)
		{
			if ((roles[i].operations.count(operation) || roles[i].operations.count("*"))
				&& (roles[i].resource_types.count(resource_type) || roles[i].resource_types.count("*"))
				&& (roles[i].resource_names.count(resource_name) || roles[i].resource_names.size() == 0))
			{
				valid.set(i);
			}
		}
		bool flag = false;
		if (username2id.count(username))
		{
			flag = (valid & user2roles[username2id[username]]).any();
		}
		if (!flag)
		{
			rep(i,0,ng)
			{
				if (groupname2id.count(groups[i]))
				{
					flag = (valid & group2roles[groupname2id[groups[i]]]).any();	
				}
				if (flag) break;
			}
		}
		cout << flag << endl;
	}
	return 0;
}

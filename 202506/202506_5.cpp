#include<bits/stdc++.h>
using namespace std;
vector<int> a[200020];
int Time = 0, dfn[200020], low[200020];
vector<int> b[200020 << 1];
vector<pair<int,int> > d[200020 << 1];
int cnt;
stack<int> s;
int N;
int n, m, q;
void tarjan (int x, int pre) {
    dfn[x] = low[x] = ++ Time;
    s.push(x);
    for(auto i:a[x]){
        if (i == pre) continue;
        if (!dfn[i]) {
            tarjan(i,x);
            low[x] = min(low[x], low[i]);
            if (low[i] >= dfn[x]) {
                ++ N;
                while (s.top() != i) {
                    b[N].push_back(s.top());
                    b[s.top()].push_back(N);
                    s.pop();
                }
                s.pop();
                b[N].push_back(i);
                b[i].push_back(N);
                b[N].push_back(x);
                b[x].push_back(N);
            }
        }
        else low[x] = min(low[x], dfn[i]);
    }
}
int dep[200020 << 1], dep_v[200020 << 1];
int fa[200020 << 1][30];
int col = 0;
void dfs (int x, int pre) {
    dep[x] = dep[pre] + 1;
    dep_v[x] = dep_v[pre] + (x <= n);
    fa[x][0] = pre;
    dfn[x] = ++ col;
    for (int j = 1; j <= __lg(dep[x]); j ++) {
        fa[x][j] = fa[fa[x][j - 1]][j - 1];
    }
    for (auto i:b[x]) {
        if (i == pre) continue;
        dfs(i,x);
	}
}
int LCA (int u, int v) {
    if (dep[u] < dep[v]) swap(u, v);
    int len = dep[u] - dep[v];
    while (len) u = fa[u][__lg(len)], len ^= (1 << __lg(len));
    if (u == v) return u;
    for (int i = __lg(dep[u] - 1); i >= 0; i --) {
        if (fa[u][i] != fa[v][i]) u = fa[u][i], v = fa[v][i];
    }
    return fa[u][0];
}
inline bool cmp (const int& x, const int& y) {
	return dfn[x] < dfn[y];
}
int siz[200020 << 1];
int c[200020];
long long c0, ans;
long long tot = 0;
void dfs2 (int x, int fa) {
	int mxsz = 0;
	for (auto& edge:d[x]) {
		int v = edge.first, w = edge.second;
		if (v != fa) {
			dfs2(v, x);
			mxsz = max(mxsz, siz[v]);
			siz[x] += siz[v];
			ans += max((long long)siz[v], (long long)(c0 - siz[v])) * (long long)w;
		}
	}
	if (x <= n) {
		tot --;
		ans += max((long long)mxsz, c0 - siz[x]);
	}
}
int main(){
//	freopen("8.in", "r", stdin);
//	freopen("8.out", "w", stdout);
	cin >> n >> m >> q;
	N = n;
	for (int i = 1; i <= m; i ++) {
		int u, v;
		cin >> u >> v;
		a[u].push_back(v),a[v].push_back(u);
	}
	for (int i = 1; i <= n; i ++) {
		if(!dfn[i]) tarjan(i, 0);
	}
	dfs(1, 0);
	while (q --) {
		cin >> c[0];
		c0 = c[0];
		tot = n;
		for (int i = 1; i <= c[0]; i ++) {
			cin >> c[i];
			siz[c[i]] = 1;
		}
		int res = c[0];
		sort(c + 1, c + res + 1, cmp);
		for (int i = 1; i < res; i ++) {
			c[++ c[0]] = LCA(c[i], c[i + 1]);
		}
		sort(c + 1, c + c[0] + 1, cmp);
		c[0] = unique(c + 1, c + c[0] + 1) - c - 1;
		ans = 0;
		for (int i = 1; i < c[0]; i ++) {
			int lca = LCA(c[i], c[i + 1]);
			d[lca].emplace_back(c[i + 1], dep_v[c[i + 1]] - dep_v[lca] - (c[i + 1] <= n));
			d[c[i + 1]].emplace_back(lca, dep_v[c[i + 1]] - dep_v[lca] - (c[i +1 ] <= n));
			tot -= dep_v[c[i + 1]] - dep_v[lca] - (c[i + 1] <= n);
		}
		dfs2(c[1], 0);
		ans += (long long)tot * c0;
		cout << ans << endl;
		
		for(int i=1;i<=c[0];i++){
			d[c[i]].clear();
			siz[c[i]]=0;
		}
	}
    return 0;
}
/*

9 8 1
1 2
2 3
3 4
1 5
5 6
6 7
5 8
8 9
3 3 4 7

7 9 5
1 2
1 3
2 3
1 4
1 5
4 5
1 6
1 7
6 7
3 2 3 4
3 2 3 4
3 2 4 6
4 2 3 4 5

17

*/

#include <bits/stdc++.h>
#define IOS ios::sync_with_stdio(0), cin.tie(0)
#define int long long
#define endl '\n'
#define lowbit(x) ((x) & (-x))
#define all(s) s.begin(), s.end()
#define pii pair<int, int>
#define ls(x) (x << 1)
#define rs(x) (x << 1 | 1)
using namespace std;

template <class T>
inline void read(T &x)
{
    x = 0;
    char c = getchar();
    bool f = 0;
    for (; !isdigit(c); c = getchar())
        f ^= (c == '-');
    for (; isdigit(c); c = getchar())
        x = (x << 3) + (x << 1) + (c ^ 48);
    x = f ? -x : x;
}

template <class T>
inline void write(T x)
{
    if (x < 0)
        putchar('-'), x = -x;
    if (x < 10)
        putchar(x + 48);
    else
        write(x / 10), putchar(x % 10 + 48);
}

#define INF 0x3f3f3f3f3f3f3f3f
const int kN = 2e6 + 10;
const int kInf = 1e9 + 2077;

int n, a[kN], b[kN], aa[kN], bb[kN];
int sumb[kN], ans_val[kN];
int rt, son[kN][2], top, st[kN], fa[kN];

void build_cartesian() {
    st[top = 0] = 0;
    for (int i = 1; i <= n; ++i) {
        while (top && aa[st[top]] < aa[i]) --top;
        son[i][0] = son[st[top]][1];
        son[st[top]][1] = i;
        st[++top] = i;
    }
    rt = st[1];
}

void dfs1(int u, int father) {
    sumb[u] = bb[u];
    fa[u] = father;
    if (son[u][0]) {
        dfs1(son[u][0], u);
        sumb[u] += sumb[son[u][0]];
    }
    if (son[u][1]) {
        dfs1(son[u][1], u);
        sumb[u] += sumb[son[u][1]];
    }
}

void dfs2(int u, int father, int max_demand) {
    ans_val[u] = max(aa[u], max_demand);
    
    if (son[u][0]) {
        dfs2(son[u][0], u, max(max_demand, aa[u] - sumb[son[u][0]]));
    }
    if (son[u][1]) {
        dfs2(son[u][1], u, max(max_demand, aa[u] - sumb[son[u][1]]));
    }
}

void solve() {
    read(n);
    a[0] = a[n + 1] = kInf;
    for (int i = 1; i <= n; ++i) read(a[i]);
    for (int i = 1; i <= n; ++i) read(b[i]);

    int q;
    read(q);
    while (q--) {
        int k;
        read(k);
        for (int i = 0; i <= n + 1; ++i) {
            aa[i] = a[i];
            bb[i] = b[i];
            sumb[i] = 0;
            son[i][0] = son[i][1] = 0;
            fa[i] = 0;
            ans_val[i] = 0;
        }
        
        while (k--) {
            int p, x, y;
            read(p); read(x); read(y);
            aa[p] = x;
            bb[p] = y;
        }
        
        build_cartesian();
        dfs1(rt, 0);
        dfs2(rt, 0, 0);

        int result = 0;
        for (int i = 2; i <= n; ++i) {
            int p = (aa[i - 1] < aa[i] ? i - 1 : i);
            result ^= ans_val[p];
        }
        write(result);
        putchar('\n');
    }
}

signed main() {
    IOS;
    solve();
    return 0;
}


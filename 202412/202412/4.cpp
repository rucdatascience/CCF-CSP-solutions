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
inline void write(T &x)
{
    if (x < 0)
        putchar('-'), x = -x;
    if (x < 10)
        putchar(x + 48);
    else
        write(x / 10), putchar(x % 10 + 48);
}
#define INF 0x3f3f3f3f3f3f3f3f
const int N = 1e5 + 10;
vector<pair<int, int>> g[N << 2];
int n;
int cnt;
int a[100005] = {0};
int k[100005] = {0};
template <typename T>
struct SegmentTree
{
    struct TreeNode
    {
        int l, r, lson, rson;
    } tr[N << 2];
    void build(int &s, int l, int r, bool io)
    {
        s = ++cnt;
        tr[s].l = l, tr[s].r = r;
        if (l == r)
        {
            if (!io)
                g[l].emplace_back(cnt, 0);
            else
                g[cnt].emplace_back(l - a[l], 0);
            return;
        }
        int mid = (l + r) >> 1;
        if (l <= mid)
            build(tr[s].lson, l, mid, io);
        if (mid < r)
            build(tr[s].rson, mid + 1, r, io);
        if (!io)
            g[tr[s].lson].emplace_back(s, 0), g[tr[s].rson].emplace_back(s, 0);
        else
            g[s].emplace_back(tr[s].lson, 0), g[s].emplace_back(tr[s].rson, 0);
    }
    vector<int> v[2];
    void query(int s, int l, int r, bool io)
    { // cout<<"s= "<<s<<"l="<<l<<"r="<<r<<endl;
        if (l <= tr[s].l && tr[s].r <= r)
            return v[io].emplace_back(s), void();
        int mid = (tr[s].l + tr[s].r) >> 1;
        if (l <= mid)
            query(tr[s].lson, l, r, io);
        if (mid < r)
            query(tr[s].rson, l, r, io);
    }
};
SegmentTree<int> T;
int dis[N << 2], ans[N];
bool vis[N << 2];
void dij()
{
    memset(dis, 0x3f, sizeof(dis));
    memset(vis, 0, sizeof(vis));
    dis[1] = 0;
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    pq.push({0, 1});
    while (!pq.empty())
    {
        int u = pq.top().second;
        pq.pop();
        if (vis[u])
            continue;
        vis[u] = true;
        // cout<<dis[u]<<" qq"<<endl;
        for (auto tem : g[u])
        {
            int v = tem.first;
            // cout<<v<<endl;

            int w = tem.second;
            if (!vis[v] && dis[v] > dis[u] + w)
            {
                dis[v] = dis[u] + w;
                pq.push({dis[v], v});
            }
        }
    }
}
void work()
{
    cin >> n;
    for (int i = 1; i <= n; i++)
    {
        cin >> a[i];

        //  g[i].push_back({i - a[i], 0});
    }
    int rto;
    cnt = n;
    T.build(rto, 1, n, true);
    for (int i = 1; i <= n; i++)
    { // cout<<rto<<endl;
        cin >> k[i];
        int ll = i + 1;
        int rr = min(i + k[i], n);
        T.v[1].clear();
        T.query(rto, ll, rr, true);
        for (auto v : T.v[1])
            g[i].emplace_back(v, 1);
    }
    dij();
    // for(int i=1;i<=4*n;i++){
    //     cout<<dis[i]<<" ";
    // }
    ///  cout<<endl;
    cout << (dis[n] < INF ? dis[n] : -1) << endl;
}
signed main()
{
    IOS;
    work();
}
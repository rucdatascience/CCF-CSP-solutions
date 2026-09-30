#include <bits/stdc++.h>
using namespace std;

static int W;
vector<int> ch0, ch1, cnt, fv, vv;
vector<int> freelist;

int newnode() {
    if (!freelist.empty()) {
        int x = freelist.back(); freelist.pop_back();
        ch0[x] = ch1[x] = cnt[x] = fv[x] = vv[x] = 0;
        return x;
    }
    ch0.push_back(0); ch1.push_back(0); cnt.push_back(0); fv.push_back(0); vv.push_back(0);
    return (int)ch0.size() - 1;
}

void insert(int root, int x) {
    static int px[31], py[31];
    int y = x ^ W;
    px[30] = py[30] = root;
    int cx = root, cy = root;
    for (int r = 30; r >= 1; --r) {
        int bx = (x >> (r - 1)) & 1;
        int &c = bx ? ch1[cx] : ch0[cx];
        if (!c) c = newnode();
        cx = c;
        px[r - 1] = cx;
        if (cy) {
            int by = (y >> (r - 1)) & 1;
            cy = by ? ch1[cy] : ch0[cy];
        }
        py[r - 1] = cy;
    }
    for (int r = 0; r <= 30; ++r) cnt[px[r]]++;
    for (int r = 0; r <= 30; ++r) {
        int A = px[r], B = py[r];
        int val;
        if (!B) val = 0;
        else if (r == 0) val = min(cnt[A], cnt[B]);
        else {
            int wbit = (W >> (r - 1)) & 1;
            if (wbit) {
                val = (ch0[A] ? vv[ch0[A]] : 0) + (ch1[A] ? vv[ch1[A]] : 0);
            } else {
                int a0 = ch0[A] ? cnt[ch0[A]] : 0;
                int a1 = ch1[A] ? cnt[ch1[A]] : 0;
                int b0 = ch0[B] ? cnt[ch0[B]] : 0;
                int b1 = ch1[B] ? cnt[ch1[B]] : 0;
                val = min({cnt[A], cnt[B],
                           a0 + b0 + (ch1[A] ? vv[ch1[A]] : 0),
                           a1 + b1 + (ch0[A] ? vv[ch0[A]] : 0)});
            }
        }
        vv[A] = val;
        if (B) vv[B] = val;

        if (r == 0) {
            fv[A] = cnt[A] ? 1 : 0;
        } else {
            int wbit = (W >> (r - 1)) & 1;
            if (wbit) {
                fv[A] = (ch0[A] ? cnt[ch0[A]] : 0) + (ch1[A] ? cnt[ch1[A]] : 0)
                      - (ch0[A] ? vv[ch0[A]] : 0);
            } else {
                fv[A] = max(ch0[A] ? fv[ch0[A]] : 0, ch1[A] ? fv[ch1[A]] : 0);
            }
        }
    }
}

void gather(int node, int r, int val, vector<int>& out) {
    if (!node) return;
    if (r == 0) {
        for (int i = 0; i < cnt[node]; ++i) out.push_back(val);
        return;
    }
    gather(ch0[node], r - 1, val, out);
    gather(ch1[node], r - 1, val | (1 << (r - 1)), out);
}

void freeSubtree(int node) {
    if (!node) return;
    freeSubtree(ch0[node]);
    freeSubtree(ch1[node]);
    freelist.push_back(node);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    cin >> n >> W;
    vector<int> root(n + 1), sz(n + 1, 0);
    long long globalSum = 0;
    ch0.reserve(30000005); ch1.reserve(30000005); cnt.reserve(30000005);
    fv.reserve(30000005); vv.reserve(30000005); freelist.reserve(30000005);
    ch0.push_back(0); ch1.push_back(0); cnt.push_back(0); fv.push_back(0); vv.push_back(0);
    for (int i = 1; i <= n; ++i) {
        root[i] = newnode();
        int m; cin >> m;
        sz[i] = m;
        for (int j = 0; j < m; ++j) {
            int x; cin >> x;
            insert(root[i], x);
        }
        globalSum += fv[root[i]];
    }
    int q; cin >> q;
    while (q--) {
        int op; cin >> op;
        if (op == 1) {
            int u, x; cin >> u >> x;
            globalSum -= fv[root[u]];
            insert(root[u], x);
            sz[u]++;
            globalSum += fv[root[u]];
        } else if (op == 2) {
            int u, v; cin >> u >> v;
            if (sz[u] < sz[v]) {
                swap(root[u], root[v]);
                swap(sz[u], sz[v]);
            }
            globalSum -= (long long)fv[root[u]] + fv[root[v]];
            vector<int> vals;
            gather(root[v], 30, 0, vals);
            for (int x : vals) insert(root[u], x);
            freeSubtree(root[v]);
            root[v] = 0;
            sz[u] += sz[v];
            globalSum += fv[root[u]];
        } else {
            cout << globalSum << '\n';
        }
    }
    return 0;
}

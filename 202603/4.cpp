#include <iostream>
#include <vector>

using namespace std;

typedef long long LL;

LL K;
const int D = 13;

LL g_func(LL x) {
    x %= K;
    return (x * (x + 1) / 2) % K;
}

struct Node {
    LL cnt;
    LL S[13];
    LL S0_d[13];
    LL S_g;
    LL lazy[13];
};

vector<Node> tree;

void apply_lazy(int node, const LL lazy_val[13]) {
    LL d0 = lazy_val[0];
    LL cnt = tree[node].cnt;
    LL old_S0 = tree[node].S[0];

    for (int d = 1; d < D; ++d) {
        LL dd = lazy_val[d];
        LL term1 = tree[node].S0_d[d];
        LL term2 = (d0 % K) * (tree[node].S[d] % K) % K;
        LL term3 = (dd % K) * (old_S0 % K) % K;
        LL term4 = (cnt % K) * ((d0 % K) * (dd % K) % K) % K;
        tree[node].S0_d[d] = (term1 + term2 + term3 + term4) % K;
        tree[node].S[d] = (tree[node].S[d] + (cnt % K) * (dd % K)) % K;
    }

    LL old_Sg = tree[node].S_g;
    LL term_g1 = old_Sg;
    LL term_g2 = (d0 % K) * (old_S0 % K) % K;
    LL term_g3 = (cnt % K) * g_func(d0) % K;
    tree[node].S_g = (term_g1 + term_g2 + term_g3) % K;

    tree[node].S[0] = (old_S0 + (cnt % K) * (d0 % K)) % K;

    for (int d = 0; d < D; ++d) {
        tree[node].lazy[d] = (tree[node].lazy[d] + lazy_val[d]) % K;
    }
}

void push_down(int node) {
    bool has_lazy = false;
    for (int d = 0; d < D; ++d) {
        if (tree[node].lazy[d] != 0) {
            has_lazy = true;
            break;
        }
    }
    if (!has_lazy) return;

    apply_lazy(2 * node, tree[node].lazy);
    apply_lazy(2 * node + 1, tree[node].lazy);

    for (int d = 0; d < D; ++d) tree[node].lazy[d] = 0;
}

void push_up(int node) {
    int lc = 2 * node, rc = 2 * node + 1;
    tree[node].cnt = tree[lc].cnt + tree[rc].cnt;
    tree[node].S_g = (tree[lc].S_g + tree[rc].S_g) % K;
    for (int d = 0; d < D; ++d) {
        tree[node].S[d] = (tree[lc].S[d] + tree[rc].S[d]) % K;
        if (d >= 1) tree[node].S0_d[d] = (tree[lc].S0_d[d] + tree[rc].S0_d[d]) % K;
    }
}

void build(int node, int l, int r, const vector<LL>& a) {
    if (l == r) {
        tree[node].cnt = 1;
        LL temp = a[l];
        LL digits[13] = {0};
        for (int d = 0; d < D; ++d) {
            digits[d] = temp % K;
            temp /= K;
        }
        for (int d = 0; d < D; ++d) {
            tree[node].S[d] = digits[d];
            if (d >= 1) tree[node].S0_d[d] = (digits[0] * digits[d]) % K;
        }
        tree[node].S_g = g_func(digits[0]);
        for (int d = 0; d < D; ++d) tree[node].lazy[d] = 0;
        return;
    }
    int mid = (l + r) / 2;
    build(2 * node, l, mid, a);
    build(2 * node + 1, mid + 1, r, a);
    push_up(node);
}

void update(int node, int l, int r, int ql, int qr, const LL val_digits[13]) {
    if (ql <= l && r <= qr) {
        apply_lazy(node, val_digits);
        return;
    }
    push_down(node);
    int mid = (l + r) / 2;
    if (ql <= mid) update(2 * node, l, mid, ql, qr, val_digits);
    if (qr > mid) update(2 * node + 1, mid + 1, r, ql, qr, val_digits);
    push_up(node);
}

struct QueryResult {
    LL S[13], S0_d[13], S_g;
};

QueryResult query(int node, int l, int r, int ql, int qr) {
    if (ql <= l && r <= qr) {
        QueryResult res;
        res.S_g = tree[node].S_g;
        for (int d = 0; d < D; ++d) {
            res.S[d] = tree[node].S[d];
            res.S0_d[d] = tree[node].S0_d[d];
        }
        return res;
    }
    push_down(node);
    int mid = (l + r) / 2;
    if (qr <= mid) return query(2 * node, l, mid, ql, qr);
    if (ql > mid) return query(2 * node + 1, mid + 1, r, ql, qr);

    QueryResult left_res = query(2 * node, l, mid, ql, qr);
    QueryResult right_res = query(2 * node + 1, mid + 1, r, ql, qr);

    QueryResult res;
    res.S_g = (left_res.S_g + right_res.S_g) % K;
    for (int d = 0; d < D; ++d) {
        res.S[d] = (left_res.S[d] + right_res.S[d]) % K;
        if (d >= 1) res.S0_d[d] = (left_res.S0_d[d] + right_res.S0_d[d]) % K;
    }
    return res;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    if (!(cin >> n >> m >> K)) return 0;

    vector<LL> a(n + 1);
    for (int i = 1; i <= n; ++i) cin >> a[i];

    tree.resize(4 * n + 5);
    build(1, 1, n, a);

    for (int i = 0; i < m; ++i) {
        int t;
        cin >> t;
        if (t == 1) {
            int l, r; LL v;
            cin >> l >> r >> v;
            LL v_digits[13] = {0};
            LL temp = v;
            for (int d = 0; d < D; ++d) {
                v_digits[d] = temp % K;
                temp /= K;
            }
            update(1, 1, n, l, r, v_digits);
        } else {
            int l, r;
            cin >> l >> r;
            QueryResult res = query(1, 1, n, l, r);
            LL ans = 0, cur_pow = 1;
            
            LL ans_digits[13] = {0};
            ans_digits[0] = res.S_g % K;
            for (int d = 1; d < D; ++d) ans_digits[d] = (res.S0_d[d] + res.S[d]) % K;

            for (int d = 0; d < D; ++d) {
                ans += ans_digits[d] * cur_pow;
                cur_pow *= K;
            }
            cout << ans << "\n";
        }
    }
    return 0;
}

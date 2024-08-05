#include <bits/stdc++.h>
using namespace std;
const long long P1 = 998244353, P2 = 1e9 + 7, P3 = 1e9 + 9, P4 = 1e9 + 21;
int n, t, a[100005];
long long hashQ(deque<int>& Q, const long long p = P1) {
    long long ret = 0;
    for (size_t i = 0; i < Q.size(); i++)
        ret = ret * p + Q[i];
    return ret;
}
using hash_t = tuple<long long, long long, long long>;
hash_t mergeHashQ(deque<int>& Q) {
    return make_tuple(hashQ(Q, P1), hashQ(Q, P2), hashQ(Q, P3));
}
int f = 0;
map<hash_t, int> mp;
int DFS(deque<int> Q, int op) {
    auto h = mergeHashQ(Q);
    if (mp.count(h))
        return mp[h];
    if (Q.size() == 1) {
        mp[h] = Q.front();
        return Q.front();
    }
    int ret;
    if (op) {  // max
        ret = 0;
        for (size_t i = 0; i < Q.size() - 1; i++) {
            deque<int> newQ;
            for (size_t j = 0; j < i; j++)
                newQ.push_back(Q[j]);
            newQ.push_back(Q[i] + Q[i + 1]);
            for (size_t j = i + 2; j < Q.size(); j++)
                newQ.push_back(Q[j]);
            ret = max(DFS(newQ, op ^ 1), ret);
        }
        {
            deque<int> newQ(Q);
            newQ.pop_front();
            ret = max(ret, DFS(newQ, op ^ 1));
        }
        {
            deque<int> newQ(Q);
            newQ.pop_back();
            ret = max(ret, DFS(newQ, op ^ 1));
        }
    } else {  // min
        ret = 1e9 + 7;
        for (size_t i = 0; i < Q.size() - 1; i++) {
            deque<int> newQ;
            for (size_t j = 0; j < i; j++)
                newQ.push_back(Q[j]);
            newQ.push_back(Q[i] + Q[i + 1]);
            for (size_t j = i + 2; j < Q.size(); j++)
                newQ.push_back(Q[j]);
            ret = min(DFS(newQ, op ^ 1), ret);
        }
        {
            deque<int> newQ(Q);
            newQ.pop_front();
            ret = min(ret, DFS(newQ, op ^ 1));
        }
        {
            deque<int> newQ(Q);
            newQ.pop_back();
            ret = min(ret, DFS(newQ, op ^ 1));
        }
    }
    mp[h] = ret;
    return ret;
}
int main() {
    deque<int> initQ;
    scanf("%d%d", &n, &t);
    for (int i = 1; i <= n; i++) {
        scanf("%d", &a[i]);
        initQ.push_back(a[i]);
    }
    if (n <= 20) {
        printf("%d", DFS(initQ, t));
    } else {
        if (n % 2 == 0) {
            if (t) {
                printf("%lld", 1ll * (n / 2 + 1) * a[1]);
            } else {
                printf("%lld", 1ll * a[1]);
            }
        } else {
            if (t) {
                printf("%lld", 1ll * a[1]);
            } else {
                printf("%lld", 1ll * (n + 1) / 2 * a[1]);
            }
        }
    }
}
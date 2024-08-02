#include <bits/stdc++.h>
using namespace std;
const int N = 1e7 + 5, mod = 1e9 + 7;
int arr[N];
void add(int x, int w) {
    if (x < 0)
        return;
    for (; x; x -= x & -x)
        arr[x] += w;
}
int query(int x) {
    int ret = 0;
    for (; x < N; x += x & -x)
        ret += arr[x];
    return ret;
}
int n, m, k, T;
int main() {
    scanf("%d%d%d", &n, &m, &k);
    for (int i = 1; i <= n; i++) {
        int t, c;
        scanf("%d%d", &t, &c);
        add(t, 1);
        add(t - c, -1);
    }
    for (int i = 1; i <= m; i++) {
        int q;
        scanf("%d", &q);
        printf("%d\n", query(q + k));
    }
}
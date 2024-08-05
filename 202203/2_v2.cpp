#include <bits/stdc++.h>
using namespace std;
const int N = 1e7 + 5, mod = 1e9 + 7;
int add[N], sub[N];
int n, m, k, T;
int how_many_equal_or_large(int* arr, int x) {
    int first_one_equal_or_large =
        lower_bound(arr + 1, arr + n + 1, x) - arr - 1;
    return n - first_one_equal_or_large;
}
int main() {
    scanf("%d%d%d", &n, &m, &k);
    for (int i = 1; i <= n; i++) {
        int t, c;
        scanf("%d%d", &t, &c);
        add[i] = t;
        sub[i] = t - c;
    }
    sort(add + 1, add + n + 1);
    sort(sub + 1, sub + n + 1);
    for (int i = 1; i <= m; i++) {
        int q;
        scanf("%d", &q);
        int ans = how_many_equal_or_large(add, q + k) -
                  how_many_equal_or_large(sub, q + k);
        printf("%d\n", ans);
    }
}
#include <bits/stdc++.h>
using namespace std;
int n, t, a[100005];
int get_ans(int state) {
    int sum = 0;
    int bit = 1;
    bool start = false;
    for (int i = 0; i <= n; i++, bit <<= 1) {
        if (start) {
            sum += a[i];
        }
        if (!(state & bit)) {
            if (!start) {  // 第一个 0
                start = true;
            } else {
                break;  // 第二个 0
            }
        }
    }
    return sum;
}
int f[1 << 21];
int DFS(int state, int depth, int op) {  // op = 1, large, op = 0 small
    if (f[state]) {
        return f[state];
    }
    if (depth == n - 1) {
        return f[state] = get_ans(state);
    }
    int bit = 1;
    int ret = op ? 0 : numeric_limits<int>::max();
    for (int i = 0; i <= n; i++, bit <<= 1) {
        if (state & bit) {
            continue;
        }
        int new_state = state | bit;
        if (op) {
            ret = max(ret, DFS(new_state, depth + 1, op ^ 1));
        } else {
            ret = min(ret, DFS(new_state, depth + 1, op ^ 1));
        }
    }
    return f[state] = ret;
}
int main() {
    scanf("%d%d", &n, &t);
    for (int i = 1; i <= n; i++) {
        scanf("%d", &a[i]);
    }
    if (n <= 20) {
        printf("%d\n", DFS(0, 0, t));
    } else {
        if (n % 2 == 0) {
            if (t) {
                printf("%d\n", (n / 2 + 1) * a[1]);
            } else {
                printf("%d\n", a[1]);
            }
        } else {
            if (t) {
                printf("%d\n", a[1]);
            } else {
                printf("%d\n", (n + 1) / 2 * a[1]);
            }
        }
    }
}
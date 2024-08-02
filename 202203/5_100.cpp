#include <bits/stdc++.h>
using namespace std;

int a[100010], n, k;

int f1(int l, int r) {         // 小z先手，且石子有偶数堆
    int m = (r - l), tot = 0;  // m%2=0
    vector<int> b(m);
    for (int i = l; i < r; i++) {
        b[i - l] = a[i];
        tot += a[i];
    }

    int disc = 0, tmp = 0;
    for (int i = 0; i < m / 2 - 1; i++) {
        tmp += b[i];
    }
    disc = max(disc, tmp);
    for (int i = 0; i < m / 2 - 1; i++) {
        tmp += b[m - i - 1] - b[m / 2 - 2 - i];
        disc = max(disc, tmp);
    }
    return tot - disc;
}

// 是否能合并为(n/2+1)个大小至少为x的堆
bool check(int x, int l, int r) {
    int sum = 0, cnt = 0;
    for (int i = l; i < r; i++) {
        sum += a[i];
        if (sum >= x) {
            sum = 0;
            cnt++;
        }
    }
    return cnt > (r - l) / 2;
}

int f2(int l, int r) {  // 小z先手，且石子有奇数堆
    int ret = 1;
    int L = 1, R = 1e9;
    while (L <= R) {
        int mid = (L + R) / 2;
        if (check(mid, l, r)) {
            ret = mid;
            L = mid + 1;
        } else
            R = mid - 1;
    }
    return ret;
}

void solve() {
    cin >> n >> k;
    for (int i = 0; i < n; i++) {
        cin >> a[i];
    }

    if ((n % 2 == 0) && k) {
        cout << f1(0, n) << '\n';
    } else if ((n % 2 == 1) && (!k)) {
        cout << min(f1(1, n), f1(0, n - 1)) << '\n';
    } else if ((n % 2 == 1) && k) {
        cout << f2(0, n) << '\n';
    } else if ((n % 2 == 0) && (!k)) {
        cout << min(f2(1, n), f2(0, n - 1)) << '\n';
    }
}
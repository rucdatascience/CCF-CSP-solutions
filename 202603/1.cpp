#include <iostream>

using namespace std;

bool is_balanced(long long x) {
    if (x <= 0) return false;
    int ones = 0;
    int zeros = 0;
    while (x > 0) {
        if (x & 1) ones++;
        else zeros++;
        x >>= 1;
    }
    return ones == zeros;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    if (!(cin >> n)) return 0;

    int ans = 0;
    for (int i = 0; i < n; ++i) {
        long long a;
        cin >> a;
        if (is_balanced(a)) {
            ans++;
        }
    }

    cout << ans << "\n";
    return 0;
}

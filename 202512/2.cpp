#include <iostream>
#include <vector>

using namespace std;

// 计算 f(x, k)
inline int f_func(int x, int k) {
    return (((x * x + k * k) % 8) ^ k);
}

// 逆变换 g_inv
inline int g_inv(int y, int k) {
    int a_prime = (y >> 6) & 7;
    int b_prime = (y >> 3) & 7;
    int c_prime = y & 7;

    int b = a_prime;
    int c = b_prime ^ f_func(b, k);
    int a = c_prime ^ f_func(c, k);

    return (a << 6) | (b << 3) | c;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    if (!(cin >> n >> m)) return 0;

    vector<int> k(m);
    for (int i = 0; i < m; ++i) {
        cin >> k[i];
    }

    // 预处理映射表 mapping[y] = x
    vector<int> mapping(512);
    for (int y = 0; y < 512; ++y) {
        int cur = y;
        for (int i = m - 1; i >= 0; --i) {
            cur = g_inv(cur, k[i]);
        }
        mapping[y] = cur;
    }

    // 针对输入直接 O(1) 输出
    for (int i = 0; i < n; ++i) {
        int a_val;
        cin >> a_val;
        cout << mapping[a_val] << (i == n - 1 ? "" : " ");
    }
    cout << "\n";

    return 0;
}

#include <iostream>
#include <vector>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    if (!(cin >> n >> m)) return 0;

    vector<unsigned int> a(n + 1);
    for (int i = 1; i <= n; ++i) {
        cin >> a[i];
    }

    vector<vector<int>> S(m), T(m);
    for (int i = 0; i < m; ++i) {
        int sz;
        cin >> sz;
        S[i].resize(sz);
        for (int j = 0; j < sz; ++j) {
            cin >> S[i][j];
        }
    }
    for (int i = 0; i < m; ++i) {
        int sz;
        cin >> sz;
        T[i].resize(sz);
        for (int j = 0; j < sz; ++j) {
            cin >> T[i][j];
        }
    }

    for (int i = 0; i < m; ++i) {
        // 1. 判断真实集合是否相等
        bool true_equal = true;
        if (S[i].size() != T[i].size()) {
            true_equal = false;
        } else {
            for (size_t j = 0; j < S[i].size(); ++j) {
                if (S[i][j] != T[i][j]) {
                    true_equal = false;
                    break;
                }
            }
        }

        // 2. 计算小 C 的异或和
        unsigned int f_S = 0, f_T = 0;
        for (int x : S[i]) f_S ^= a[x];
        for (int y : T[i]) f_T ^= a[y];
        bool c_equal = (f_S == f_T);

        // 3. 比较逻辑是否吻合
        if (true_equal == c_equal) {
            cout << "correct\n";
        } else {
            cout << "wrong\n";
        }
    }

    return 0;
}

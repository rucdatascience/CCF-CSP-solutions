#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

struct Op {
    int op;
    int u, v, L, d, r; 
    int d_row, l, r_col, o;
};

// 将逻辑坐标 (r, c) 转换为物理数组的坐标
pair<int, int> get_phys(int r, int c, int dir, int Z) {
    if (dir == 0) return {r, c};
    if (dir == 1) return {Z - 1 - c, r};
    if (dir == 2) return {Z - 1 - r, Z - 1 - c};
    return {c, Z - 1 - r};
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int Z;
    if (!(cin >> Z)) return 0;

    vector<string> phys(Z);
    for (int i = 0; i < Z; ++i) {
        cin >> phys[i];
    }

    int k_len;
    cin >> k_len;
    vector<int> K(k_len);
    for (int i = 0; i < k_len; ++i) {
        cin >> K[i];
    }

    int t = K[0];
    vector<Op> ops;
    int idx = 1;
    for (int i = 0; i < t; ++i) {
        Op op;
        op.op = K[idx++];
        if (op.op == 1) {
            op.u = K[idx++] - 1; op.v = K[idx++] - 1;
            op.L = K[idx++]; op.d = K[idx++]; op.r = K[idx++];
        } else {
            op.u = K[idx++] - 1; op.d_row = K[idx++] - 1;
            op.l = K[idx++] - 1; op.r_col = K[idx++] - 1;
            op.o = K[idx++];
        }
        ops.push_back(op);
    }

    int dir = 0; // 当前逻辑矩阵顺时针旋转的次数

    for (int i = t - 1; i >= 0; --i) {
        const auto& op = ops[i];
        if (op.op == 1) {
            dir = (dir + op.r) % 4; // 逆向全局旋转：顺时针旋转 r_i 次
            int times = op.d / 90;
            for (int step = 0; step < times; ++step) { // 逆向局部旋转：逆时针旋转
                vector<vector<char>> sub(op.L, vector<char>(op.L));
                for (int r = 0; r < op.L; ++r) {
                    for (int c = 0; c < op.L; ++c) {
                        auto [pr, pc] = get_phys(op.u + r, op.v + c, dir, Z);
                        sub[r][c] = phys[pr][pc];
                    }
                }
                for (int r = 0; r < op.L; ++r) {
                    for (int c = 0; c < op.L; ++c) {
                        auto [pr, pc] = get_phys(op.u + op.L - 1 - c, op.v + r, dir, Z);
                        phys[pr][pc] = sub[r][c];
                    }
                }
            }
        } else {
            int h = op.d_row - op.u + 1;
            int w = op.r_col - op.l + 1;
            vector<vector<char>> sub(h, vector<char>(w));
            for (int r = 0; r < h; ++r) {
                for (int c = 0; c < w; ++c) {
                    auto [pr, pc] = get_phys(op.u + r, op.l + c, dir, Z);
                    sub[r][c] = phys[pr][pc];
                }
            }
            if (op.o == 1) { // 上下翻转
                for (int r = 0; r < h; ++r) {
                    for (int c = 0; c < w; ++c) {
                        auto [pr, pc] = get_phys(op.u + h - 1 - r, op.l + c, dir, Z);
                        phys[pr][pc] = sub[r][c];
                    }
                }
            } else { // 左右翻转
                for (int r = 0; r < h; ++r) {
                    for (int c = 0; c < w; ++c) {
                        auto [pr, pc] = get_phys(op.u + r, op.l + w - 1 - c, dir, Z);
                        phys[pr][pc] = sub[r][c];
                    }
                }
            }
        }
    }

    int max_r = 0, max_c = 0;
    vector<string> orig(Z, string(Z, ' '));
    for (int r = 0; r < Z; ++r) {
        for (int c = 0; c < Z; ++c) {
            auto [pr, pc] = get_phys(r, c, dir, Z);
            orig[r][c] = phys[pr][pc];
            if (orig[r][c] != '?') {
                max_r = max(max_r, r + 1);
                max_c = max(max_c, c + 1);
            }
        }
    }

    cout << max_r << " " << max_c << "\n";
    for (int r = 0; r < max_r; ++r) {
        cout << orig[r].substr(0, max_c) << "\n";
    }

    return 0;
}

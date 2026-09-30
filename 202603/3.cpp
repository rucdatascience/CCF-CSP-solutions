#include <iostream>
#include <vector>
#include <set>
#include <string>

using namespace std;

typedef long long LL;
const LL INF = 4e18;

struct IntervalByPos {
    LL l, r;
    bool operator<(const IntervalByPos& other) const {
        return l < other.l;
    }
};

struct IntervalByLen {
    LL len, l, r;
    bool operator<(const IntervalByLen& other) const {
        if (len != other.len) return len < other.len;
        return l < other.l;
    }
};

set<IntervalByPos> free_pos;
set<IntervalByLen> free_len;

void insert_free(LL l, LL r) {
    if (l > r) return;
    free_pos.insert({l, r});
    free_len.insert({r - l + 1, l, r});
}

void erase_free(LL l, LL r) {
    free_pos.erase({l, r});
    free_len.erase({r - l + 1, l, r});
}

struct Interface {
    LL a, b;
    LL cur_pos;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    if (!(cin >> n >> q)) return 0;

    // 初始状态全局内存为 [0, INF]
    insert_free(0, INF);

    vector<vector<Interface>> proc(n + 1);

    for (int k = 0; k < q; ++k) {
        string op;
        cin >> op;
        if (op == "new") {
            int p;
            LL L;
            cin >> p >> L;

            // 寻找 len >= L 且 len 最小、起点最小的空闲块
            auto it = free_len.lower_bound({L, -1, -1});
            LL chosen_l = it->l;
            LL chosen_r = it->r;

            erase_free(chosen_l, chosen_r);

            LL alloc_a = chosen_l;
            LL alloc_b = chosen_l + L - 1;

            if (alloc_b < chosen_r) {
                insert_free(alloc_b + 1, chosen_r);
            }

            proc[p].push_back({alloc_a, alloc_b, -1});
            cout << alloc_a << "\n";

        } else if (op == "send") {
            int p;
            cin >> p;
            LL sum_pos = 0;
            for (auto& itf : proc[p]) {
                if (itf.cur_pos == -1) {
                    itf.cur_pos = itf.a;
                } else {
                    if (itf.cur_pos < itf.b) {
                        itf.cur_pos++;
                    } else {
                        itf.cur_pos = itf.a;
                    }
                }
                sum_pos += itf.cur_pos;
            }
            cout << sum_pos << "\n";

        } else if (op == "delete") {
            int p, idx;
            cin >> p >> idx;
            idx--; 

            LL free_a = proc[p][idx].a;
            LL free_b = proc[p][idx].b;

            proc[p].erase(proc[p].begin() + idx);

            LL new_l = free_a;
            LL new_r = free_b;

            // 尝试与右侧相邻空闲块合并
            auto it_right = free_pos.lower_bound({free_b + 1, free_b + 1});
            if (it_right != free_pos.end() && it_right->l == free_b + 1) {
                new_r = it_right->r;
                erase_free(it_right->l, it_right->r);
            }

            // 尝试与左侧相邻空闲块合并
            auto it_left = free_pos.lower_bound({free_a, free_a});
            if (it_left != free_pos.begin()) {
                --it_left;
                if (it_left->r == free_a - 1) {
                    new_l = it_left->l;
                    erase_free(it_left->l, it_left->r);
                }
            }

            insert_free(new_l, new_r);
        }
    }

    return 0;
}

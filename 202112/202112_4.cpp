/*
202112-4 磁盘文件操作
得分：100

https://blog.csdn.net/Crispo_W/article/details/123512092
*/

#include <bits/stdc++.h>
using namespace std;

#define FREE 0
#define OCCUPIED 1
#define ZOMBIE 2

#define N_MAX 200005
#define MIXED 2000000020
#define FULL (-2000000020)

struct FILE_BLOCK {
    int l, r;
    int x;  // value: -1e9~1e9, 2e9: mixed
    int id; // id: 1~n(<2e5), 2e9: mixed
    int state;  // FREE, OCCUPIED, ZOMBIE, 2e9: mixed
};
struct OPRT {
    OPRT(int t, int i, int ll, int rr, int xx, int pp) : type(t), id(i), l(ll), r(rr), x(xx), p(pp) {}
    int type, id, l, r, x, p;
};

int n, m, k;
vector<FILE_BLOCK> fb(N_MAX << 4);
vector<OPRT> oprt;
vector<int> coordinates;

int discretization() {
    coordinates.push_back(0);   // let coordinates begin with 1
    sort(coordinates.begin(), coordinates.end());
    m = unique(coordinates.begin(), coordinates.end()) - coordinates.begin();   // 1~m
    coordinates.resize(m);
    for (auto& op : oprt) {
        switch (op.type) {
        case 0: case 1: case 2:
            op.l = lower_bound(coordinates.begin(), coordinates.end(), op.l) - coordinates.begin();
            op.r = lower_bound(coordinates.begin(), coordinates.end(), op.r) - coordinates.begin();
            break;
        case 3: default:
            op.p = lower_bound(coordinates.begin(), coordinates.end(), op.p) - coordinates.begin();
            break;
        }
    }
    return 0;
}

void pushup(int curr) {
    if (fb[curr << 1].id == fb[curr << 1 | 1].id) fb[curr].id = fb[curr << 1].id;
    else fb[curr].id = MIXED;
    if (fb[curr << 1].x == fb[curr << 1 | 1].x) fb[curr].x = fb[curr << 1].x;
    else fb[curr].x = MIXED;
    if (fb[curr << 1].state == fb[curr << 1 | 1].state) fb[curr].state = fb[curr << 1].state;
    else fb[curr].state = MIXED;
}
/* f5 */
void build(int curr, int l, int r) {
    fb[curr].l = l;
    fb[curr].r = r;
    if (l == r) return;
    else {
        int mid = l + ((r - l) >> 1);     // 用减法不用加法，避免爆int
        build(curr << 1, l, mid);
        build((curr << 1) + 1, mid + 1, r);
        pushup(curr);
    }
}
void pushdown(int curr) {
    if (fb[curr].r != fb[curr].l) {
        if (fb[curr].id != MIXED) fb[curr << 1].id = fb[curr << 1 | 1].id = fb[curr].id;
        if (fb[curr].state != MIXED) fb[curr << 1].state = fb[curr << 1 | 1].state = fb[curr].state;
        if (fb[curr].x != MIXED) fb[curr << 1].x = fb[curr << 1 | 1].x = fb[curr].x;
    }
}
/* Interval change */
/* update_test: Find the rightmost available to_r */
int update_test(int curr, int to_l, int id) {
    if (fb[curr].state == FREE || fb[curr].state == ZOMBIE || fb[curr].id == id) return fb[curr].r;
    else if (fb[curr].state == OCCUPIED && fb[curr].id != MIXED) return FULL;   // debug: state = mixed, maybe itself
    else {  // state == mixed
        int mid = fb[curr].l + ((fb[curr].r - fb[curr].l) >> 1);
        pushdown(curr);
        if (to_l <= mid) {
            int left_r = update_test(curr << 1, to_l, id);
            if (left_r < mid) return left_r;
            int right_r = update_test(curr << 1 | 1, to_l, id);
            return right_r == FULL ? left_r : right_r;
        }
        else {
            return update_test(curr << 1 | 1, to_l, id);
        }
    }
};
void update_ic(int curr, int to_l, int to_r, int x, int id) {
    if (fb[curr].l == to_l && fb[curr].r == to_r) { // 刚好覆盖
        fb[curr].state = OCCUPIED;
        fb[curr].id = id;
        fb[curr].x = x;
        return;
    }
    int mid = fb[curr].l + ((fb[curr].r - fb[curr].l) >> 1);
    pushdown(curr);
    if (to_l <= mid) update_ic(curr << 1, to_l, min(to_r, mid), x, id);
    if (to_r > mid) update_ic(curr << 1 | 1, max(to_l, mid + 1), to_r, x, id);
    pushup(curr);
};
/* Interval deletion */
bool deletion_test(int curr, int to_l, int to_r, int id) {
    if (fb[curr].l == to_l && fb[curr].r == to_r) { // 只在完全匹配的时候作判断
        if (fb[curr].state == OCCUPIED && fb[curr].id == id) return true;
        else return false;
    }
    int mid = fb[curr].l + ((fb[curr].r - fb[curr].l) >> 1);
    pushdown(curr);
    bool avai = true;
    if (to_l <= mid) avai &= deletion_test(curr << 1, to_l, min(to_r, mid), id);
    if (to_r > mid) avai &= deletion_test(curr << 1 | 1, max(to_l, mid + 1), to_r, id);
    return avai;
}
void deletion(int curr, int to_l, int to_r, int id) {
    if (fb[curr].l == to_l && fb[curr].r == to_r) {
        fb[curr].state = ZOMBIE;
        return;
    }
    int mid = fb[curr].l + ((fb[curr].r - fb[curr].l) >> 1);
    pushdown(curr);
    if (to_l <= mid) deletion(curr << 1, to_l, min(to_r, mid), id);
    if (to_r > mid) deletion(curr << 1 | 1, max(to_l, mid + 1), to_r, id);
    pushup(curr);
}
/* Interval recovery */
bool recover_test(int curr, int to_l, int to_r, int id) {
    if (fb[curr].l == to_l && fb[curr].r == to_r) {
        if (fb[curr].state == ZOMBIE && fb[curr].id == id) return true;
        else return false;
    }
    int mid = fb[curr].l + ((fb[curr].r - fb[curr].l) >> 1);
    pushdown(curr);
    bool avai = true;
    if (to_l <= mid) avai &= recover_test(curr << 1, to_l, min(to_r, mid), id);
    if (to_r > mid) avai &= recover_test(curr << 1 | 1, max(to_l, mid + 1), to_r, id);
    return avai;
}
void recover(int curr, int to_l, int to_r, int id) {
    if (fb[curr].l == to_l && fb[curr].r == to_r) {
        fb[curr].state = OCCUPIED;
        return;
    }
    int mid = fb[curr].l + ((fb[curr].r - fb[curr].l) >> 1);
    pushdown(curr);
    if (to_l <= mid) recover(curr << 1, to_l, min(to_r, mid), id);
    if (to_r > mid) recover(curr << 1 | 1, max(to_l, mid + 1), to_r, id);
    pushup(curr);
}
/* query */
int query(int curr, int p) {
    if (fb[curr].l == fb[curr].r) return curr;
    else if (fb[curr].state != MIXED && fb[curr].id != MIXED && fb[curr].x != MIXED) return curr;
    else {
        int mid = fb[curr].l + ((fb[curr].r - fb[curr].l) >> 1);
        pushdown(curr);
        if (p <= mid) return query(curr << 1, p);
        else return query(curr << 1 | 1, p);
    }
}

int main() {
    scanf("%d%d%d", &n, &m, &k);
    int type = 0, id = 0, l = 0, r = 0, x = 0, p = 0;
    for (int i = 0; i < k; i++) {
        scanf("%d", &type);
        switch (type) {
        case 0:
            scanf("%d%d%d%d", &id, &l, &r, &x);
            oprt.emplace_back(type, id, l, r, x, 0);
            break;
        case 1:
            scanf("%d%d%d", &id, &l, &r);
            oprt.emplace_back(type, id, l, r, 0, 0);
            break;
        case 2:
            scanf("%d%d%d", &id, &l, &r);
            oprt.emplace_back(type, id, l, r, 0, 0);
            break;
        case 3:
            scanf("%d", &p);
            oprt.emplace_back(type, 0, 0, 0, 0, p);
            break;
        default:
            break;
        }
        if (type == 0 || type == 1 || type == 2) {
            coordinates.push_back(l);
            coordinates.push_back(r);
            if (l != 1) coordinates.push_back(l - 1);     // 注意离散化要加入前后的点
            if (r != m) coordinates.push_back(r + 1);
        }
        else {
            coordinates.push_back(p);
        }
    }
    /* 离散化 */
    discretization();
    /* 建树 */
    build(1, 1, m - 1);
    int query_index;
    for (auto& op : oprt) {
        switch (op.type) {
        case 0:
            op.r = min(op.r, update_test(1, op.l, op.id));
            if (op.r != FULL) update_ic(1, op.l, op.r, op.x, op.id);
            printf("%d\n", op.r == FULL ? -1 : coordinates[op.r]);
            break;
        case 1:
            if (deletion_test(1, op.l, op.r, op.id)) {
                deletion(1, op.l, op.r, op.id);
                printf("OK\n");
            }
            else {
                printf("FAIL\n");
            }
            break;
        case 2:
            if (recover_test(1, op.l, op.r, op.id)) {
                recover(1, op.l, op.r, op.id);
                printf("OK\n");
            }
            else printf("FAIL\n");
            break;
        case 3:
            query_index = query(1, op.p);
            if (fb[query_index].state != OCCUPIED) printf("%d %d\n", 0, 0);
            else printf("%d %d\n", fb[query_index].id, fb[query_index].x);
            break;
        default:
            break;
        }
    }
    return 0;
}

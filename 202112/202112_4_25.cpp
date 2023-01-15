/*
202112-4 磁盘文件操作
得分：25 (可能在跑不了的大规模测试集上是错误的)
*/

#include <bits/stdc++.h>
using namespace std;


struct segment {
    int l, r;
    mutable int value, id, previous_id; // id==0 means free, otherwise occupied
};
struct compare { // 函数对象
    bool operator()(const segment& a, const segment& b)const { // set要求此处为常类型
        return a.r < b.r; // l from small to large
    }
};
set<segment, compare> segment_tree;

void initialization(int m);
void write(int id, int l, int r, int x);
void del(int id, int l, int r);
void recover(int id, int l, int r);
void query(int p);

int main() {
    int n, m, k;
    cin >> n >> m >> k;

    initialization(m);

    int type, id, l, r, x, p;
    for (int i = 0; i < k; i++) {
        cin >> type;
        if (type == 0) {
            cin >> id >> l >> r >> x;
            write(id, l, r, x);
        }
        else if (type == 1) {
            cin >> id >> l >> r;
            del(id, l, r);
        }
        else if (type == 2) {
            cin >> id >> l >> r;
            recover(id, l, r);
        }
        else if (type == 3) {
            cin >> p;
            query(p);
        }
    }
    return 0;
}


void initialization(int m) {

    segment s;
    s.l = 1;
    s.r = m;
    s.value = 0;
    s.id = 0;
    s.previous_id = 0;

    segment_tree.insert(s);
}

void write(int id, int l, int r, int x) {

    /*check: [l,r] 在 [it_begin, it_end] 内部 */
    segment s;
    s.r = r;
    auto it_end = segment_tree.upper_bound(s); // 寻找r值大于r的第一个已有线段
    if (it_end == segment_tree.end()) {
        it_end--; // 若寻找到了线段树的end处，则it_begin指向最后一个线段
    }
    auto it_begin = it_end;
    for (;; it_begin--) { // 向right值更小的方向遍历线段
        if (it_begin->r < l) {
            break; // 该线段及right值更小的线段与[l,r]不重叠
        }
        if (it_begin == segment_tree.begin()) {
            break; // 遍历完线段
        }
    }

    /*write; 尝试从最左端开始向右顺次写入数据*/
    vector<segment> to_be_deleted, to_be_inserted;
    vector<pair<int, int>> to_write_segments;
    int write_right_bound = -1;
    for (auto it = it_begin;; it++) {
        if (it->r < l || it->l > r) { // 该线段与[l,r]不重叠
            if (it == it_end) {
                break; // 遍历完线段
            }
            continue;
        }
        if (it->id != 0 && it->id != id) {
            break; // 操作立刻中断
        }

        //if (it->l >= l && it->r <= r) {
        //    it->value = x;
        //    it->previous_id = it->id;
        //    it->id = id;
        //    write_right_bound = it->r;
        //    if (it == it_end) {
        //        break; // 遍历完线段
        //    }
        //    continue;
        //}

        to_be_deleted.push_back(*it); // 该线段与[l,r]（部分）重叠

        /*write 重叠的部分 { max(it->l,l), min(it->r,r) } */
        //s.l = max(it->l, l);
        //s.r = min(it->r, r);
        //s.value = x;
        //s.id = id;
        //s.previous_id = it->id;
        //to_be_inserted.push_back(s); // 写在重叠的部分
        write_right_bound = min(it->r, r);

        /*维护不重叠部分*/
        if (it->l < l) {
            s = *it;
            s.l = it->l;
            s.r = l - 1;
            to_be_inserted.push_back(s); // 将该线段与[l,r]不重叠的部分生成新线段
        }
        if (it->r > r) {
            s = *it;
            s.l = r + 1;
            s.r = it->r;
            to_be_inserted.push_back(s); // 将该线段与[l,r]不重叠的部分生成新线段
        }
        if (it == it_end) {
            break; // 遍历完线段
        }
    }
    for (auto it = to_be_deleted.begin(); it != to_be_deleted.end(); it++) {
        segment_tree.erase(*it);
    }
    for (auto it = to_be_inserted.begin(); it != to_be_inserted.end(); it++) {
        segment_tree.insert(*it);
    }

    if (write_right_bound != -1) {
        s.l = l;
        s.r = write_right_bound;
        s.id = id;
        s.value = x;
        s.previous_id = 0; // 此处无所谓
        segment_tree.insert(s);
    }

    cout << write_right_bound << endl;
}

void del(int id, int l, int r) {

    /*check: [l,r] 在 [it_begin, it_end] 内部 */
    segment s;
    s.r = r;
    auto it_end = segment_tree.upper_bound(s); // 寻找r值大于r的第一个已有线段
    if (it_end == segment_tree.end()) {
        it_end--; // 若寻找到了线段树的end处，则it_begin指向最后一个线段
    }
    auto it_begin = it_end;
    for (;; it_begin--) { // 向right值更小的方向遍历线段
        if (it_begin->r < l) {
            break; // 该线段及right值更小的线段与[l,r]不重叠
        }
        if (it_begin == segment_tree.begin()) {
            break; // 遍历完线段
        }
    }

    /*write; 尝试从最左端开始向右顺次写入数据*/
    vector<segment> to_be_deleted, to_be_inserted;
    for (auto it = it_begin;; it++) {
        if (it->r < l || it->l > r) { // 该线段与[l,r]不重叠
            if (it == it_end) {
                break; // 遍历完线段
            }
            continue;
        }
        if (it->id != id) {
            cout << "FAIL" << endl;
            return;
        }

        //if (it->l >= l && it->r <= r) {
        //    it->previous_id = it->id;
        //    it->id = 0;
        //    if (it == it_end) {
        //        break; // 遍历完线段
        //    }
        //    continue;
        //}

        to_be_deleted.push_back(*it); // 该线段与[l,r]（部分）重叠

        /*write 重叠的部分 { max(it->l,l), min(it->r,r) } */
        s = *it;
        s.l = max(it->l, l);
        s.r = min(it->r, r);
        s.id = 0;
        s.previous_id = it->id;
        to_be_inserted.push_back(s); // 写在重叠的部分

        /*维护不重叠部分*/
        if (it->l < l) {
            s = *it;
            s.l = it->l;
            s.r = l - 1;
            to_be_inserted.push_back(s); // 将该线段与[l,r]不重叠的部分生成新线段
        }
        if (it->r > r) {
            s = *it;
            s.l = r + 1;
            s.r = it->r;
            to_be_inserted.push_back(s); // 将该线段与[l,r]不重叠的部分生成新线段
        }
        if (it == it_end) {
            break; // 遍历完线段
        }
    }
    for (auto it = to_be_deleted.begin(); it != to_be_deleted.end(); it++) {
        segment_tree.erase(*it);
    }
    for (auto it = to_be_inserted.begin(); it != to_be_inserted.end(); it++) {
        segment_tree.insert(*it);
    }

    cout << "OK" << endl;
}

void recover(int id, int l, int r) {

    /*check: [l,r] 在 [it_begin, it_end] 内部 */
    segment s;
    s.r = r;
    auto it_end = segment_tree.upper_bound(s); // 寻找r值大于r的第一个已有线段
    if (it_end == segment_tree.end()) {
        it_end--; // 若寻找到了线段树的end处，则it_begin指向最后一个线段
    }
    auto it_begin = it_end;
    for (;; it_begin--) { // 向right值更小的方向遍历线段
        if (it_begin->r < l) {
            break; // 该线段及right值更小的线段与[l,r]不重叠
        }
        if (it_begin == segment_tree.begin()) {
            break; // 遍历完线段
        }
    }

    /*write; 尝试从最左端开始向右顺次写入数据*/
    vector<segment> to_be_deleted, to_be_inserted;
    for (auto it = it_begin;; it++) {
        if (it->r < l || it->l > r) { // 该线段与[l,r]不重叠
            if (it == it_end) {
                break; // 遍历完线段
            }
            continue;
        }
        if (it->id != 0 || it->previous_id != id) {
            cout << "FAIL" << endl;
            return;
        }

        //if (it->l >= l && it->r <= r) {
        //    it->previous_id = 0;
        //    it->id = id;
        //    if (it == it_end) {
        //        break; // 遍历完线段
        //    }
        //    continue;
        //}

        to_be_deleted.push_back(*it); // 该线段与[l,r]（部分）重叠

        /*write 重叠的部分 { max(it->l,l), min(it->r,r) } */
        s = *it;
        s.l = max(it->l, l);
        s.r = min(it->r, r);
        s.id = id;
        s.previous_id = 0;
        to_be_inserted.push_back(s); // 写在重叠的部分

        /*维护不重叠部分*/
        if (it->l < l) {
            s = *it;
            s.l = it->l;
            s.r = l - 1;
            to_be_inserted.push_back(s); // 将该线段与[l,r]不重叠的部分生成新线段
        }
        if (it->r > r) {
            s = *it;
            s.l = r + 1;
            s.r = it->r;
            to_be_inserted.push_back(s); // 将该线段与[l,r]不重叠的部分生成新线段
        }
        if (it == it_end) {
            break; // 遍历完线段
        }
    }
    for (auto it = to_be_deleted.begin(); it != to_be_deleted.end(); it++) {
        segment_tree.erase(*it);
    }
    for (auto it = to_be_inserted.begin(); it != to_be_inserted.end(); it++) {
        segment_tree.insert(*it);
    }

    cout << "OK" << endl;
}

void query(int p) {

    segment s;
    s.l = p;
    auto it_begin = segment_tree.upper_bound(s); // 寻找l值大于p的第一个已有线段
    if (it_begin == segment_tree.end()) {
        it_begin--; // 若寻找到了线段树的end处，则it_begin指向最后一个线段
    }

    for (auto it = it_begin;; it--) { // 向left值更小的方向遍历线段
        if (it->l <= p && it->r >= p) {
            if (it->id == 0) {
                cout << 0 << " " << 0 << endl;
            }
            else {
                cout << it->id << " " << it->value << endl;
            }
            return;
        }
    }
}

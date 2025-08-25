#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <cstdint>
using namespace std;
 
// ------------------------
// 线段树部分：支持查询区间内颜色段数、左右端颜色
// ------------------------
 
// 线段树节点结构
struct SegmentTreeNode {
    int left, right;        // 节点区间 [left, right]
    int segment_count;      // 该区间内颜色段数（相邻颜色相同视为同一段）
    int left_color, right_color;  // 区间最左/最右的颜色
    SegmentTreeNode *left_child, *right_child;
    SegmentTreeNode(int l, int r)
        : left(l), right(r), segment_count(1), left_color(0), right_color(0),
          left_child(nullptr), right_child(nullptr) {}
};
 
// 建树：叶节点对应单个位置，其颜色直接取自 colors 数组
SegmentTreeNode* buildTree(const vector<int>& colors, int l, int r) {
    SegmentTreeNode* node = new SegmentTreeNode(l, r);
    if (l == r) {
        node->left_color = node->right_color = colors[l];
        node->segment_count = 1;
        return node;
    }
    int mid = (l + r) / 2;
    node->left_child = buildTree(colors, l, mid);
    node->right_child = buildTree(colors, mid + 1, r);
    node->left_color = node->left_child->left_color;
    node->right_color = node->right_child->right_color;
    // 初步合并左右子区间的段数
    node->segment_count = node->left_child->segment_count + node->right_child->segment_count;
    // 如果左右子区间的边界颜色相同，则合并为一段
    if (node->left_child->right_color == node->right_child->left_color) {
        node->segment_count--;
    }
    return node;
}
 
// 查询区间 [l, r] 的颜色段数以及左右端颜色
void query(SegmentTreeNode* node, int l, int r, int& segment_count, int& first_color, int& last_color) {
    if (node == nullptr || node->right < l || node->left > r) {
        segment_count = 0;
        first_color = -1;
        last_color = -1;
        return;
    }
    if (l <= node->left && node->right <= r) {
        segment_count = node->segment_count;
        first_color = node->left_color;
        last_color = node->right_color;
        return;
    }
    int left_seg = 0, right_seg = 0;
    int left_first = -1, left_last = -1;
    int right_first = -1, right_last = -1;
    query(node->left_child, l, r, left_seg, left_first, left_last);
    query(node->right_child, l, r, right_seg, right_first, right_last);
    if (left_seg == 0) {
        segment_count = right_seg;
        first_color = right_first;
        last_color = right_last;
    } else if (right_seg == 0) {
        segment_count = left_seg;
        first_color = left_first;
        last_color = left_last;
    } else {
        segment_count = left_seg + right_seg;
        if (left_last == right_first) segment_count--;
        first_color = left_first;
        last_color = right_last;
    }
}
 
// ------------------------
// 块状分解部分：预处理整个颜色数组以快速回答区间内“不同颜色数”查询  
// 用 64 位整数数组模拟 bitset，每一位表示某种颜色是否出现。
// ------------------------
 
int B;            // 块大小（可调）
int numBlocks;    // 总块数
int W;            // 每个 bitset 需要的 64 位整数数量（W = ceil(m/64)）
vector<int> colors;  // 颜色数组（1-indexed）
vector<vector<uint64_t>> blockOR;  // 每块预处理的“或”结果
 
// 查询区间 [L,R] 内颜色的 bitset（即各颜色是否出现的标记），返回大小为 W 的 uint64_t 数组
vector<uint64_t> distinctQuery(int L, int R) {
    vector<uint64_t> res(W, 0ULL);
    int startBlock = (L - 1) / B;
    int endBlock = (R - 1) / B;
    if (startBlock == endBlock) {
        // 区间完全落在同一块，直接遍历求或
        for (int i = L; i <= R; i++) {
            int col = colors[i];
            int idx = col - 1;
            res[idx / 64] |= (1ULL << (idx % 64));
        }
    } else {
        // 处理起始块的部分
        int blockStartEnd = (startBlock + 1) * B;
        for (int i = L; i <= blockStartEnd; i++) {
            int col = colors[i];
            int idx = col - 1;
            res[idx / 64] |= (1ULL << (idx % 64));
        }
        // 处理中间整块
        for (int b = startBlock + 1; b < endBlock; b++) {
            for (int j = 0; j < W; j++) {
                res[j] |= blockOR[b][j];
            }
        }
        // 处理结束块的部分
        int endBlockStart = endBlock * B + 1;
        for (int i = endBlockStart; i <= R; i++) {
            int col = colors[i];
            int idx = col - 1;
            res[idx / 64] |= (1ULL << (idx % 64));
        }
    }
    return res;
}
 
// 将 src 的 bitset 结果“或”到 dest 上
void unionBitset(vector<uint64_t>& dest, const vector<uint64_t>& src) {
    for (int i = 0; i < W; i++) {
        dest[i] |= src[i];
    }
}
 
// 统计 bitset 中 1 的个数，即不同颜色数
int countBits(const vector<uint64_t>& bs) {
    int cnt = 0;
    for (auto x : bs) {
        cnt += __builtin_popcountll(x);
    }
    return cnt;
}
 
// ------------------------
// 主函数
// ------------------------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
 
    int n, m, k;
    cin >> n >> m >> k;
    // colors 数组采用 1-indexed，下标 1~n
    colors.resize(n + 1);
    
    bool allDistinct = true; // 标记是否所有颜色都不相同（即 c[i] = i 的情况），用于特殊情况优化
    for (int i = 1; i <= n; i++) {
        cin >> colors[i];
        if (colors[i] != i)
            allDistinct = false;
    }
    
    // 构建线段树（当 allDistinct 为 true 时，本质上不用查询区间信息，但为了代码统一构造）
    SegmentTreeNode* root = buildTree(colors, 1, n);
 
    // ------------------------
    // 块状分解预处理：构造每块的颜色“或”信息
    // ------------------------
    B = 512;  // 块大小（可根据实际情况调节）
    numBlocks = (n + B - 1) / B;
    // 每个 bitset 长度为 W = ceil(m/64)
    W = (m + 63) / 64;
    blockOR.assign(numBlocks, vector<uint64_t>(W, 0ULL));
    for (int b = 0; b < numBlocks; b++) {
        int L = b * B + 1;
        int R = min(n, (b + 1) * B);
        for (int i = L; i <= R; i++) {
            int col = colors[i];
            int idx = col - 1;
            blockOR[b][idx / 64] |= (1ULL << (idx % 64));
        }
    }
 
    // ------------------------
    // 每块木板维护一个 set，记录当前木板上存在的区间（区间端点均为原数组下标）
    // 初始时 1 号木板包含整个区间 [1, n]
    // ------------------------
    // 用 pair<int, int> 表示一个区间 [first, second]
    vector<set<pair<int, int>>> boards(k + 2);  // 共 k+1 号木板，编号 1~k+1
    boards[1].insert({1, n});
 
    // 处理 k 次切割操作
    // 每次输入 x, l, r 表示对木板 x 的区间 [l, r] 进行切割，
    // 切下来的部分构成新的木板，输出：切下部分的不同颜色数 和 颜色段数（合并相邻颜色相同的段后）
    for (int op = 1; op <= k; op++) {
        int x, l, r;
        cin >> x >> l >> r;
        set<pair<int, int>>& current = boards[x];
        // 收集本次切割中，被切下来的区间
        vector<pair<int, int>> cut_segments;
        // 在当前木板的区间集合中找到所有与 [l, r] 有交集的区间
        auto it = current.lower_bound({l, 0});
        if (it != current.begin()) --it;
        while (it != current.end()) {
            int s = it->first, e = it->second;
            if (s > r) break;  // 后面的区间起点超过 r，无需继续
            if (e < l) { // 当前区间完全在 [l, r] 之前
                ++it;
                continue;
            }
            // 求交集
            int a = max(s, l), b = min(e, r);
            if (a <= b) {
                cut_segments.push_back({a, b});
                // 从当前木板中移除该区间，并将剩余部分（若有）重新加入
                auto temp = it;
                ++it;
                current.erase(temp);
                if (s < a) {
                    current.insert({s, a - 1});
                }
                if (b < e) {
                    current.insert({b + 1, e});
                }
            } else {
                ++it;
            }
        }
        if (cut_segments.empty()) {
            cout << "0 0\n";
            boards[op + 1].clear();
            continue;
        }
        // 为了后续合并相邻区间处理，先对切割出的区间按起点排序
        sort(cut_segments.begin(), cut_segments.end());
        
        // 根据是否满足 c[i] = i（即 allDistinct 为 true）分别处理
        if (allDistinct) {
            // 在全不相同的情况下，每个区间内不同颜色数和颜色段数均等于区间长度
            int distinct_count = 0, total_segments = 0;
            for (auto& seg : cut_segments) {
                int len = seg.second - seg.first + 1;
                distinct_count += len;
                total_segments += len;
            }
            cout << distinct_count << " " << total_segments << "\n";
        } else {
            // ------------------------
            // 1. 利用块状分解“distinctQuery”求多个区间中不同颜色的并集
            // ------------------------
            vector<uint64_t> union_bs(W, 0ULL);
            // 统计所有被切区间的颜色（并集）
            for (auto& seg : cut_segments) {
                int a = seg.first, b = seg.second;
                vector<uint64_t> bs = distinctQuery(a, b);
                unionBitset(union_bs, bs);
            }
            int distinct_count = countBits(union_bs);
 
            // ------------------------
            // 2. 利用线段树查询每个被切区间的颜色段数，然后对相邻区间作合并处理
            // ------------------------
            int total_segments = 0;
            for (auto& seg : cut_segments) {
                int a = seg.first, b = seg.second;
                int seg_count, first, last;
                query(root, a, b, seg_count, first, last);
                total_segments += seg_count;
            }
            // 如果相邻两个切割区间原本是连续的且两端颜色相同，则合并后颜色段数减少1
            int merged = 0;
            for (int i = 1; i < (int)cut_segments.size(); i++) {
                int prev_b = cut_segments[i - 1].second;
                int curr_a = cut_segments[i].first;
                if (prev_b + 1 == curr_a) {
                    int dummy;
                    int col1, col2;
                    query(root, prev_b, prev_b, dummy, col1, col1);
                    query(root, curr_a, curr_a, dummy, col2, col2);
                    if (col1 == col2) {
                        merged++;
                    }
                }
            }
            total_segments -= merged;
            cout << distinct_count << " " << total_segments << "\n";
        }
        // 新木板编号为 op+1，记录本次切下的所有区间
        boards[op + 1].clear();
        for (auto& seg : cut_segments) {
            boards[op + 1].insert(seg);
        }
    }
    return 0;
}
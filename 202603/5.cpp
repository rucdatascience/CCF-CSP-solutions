#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <random>

using namespace std;

const int MAXN = 100005;
const int MAXSEGS = 2100005;

struct Edge {
    int to, id;
};

int N, X_param, K, M, Q;
vector<Edge> adj[MAXN];
int stations[25];

// 树链剖分
int depth[MAXN], parent_node[MAXN], sz[MAXN], hson[MAXN];
int top_node[MAXN], dfn[MAXN], timer_dfn = 0;
int edge_id_of_node[MAXN]; // node -> edge entering it
int dfn_to_edge[MAXN];     // dfn -> edge id

void dfs1(int u, int p, int d) {
    depth[u] = d;
    parent_node[u] = p;
    sz[u] = 1;
    hson[u] = 0;
    int max_sub = 0;
    for (const auto& edge : adj[u]) {
        int v = edge.to;
        if (v == p) continue;
        edge_id_of_node[v] = edge.id;
        dfs1(v, u, d + 1);
        sz[u] += sz[v];
        if (sz[v] > max_sub) {
            max_sub = sz[v];
            hson[u] = v;
        }
    }
}

void dfs2(int u, int t) {
    top_node[u] = t;
    dfn[u] = ++timer_dfn;
    if (edge_id_of_node[u] != 0) {
        dfn_to_edge[dfn[u]] = edge_id_of_node[u];
    }
    if (hson[u]) dfs2(hson[u], t);
    for (const auto& edge : adj[u]) {
        int v = edge.to;
        if (v == parent_node[u] || v == hson[u]) continue;
        dfs2(v, v);
    }
}

int get_lca(int u, int v) {
    while (top_node[u] != top_node[v]) {
        if (depth[top_node[u]] < depth[top_node[v]]) swap(u, v);
        u = parent_node[top_node[u]];
    }
    return depth[u] < depth[v] ? u : v;
}

int get_dist(int u, int v) {
    return depth[u] + depth[v] - 2 * depth[get_lca(u, v)];
}

// 树状数组 (维护路径破损边数)
int bit[MAXN];
void add_bit(int idx, int val) {
    for (; idx <= N; idx += idx & -idx) bit[idx] += val;
}
int query_bit(int idx) {
    int sum = 0;
    for (; idx > 0; idx -= idx & -idx) sum += bit[idx];
    return sum;
}

// 获取 u 到 v 路径上的破损边数量
int get_broken_count(int u, int v) {
    int res = 0;
    while (top_node[u] != top_node[v]) {
        if (depth[top_node[u]] < depth[top_node[v]]) swap(u, v);
        res += query_bit(dfn[u]) - query_bit(dfn[top_node[u]] - 1);
        u = parent_node[top_node[u]];
    }
    if (depth[u] > depth[v]) swap(u, v);
    if (u != v) {
        res += query_bit(dfn[v]) - query_bit(dfn[u]);
    }
    return res;
}

// 获取 u 到 v 路径上第 k 个破损边 (基于树状数组二分)
int get_kth_broken(int u, int v, int k) {
    vector<pair<int, int>> intervals;
    while (top_node[u] != top_node[v]) {
        if (depth[top_node[u]] < depth[top_node[v]]) swap(u, v);
        intervals.push_back({dfn[top_node[u]], dfn[u]});
        u = parent_node[top_node[u]];
    }
    if (depth[u] > depth[v]) swap(u, v);
    if (u != v) intervals.push_back({dfn[u] + 1, dfn[v]});
    
    for (const auto& p : intervals) {
        int L = p.first, R = p.second;
        int cnt = query_bit(R) - query_bit(L - 1);
        if (k <= cnt) {
            int target = k + query_bit(L - 1);
            int pos = 0;
            for (int i = 17; i >= 0; --i) {
                if (pos + (1 << i) <= N && bit[pos + (1 << i)] < target) {
                    pos += (1 << i);
                    target -= bit[pos];
                }
            }
            return dfn_to_edge[pos + 1];
        }
        k -= cnt;
    }
    return -1;
}

// 核心监视状态
struct UniqueSegment { int u, v; };
UniqueSegment unique_segs[MAXSEGS];
bool seg_valid[MAXSEGS];
int watched_by[MAXSEGS][2];
vector<int> plans_using_seg[MAXSEGS];
vector<int> watch_list[MAXN];

int plan_invalid_count[MAXN];
bool edge_repaired[MAXN];

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    if (!(cin >> N >> X_param)) return 0;

    for (int i = 1; i < N; ++i) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back({v, i});
        adj[v].push_back({u, i});
    }

    cin >> K;
    for (int i = 0; i < K; ++i) cin >> stations[i];

    dfs1(1, 0, 1);
    dfs2(1, 1);

    // 初始所有边都是损坏的
    for (int i = 2; i <= N; ++i) add_bit(i, 1);

    cin >> M;
    map<pair<int, int>, int> seg_map;
    int num_segs = 0;

    for (int i = 1; i <= M; ++i) {
        int S, T;
        cin >> S >> T;
        vector<pair<int, int>> path_stations;
        int dist_ST = get_dist(S, T);
        
        // 利用距离判定维修站是否在当前路径上
        for (int j = 0; j < K; ++j) {
            int U = stations[j];
            if (get_dist(S, U) + get_dist(U, T) == dist_ST) {
                path_stations.push_back({get_dist(S, U), U});
            }
        }
        sort(path_stations.begin(), path_stations.end());
        
        vector<pair<int, int>> local_segs;
        if (path_stations.empty()) {
            local_segs.push_back({S, T});
        } else {
            int curr = S;
            for (auto& p : path_stations) {
                int U = p.second;
                if (curr != U) local_segs.push_back({curr, U});
                curr = U;
            }
            if (curr != T) local_segs.push_back({curr, T});
        }

        // 去重记录片段
        for (auto& seg : local_segs) {
            int u = seg.first, v = seg.second;
            if (u > v) swap(u, v);
            if (seg_map.find({u, v}) == seg_map.end()) {
                seg_map[{u, v}] = ++num_segs;
                unique_segs[num_segs] = {u, v};
            }
            int sid = seg_map[{u, v}];
            plans_using_seg[sid].push_back(i);
        }
    }

    mt19937 rnd(1337);
    int feasible_plans = 0;

    for (int s = 1; s <= num_segs; ++s) {
        int u = unique_segs[s].u;
        int v = unique_segs[s].v;
        int W = get_broken_count(u, v);
        
        if (W <= 1) {
            seg_valid[s] = true;
        } else {
            seg_valid[s] = false;
            for (int p_id : plans_using_seg[s]) {
                plan_invalid_count[p_id]++;
            }
            
            // 随机挑选2条边进行监视
            int r1 = rnd() % W + 1, r2;
            do { r2 = rnd() % W + 1; } while (r1 == r2);
            
            int e1 = get_kth_broken(u, v, r1);
            int e2 = get_kth_broken(u, v, r2);
            
            watch_list[e1].push_back(s);
            watch_list[e2].push_back(s);
            watched_by[s][0] = e1;
            watched_by[s][1] = e2;
        }
    }

    // 初始合法计划数量
    for (int i = 1; i <= M; ++i) {
        if (plan_invalid_count[i] == 0) feasible_plans++;
    }

    cin >> Q;
    long long lastans = 0;

    for (int q = 0; q < Q; ++q) {
        int op;
        cin >> op;
        if (op == 1) {
            int u_raw, v_raw;
            cin >> u_raw >> v_raw;
            int u = u_raw ^ (X_param * lastans);
            int v = v_raw ^ (X_param * lastans);
            
            if (depth[u] < depth[v]) swap(u, v);
            int edge_id = edge_id_of_node[u];

            if (!edge_repaired[edge_id]) {
                edge_repaired[edge_id] = true;
                add_bit(dfn[u], -1); // 从树状数组移除破损标记
                
                vector<int> to_check = watch_list[edge_id];
                watch_list[edge_id].clear();
                
                for (int s : to_check) {
                    if (seg_valid[s]) continue;
                    int su = unique_segs[s].u, sv = unique_segs[s].v;
                    int W = get_broken_count(su, sv);
                    
                    if (W <= 1) { // 片段变合法
                        seg_valid[s] = true;
                        for (int p_id : plans_using_seg[s]) {
                            plan_invalid_count[p_id]--;
                            if (plan_invalid_count[p_id] == 0) {
                                feasible_plans++;
                            }
                        }
                    } else { // 片段仍失效，寻找新的替换边监视
                        int other_e = (watched_by[s][0] == edge_id) ? watched_by[s][1] : watched_by[s][0];
                        int new_e = -1;
                        while (true) {
                            int r = rnd() % W + 1;
                            new_e = get_kth_broken(su, sv, r);
                            if (new_e != other_e) break;
                        }
                        watch_list[new_e].push_back(s);
                        if (watched_by[s][0] == edge_id) watched_by[s][0] = new_e;
                        else watched_by[s][1] = new_e;
                    }
                }
            }
        } else if (op == 2) {
            cout << feasible_plans << "\n";
            lastans = feasible_plans;
        }
    }

    return 0;
}
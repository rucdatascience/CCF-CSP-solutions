#include <iostream>
#include <vector>
#include <queue>
#include <limits>

// 为了方便，定义一些类型别名
using ll = long long;
using pii = std::pair<ll, int>; // {distance, vertex}

// 节点坐标结构体
struct Point {
    ll x, y;
};

// 图的邻接表表示
std::vector<std::pair<int, int>> adj[5000 + 5000 + 1];
// 距离数组
ll dist[5000 + 5000 + 1];

const ll INF = std::numeric_limits<ll>::max();

int main() {
    // 提高C++标准输入的效率
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n, m;
    std::cin >> n >> m;

    std::vector<Point> nodes(n + 1);
    for (int i = 1; i <= n; ++i) {
        std::cin >> nodes[i].x >> nodes[i].y;
    }

    // 建图过程
    for (int j = 1; j <= m; ++j) {
        ll xj, yj, rj, tj;
        std::cin >> xj >> yj >> rj >> tj;

        int station_node_idx = n + j;

        for (int i = 1; i <= n; ++i) {
            // 检查节点 i 是否在基站 j 的覆盖范围内
            // abs(nodes[i].x - xj) <= rj
            // abs(nodes[i].y - yj) <= rj
            if (std::abs(nodes[i].x - xj) <= rj && std::abs(nodes[i].y - yj) <= rj) {
                // 添加两条有向边
                // 1. 从物理节点 i 到基站虚拟节点 n+j
                adj[i].push_back({station_node_idx, (int)tj});
                // 2. 从基站虚拟节点 n+j 到物理节点 i
                adj[station_node_idx].push_back({i, 0});
            }
        }
    }

    // Dijkstra 算法
    // 1. 初始化距离数组
    for (int i = 1; i <= n + m; ++i) {
        dist[i] = INF;
    }
    dist[1] = 0;

    // 2. 优先队列，用于Dijkstra
    std::priority_queue<pii, std::vector<pii>, std::greater<pii>> pq;
    pq.push({0, 1}); // {distance, vertex}

    while (!pq.empty()) {
        ll d = pq.top().first;
        int u = pq.top().second;
        pq.pop();

        // 如果已经有更短的路径，则跳过
        if (d > dist[u]) {
            continue;
        }

        // 遍历所有邻接边
        for (auto& edge : adj[u]) {
            int v = edge.first;
            int weight = edge.second;

            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                pq.push({dist[v], v});
            }
        }
    }

    // 输出结果
    if (dist[n] == INF) {
        std::cout << "Nan" << std::endl;
    } else {
        std::cout << dist[n] << std::endl;
    }

    return 0;
}
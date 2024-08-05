#include <bits/stdc++.h>
using namespace std;
typedef pair<int, int> pii;
const int N = 4e5 + 5;
class Edge {
   public:
    long long w;
    int to = 0;
    Edge(int id, long long w)
        : w(w), to(id) {}
    bool operator<(const Edge& rhs) const {
        return w == rhs.w ? to < rhs.to : w > rhs.w;
    }
};
vector<int> end_u[N], end_v[N], end_x[N], end_y[N];
map<pii, long long> weight;
set<Edge> edges[N];
int n, m, T, aloneCount = 0, primaryTargetCount = 0;
bool hasPrimaryTarget(int u) {
    return (!edges[u].empty() &&
            edges[edges[u].begin()->to].begin()->to == u);
}
int main() {
    scanf("%d%d", &n, &m);
    aloneCount = n;
    primaryTargetCount = 0;
    for (int i = 1; i <= m; i++) {
        if (!end_u[i].empty()) {  // 请读下文，这里的处理是下文的镜像操作
            for (size_t j = 0; j < end_u[i].size(); j++) {
                int u = end_u[i][j], v = end_v[i][j], x = end_x[i][j];
                bool isPrimaryTarget = 0;
                if (edges[u].empty())
                    aloneCount--;
                if (edges[v].empty())
                    aloneCount--;
                primaryTargetCount -= hasPrimaryTarget(u);
                primaryTargetCount -= hasPrimaryTarget(v);
                isPrimaryTarget = (!edges[u].empty() && !edges[v].empty() &&
                                   edges[u].begin()->to == v && edges[v].begin()->to == u);
                primaryTargetCount += isPrimaryTarget;
                long long w = weight[make_pair(u, v)];
                edges[u].erase(Edge(v, w));
                edges[v].erase(Edge(u, w));
                weight[make_pair(u, v)] -= x;
                w -= x;
                if (w > 0) {
                    edges[u].insert(Edge(v, w));
                    edges[v].insert(Edge(u, w));
                }
                if (edges[u].empty())
                    aloneCount++;
                if (edges[v].empty())
                    aloneCount++;
                primaryTargetCount += hasPrimaryTarget(u);
                primaryTargetCount += hasPrimaryTarget(v);
                isPrimaryTarget = (!edges[u].empty() && !edges[v].empty() &&
                                   edges[u].begin()->to == v && edges[v].begin()->to == u);
                primaryTargetCount -= isPrimaryTarget;
            }
        }

        int k;
        scanf("%d", &k);
        for (int j = 1; j <= k; j++) {
            int u, v, x, y;
            scanf("%d%d%d%d", &u, &v, &x, &y);
            if (u > v)
                swap(u, v);

            // 为失效的那天保存信息
            end_u[i + y].push_back(u);
            end_v[i + y].push_back(v);
            end_x[i + y].push_back(x);

            // 更新和 u 或 v 相关的通信孤岛个数
            // 更新方法是：先在答案里扣除之前的，再加上新的

            if (edges[u].empty())
                aloneCount--;
            if (edges[v].empty())
                aloneCount--;

            // 更新和 u 或 v 相关的通信对个数
            // 更新方法是：先在答案里扣除之前的，再加上新的

            // 先扣除可能之前和 u 或 v 相关的通信对
            bool isPrimaryTarget = 0;
            primaryTargetCount -= hasPrimaryTarget(u);
            primaryTargetCount -= hasPrimaryTarget(v);
            isPrimaryTarget = (!edges[u].empty() && !edges[v].empty() &&
                               edges[u].begin()->to == v && edges[v].begin()->to == u);
            primaryTargetCount += isPrimaryTarget;  // 如果 uv 恰好是通信对，那么要补偿回来

            long long w = weight[make_pair(u, v)];  // 之前的通信量，取出是为了删除 edges[u] 和 edges[v] 中的元素
            edges[u].erase(Edge(v, w));             // 删除 u 和 v 之间的通信量
            edges[v].erase(Edge(u, w));             // 删除 v 和 u 之间的通信量
            weight[make_pair(u, v)] += x;           // 更新 u 和 v 之间的通信量
            w += x;
            if (w > 0) {  // 如果通信量大于 0，那么重新插入，即更新
                edges[u].insert(Edge(v, w));
                edges[v].insert(Edge(u, w));
            }

            if (edges[u].empty())  // 如果 u 没有通信量，那么通信孤岛个数加一
                aloneCount++;
            if (edges[v].empty())  // 如果 v 没有通信量，那么通信孤岛个数加一
                aloneCount++;

            // 再更新可能新的和 u 或 v 相关的通信对
            primaryTargetCount += hasPrimaryTarget(u);
            primaryTargetCount += hasPrimaryTarget(v);
            isPrimaryTarget = (!edges[u].empty() && !edges[v].empty() &&
                               edges[u].begin()->to == v && edges[v].begin()->to == u);
            primaryTargetCount -= isPrimaryTarget;
            // 同理，如果 uv 恰好是通信对，那么要扣除
        }

        int l;
        scanf("%d", &l);
        for (int j = 1; j <= l; j++) {
            int u;
            scanf("%d", &u);
            if (!edges[u].empty()) {
                printf("%d\n", edges[u].begin()->to);
            } else {
                printf("0\n");
            }
        }

        int p, q;
        scanf("%d%d", &p, &q);
        if (p)
            printf("%d\n", aloneCount);
        if (q)
            printf("%d\n", primaryTargetCount);
    }
}
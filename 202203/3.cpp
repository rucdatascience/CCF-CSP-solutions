#include <bits/stdc++.h>
using namespace std;
typedef long long LL;
typedef pair<int, int> pii;
const int N = 2000 + 5, mod = 1e9 + 7;
const double eps = 1e-7;
int n, m, l[N], f[N], a[N], na[N], pa[N], paa[N], paar[N], g, T, sum[N];
unordered_map<int, bool> ML[N], MN[N];
// remind:
// l[i] 表示计算节点 i 的可用区
// ML[i][j] 表示可用区 i 中有没有 j 应用
// MN[i][j] 表示计算节点 i 中有没有 j 应用
int work(int a, int na, int pa, int paa) {
    int ret = 0;
    for (int i = 1; i <= n; i++) {
        if (na && l[i] != na)  // 计算节点 i 不在指定的可用区中
            continue;
        if (pa && !ML[l[i]][pa])  // 计算节点 i 的可用区中没有 pa 应用
            continue;
        if (paa && MN[i][paa])  // 计算节点 i 中有 ppa 应用
            continue;
        if (!ret || sum[ret] > sum[i])  // 选择 sum 最小的节点
            ret = i;
    }
    return ret;
}
int main() {
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= n; i++) {
        scanf("%d", &l[i]);
    }
    scanf("%d", &g);
    for (int i = 1; i <= g; i++) {
        scanf("%d%d%d%d%d%d", f + i, a + i, na + i, pa + i, paa + i, paar + i);
        for (int j = 1; j <= f[i]; j++) {
            int ans = 0;
            if (!paar[i]) {
                ans = work(a[i], na[i], pa[i], paa[i]);
                if (!ans)
                    ans = work(a[i], na[i], pa[i], 0);
            } else
                ans = work(a[i], na[i], pa[i], paa[i]);
            ML[l[ans]][a[i]] = 1;
            MN[ans][a[i]] = 1;
            sum[ans]++;
            printf("%d ", ans);
        }
        puts("");
    }
}
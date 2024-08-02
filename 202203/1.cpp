#include <bits/stdc++.h>
using namespace std;
int n, m, T, sum = 0;
bool vis[100005];
int main() {
    cin >> n >> m;
    vis[0] = 1;
    for (int i = 1; i <= m; i++) {
        int a, b;
        cin >> a >> b;
        if (!vis[b])  // 之前出现过
            sum++;
        vis[a] = 1;  // 标记为出现过
    }
    cout << sum << endl;
}
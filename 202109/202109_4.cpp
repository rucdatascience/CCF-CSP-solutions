/*
CSP202109_4 收集卡牌
得分：100
*/
#include <bits/stdc++.h>
using namespace std;

const int maxn = 30;
using namespace std;
int N, K;
double p[maxn + 5];
int book[maxn + 5];
double dp[100][80000];
double dfs(int level, int have, int state) {//层数，已经抽中的卡牌数，01状态
    if ((N - have) * K <= level - have) {
        return level;
    }
    double ans = 0;
    for (int i = 0; i < N; i++) {
        if (book[i] == 0) {
            book[i] = 1;
            if (dp[level + 1][state + (1 << i)] == 0) {
                dp[level + 1][state + (1 << i)] = dfs(level + 1, have + 1, state + (1 << i));
            }
            ans += p[i] * dp[level + 1][state + (1 << i)];
            book[i] = 0;
        }
        else {
            if (dp[level + 1][state] == 0) {
                dp[level + 1][state] = dfs(level + 1, have, state);
            }
            ans += p[i] * dp[level + 1][state];
        }
    }
    return ans;
}
int main()
{
    cin >> N >> K;
    for (int i = 0; i < N; i++) {
        cin >> p[i];
    }
    double ans = dfs(0, 0, 0);
    printf("%.10f", ans);
    return 0;
}


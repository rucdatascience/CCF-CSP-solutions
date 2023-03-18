/*
CSP202109_4 收集卡牌
得分：100
*/
#include <bits/stdc++.h>
using namespace std;

using namespace std;
int N, K;
double p[17];
double dp[100][70000]; // 第一维表示已抽取次数（因为k<=5，在最差情况下只抽到一种卡，那么最多需要5 × 15 + 1 = 76次即可收集完成），第二维表示抽取卡牌的01状态（2^16 =65536），dp记录下面的dfs返回的期望抽取次数（动态规划：避免重复计算）
double dfs(int times, int have, int state) { // 已抽取次数，已抽中的卡牌数，抽取卡牌的01状态；  返回从输入状态开始到抽中所有卡牌的的期望抽取次数（包含已抽取次数） （have其实是可以从state算出来的，两者对应）
    if ((N - have) * K <= times - have) { // 通过兑换就可以获得剩余所有卡牌，因此返回期望次数为输入的已有次数times
        return times;
    }
    double ans = 0;
    for (int i = 0; i < N; i++) { // 枚举在输入状态下再抽取某种卡牌
        if (state & (1 << i)) { // 输入状态中第i张牌没被抽取
            if (dp[times + 1][state | (1 << i)] == 0) { // dp[times + 1][state | (1 << i)] 还没被计算过（动态规划：避免重复计算）
                dp[times + 1][state | (1 << i)] = dfs(times + 1, have + 1, state | (1 << i)); // 注意此处的have+1
            }
            ans += p[i] * dp[times + 1][state | (1 << i)]; // 递归公式： dp[times + 1][state | (1 << i)]期望次数*抽取第i张牌的概率
        }
        else {
            if (dp[times + 1][state] == 0) {
                dp[times + 1][state] = dfs(times + 1, have, state); // 注意此处的have没变
            }
            ans += p[i] * dp[times + 1][state];
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
    printf("%.10f", ans); //注意输出时要保留足够位数
    return 0;
}


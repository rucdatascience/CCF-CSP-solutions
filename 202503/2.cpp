#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    // 提高C++标准流的输入输出效率
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    // 读入苹果总数 n 和每日最大投喂量 m
    int n, m;
    std::cin >> n >> m;

    // A[0] = 0，为了方便处理，我们将快乐值存储在大小为 m+1 的 vector 中
    // A[j] 对应投喂 j 个苹果的快乐值
    std::vector<int> a(m + 1, 0);
    for (int i = 1; i <= m; ++i) {
        std::cin >> a[i];
    }

    // dp 数组：dp[i] 表示消耗 i 个苹果能获得的最大快乐值
    // 使用 long long 防止 n * max(A[i]) 可能产生的整数溢出
    std::vector<long long> dp(n + 1, 0);

    // 填充 dp 数组
    // 外层循环遍历背包容量（苹果总数）
    for (int i = 1; i <= n; ++i) {
        // 内层循环遍历物品（每日投喂方案）
        for (int j = 1; j <= m; ++j) {
            // 如果当前苹果总数 i 足够进行 j 个苹果的投喂
            if (i >= j) {
                // 状态转移：
                // dp[i] 的值更新为 “不选择投喂j个苹果” 和 “选择投喂j个苹果” 两者中的较大值
                dp[i] = std::max(dp[i], dp[i - j] + a[j]);
            }
        }
    }

    // dp[n] 就是消耗 n 个苹果能获得的最大快乐值
    std::cout << dp[n] << std::endl;

    return 0;
}
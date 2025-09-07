#include <iostream>
#include <vector>
#include <numeric>   // C++17及以上版本中的 std::gcd
#include <utility>   // std::swap 和 std::move
#include <algorithm>

// 如果编译器支持 C++17 或更高版本，则使用 std::gcd，
// 否则使用自定义的 GCD 函数以保证兼容性。
#if __cplusplus >= 201703L
#define an_gcd std::gcd
#else
long long custom_gcd(long long a, long long b) {
    while (b) {
        a %= b;
        std::swap(a, b);
    }
    return a;
}
#define an_gcd custom_gcd
#endif

const int MOD = 998244353;

// 计算从 1 到 k 的和 (1+2+...+k) % MOD
// 使用 __int128 类型来避免 k*(k+1) 在除以 2 之前发生溢出。
long long sum_1_to_k(long long k) {
    if (k <= 0) return 0;
    __int128 k_128 = k;
    __int128 res_128 = k_128 * (k_128 + 1) / 2;
    return res_128 % MOD;
}

int main() {
    // 优化标准输入输出
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n;
    std::cin >> n;

    std::vector<int> a(n);
    for (int i = 0; i < n; ++i) {
        std::cin >> a[i];
    }

    long long total_value = 0;
    
    // 存储GCD块的列表：{gcd值, 起始的l坐标}
    // 每个块代表一段具有相同GCD值的左端点'l'。
    // 列表按 l 坐标的降序（从大到小）存储。
    std::vector<std::pair<int, int>> gcd_blocks;

    // 遍历所有可能的右端点 'r'
    for (int r = 1; r <= n; ++r) {
        int current_val = a[r - 1];
        
        // 'next_gcd_blocks' 用于存储当前 'r' 计算出的新GCD块。
        std::vector<std::pair<int, int>> next_gcd_blocks;
        // 第一个块是 l=r 的情况，其GCD就是当前元素 a[r-1]。
        next_gcd_blocks.push_back({current_val, r});

        // 用新元素 a[r-1] 更新上一轮的GCD块。
        for (const auto& block : gcd_blocks) {
            // 新的GCD是旧块的GCD值与当前元素的GCD。
            int new_gcd = an_gcd(block.first, current_val);
            
            // 如果新计算出的GCD与 'next_gcd_blocks' 中最后一个块的GCD相同，
            // 则合并这两个块（通过更新起始坐标来扩展范围）。
            if (new_gcd == next_gcd_blocks.back().first) {
                next_gcd_blocks.back().second = block.second;
            } else {
                // 如果GCD不同，则添加一个新块。
                next_gcd_blocks.push_back({new_gcd, block.second});
            }
        }
        // 使用 std::move 高效地将新块列表赋值给主列表，避免不必要的数据拷贝。
        gcd_blocks = std::move(next_gcd_blocks);

        // 计算当前右端点 'r' 的贡献值: sum_{l=1 to r} (l * gcd(l,r))
        long long sum_for_r = 0;
        int right_boundary_of_l = r;
        
        // 遍历当前 'r' 对应的所有GCD块。
        for (const auto& block : gcd_blocks) {
            int val = block.first;
            int l_start = block.second;

            // 当前块对应的 'l' 的范围是 [l_start, right_boundary_of_l]。
            // 计算这个范围内所有 'l' 的和。
            long long sum_of_indices = (sum_1_to_k(right_boundary_of_l) - sum_1_to_k(l_start - 1) + MOD) % MOD;
            
            // 这个块对 sum_for_r 的贡献是 val * (l的和)。
            long long term = (static_cast<long long>(val) * sum_of_indices) % MOD;
            sum_for_r = (sum_for_r + term) % MOD;
            
            // 为下一个块（其 'l' 的范围更小，即更靠左）更新右边界。
            right_boundary_of_l = l_start - 1;
        }
        
        // 当前 'r' 对总结果的最终贡献是 r * sum_for_r。
        long long r_contribution = (static_cast<long long>(r) * sum_for_r) % MOD;
        total_value = (total_value + r_contribution) % MOD;
    }

    std::cout << total_value << std::endl;

    return 0;
}
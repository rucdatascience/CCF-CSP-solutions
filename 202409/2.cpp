#include <iostream>
#include <string>
#include <vector>
#include <map>

// 定义所有可能的字符
const std::string CHARSET = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 ";
const int CHARSET_SIZE = CHARSET.length();

// 预先建立字符到索引和索引到字符的映射，方便快速查找
std::map<char, int> char_to_idx;
std::vector<char> idx_to_char;

// 函数复合操作
// a(b(x)) -> new_map[x] = a_map[b_map[x]]
std::vector<int> compose(const std::vector<int>& a_map, const std::vector<int>& b_map) {
    std::vector<int> result(CHARSET_SIZE);
    for (int i = 0; i < CHARSET_SIZE; ++i) {
        result[i] = a_map[b_map[i]];
    }
    return result;
}

int main() {
    // 禁用C++流与C标准流的同步，加快IO速度
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    // 1. 初始化字符映射表
    for (int i = 0; i < CHARSET_SIZE; ++i) {
        char_to_idx[CHARSET[i]] = i;
        idx_to_char.push_back(CHARSET[i]);
    }

    // 2. 读取初始字符串 s
    std::string line;
    std::getline(std::cin, line);
    std::string s = line.substr(1, line.length() - 2);

    // 3. 读取并构建基础变换 f^1
    int n;
    std::cin >> n;
    std::getline(std::cin, line); // 消费掉n后面的换行符

    // 基础变换映射 f^1
    std::vector<int> base_transform(CHARSET_SIZE);
    // 初始为恒等变换，即 f(ch) = ch
    for (int i = 0; i < CHARSET_SIZE; ++i) {
        base_transform[i] = i;
    }

    // 根据输入的n个规则更新基础变换
    for (int i = 0; i < n; ++i) {
        std::getline(std::cin, line);
        char from = line[1];
        char to = line[2];
        base_transform[char_to_idx[from]] = char_to_idx[to];
    }
    
    // 4. 预计算 f^(2^i) 的变换映射
    // f_powers[i] 存储 f^(2^i) 的变换
    // log2(10^9)约等于29.89，所以计算到30就足够了
    std::vector<std::vector<int>> f_powers(31, std::vector<int>(CHARSET_SIZE));
    f_powers[0] = base_transform; // f^1

    for (int i = 1; i <= 30; ++i) {
        // f^(2^i) = f^(2^(i-1)) o f^(2^(i-1))
        f_powers[i] = compose(f_powers[i-1], f_powers[i-1]);
    }

    // 5. 处理 m 个查询
    int m;
    std::cin >> m;
    for (int i = 0; i < m; ++i) {
        long long k;
        std::cin >> k;

        // 计算 f^k 的最终变换映射
        std::vector<int> final_transform(CHARSET_SIZE);
        // 初始化为恒等变换
        for (int j = 0; j < CHARSET_SIZE; ++j) {
            final_transform[j] = j;
        }

        // 使用二进制幂的思想来组合变换
        // k = b_0*2^0 + b_1*2^1 + ...
        // f^k = f^(b_0*2^0) o f^(b_1*2^1) o ...
        for (int j = 0; j <= 30; ++j) {
            if ((k >> j) & 1) { // 检查k的第j位二进制是否为1
                final_transform = compose(f_powers[j], final_transform);
            }
        }
        
        // 6. 应用最终变换并输出结果
        std::string result_s = "";
        for (char ch : s) {
            int original_idx = char_to_idx[ch];
            int new_idx = final_transform[original_idx];
            result_s += idx_to_char[new_idx];
        }

        std::cout << "#" << result_s << "#" << std::endl;
    }

    return 0;
}
#include <iostream>
#include <string>
#include <vector>
#include <cctype> // 包含 isalpha 和 isdigit 函数

// 函数用于判断单个密码的安全级别
void check_password_security(const std::string& password) {
    // 标志位，用于判断是否包含所需的三种字符类型
    bool has_letter = false;
    bool has_digit = false;
    bool has_special = false;

    // 使用一个数组来统计每个字符的出现次数
    // ASCII 码范围为 0-127，所以大小为 128 的数组足够
    std::vector<int> char_counts(128, 0);

    // 第一次遍历：检查字符类型并统计字符频率
    for (char ch : password) {
        if (isalpha(ch)) { // isalpha() 检查字符是否为字母 (a-z, A-Z)
            has_letter = true;
        } else if (isdigit(ch)) { // isdigit() 检查字符是否为数字 (0-9)
            has_digit = true;
        } else if (ch == '*' || ch == '#') {
            has_special = true;
        }
        // 增加对应字符的计数
        char_counts[static_cast<int>(ch)]++;
    }

    // 步骤 1: 判断是否满足中等级别的基本要求
    // 如果不包含字母、数字、特殊字符中的任何一种，则为低安全度
    if (!has_letter || !has_digit || !has_special) {
        std::cout << 0 << std::endl;
        return;
    }

    // 到这里，密码至少是中等级别。现在检查是否能达到高级别。
    // 步骤 2: 判断是否满足高等级别的要求（字符出现次数）
    bool is_high_security = true;
    for (int count : char_counts) {
        if (count > 2) {
            is_high_security = false;
            break; // 找到一个出现超过2次的字符，无需继续检查
        }
    }

    if (is_high_security) {
        // 满足所有高安全度要求
        std::cout << 2 << std::endl;
    } else {
        // 不满足高安全度要求，但满足中安全度要求
        std::cout << 1 << std::endl;
    }
}

int main() {
    // 加速 C++ 的标准输入输出
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n;
    std::cin >> n;

    std::string password;
    // 循环读取 n 个密码
    for (int i = 0; i < n; ++i) {
        std::cin >> password;
        check_password_security(password);
    }

    return 0;
}
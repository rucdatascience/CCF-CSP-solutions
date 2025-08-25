#include <iostream>

int main() {
    // 为了提高输入输出效率（在此题中非必需，但为良好习惯）
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    // 1. 读取输入
    int b, c, l, r;
    std::cin >> b >> c >> l >> r;

    // 2. 初始化总和变量，使用 long long 防止溢出
    long long sum = 0;

    // 3. 确定循环的起始点 (start_x)
    int start_x = l;
    if (l % 2 != 0) {
        // 如果 l 是奇数，则从 l 之后的第一个偶数开始
        start_x = l + 1;
    }
    // 上述 if 语句也可以写成三元表达式：
    // int start_x = (l % 2 == 0) ? l : l + 1;

    // 4. 循环遍历 [l, r] 区间内的所有偶数坐标点
    for (int x = start_x; x <= r; x += 2) {
        // 计算 f(x) = x^2 + bx + c
        // 显式地将计算中的乘法项转换为 long long，以确保中间结果的正确性
        long long fx = (long long)x * x + (long long)b * x + c;
        
        // 累加函数值
        sum += fx;
    }

    // 5. 计算最终结果 s = 2 * sum
    long long s = 2 * sum;

    // 6. 输出结果
    std::cout << s << std::endl;

    return 0;
}
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <map>

// 定义模数
const int MOD = 1000000007;

// 操作数结构体
// 表示表达式中的一个部分，可能是变量或字面字符串
struct Operand {
    bool is_variable; // true 代表是变量 ($var), false 代表是字面字符串
    std::string value;    // 存储变量名或字面字符串内容
};

// 变量信息结构体
// 存储一个变量的所有信息
struct VariableInfo {
    int type = 0; // 0: 未赋值, 1: 直接赋值, 2: 间接赋值

    // 类型为 1 (直接赋值) 时使用，存储已计算好的长度
    long long length = 0;

    // 类型为 2 (间接赋值) 时使用，存储其对应的表达式
    std::vector<Operand> expression;
};

// 使用 map 全局存储所有变量的信息，键为变量名
std::map<std::string, VariableInfo> variables;

// 核心函数：递归计算一个变量在当前状态下的值的长度
// memo: 用于记忆化搜索的缓存，避免在单次求值中重复计算
long long get_variable_length(const std::string& var_name, std::map<std::string, long long>& memo) {
    // 1. 检查缓存中是否已有结果
    if (memo.count(var_name)) {
        return memo[var_name];
    }

    // 2. 检查变量是否被定义过
    if (!variables.count(var_name)) {
        // 未定义的变量视为空字符串，长度为 0
        return memo[var_name] = 0;
    }

    const auto& info = variables.at(var_name);

    // 3. 根据变量类型进行计算
    if (info.type == 1) { // 类型 1: 直接赋值
        // 直接返回存储的长度
        return memo[var_name] = info.length;
    } else if (info.type == 2) { // 类型 2: 间接赋值
        // 需要实时计算其表达式的长度
        long long current_total_length = 0;
        for (const auto& op : info.expression) {
            if (op.is_variable) {
                // 如果操作数是变量，递归调用本函数计算其长度
                current_total_length = (current_total_length + get_variable_length(op.value, memo)) % MOD;
            } else {
                // 如果是字面字符串，直接加上其长度
                current_total_length = (current_total_length + op.value.length()) % MOD;
            }
        }
        return memo[var_name] = current_total_length;
    }

    // 默认情况（未赋值的变量），长度为 0
    return memo[var_name] = 0;
}

int main() {
    // 提高C++ I/O效率
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n;
    std::cin >> n;
    std::string line;
    std::getline(std::cin, line); // 消耗掉第一行末尾的换行符

    for (int i = 0; i < n; ++i) {
        std::getline(std::cin, line);
        std::stringstream ss(line); // 使用 stringstream 方便地按空格分割

        int type;
        ss >> type;

        if (type == 1) { // 语句类型 1: 直接赋值
            std::string var_name;
            ss >> var_name;

            long long total_expr_length = 0;
            std::string token;

            // 为本次求值创建一个新的缓存
            std::map<std::string, long long> memo;

            // 遍历表达式中的所有操作数
            while (ss >> token) {
                if (token[0] == '$') {
                    std::string dependent_var_name = token.substr(1);
                    total_expr_length = (total_expr_length + get_variable_length(dependent_var_name, memo)) % MOD;
                } else {
                    total_expr_length = (total_expr_length + token.length()) % MOD;
                }
            }

            // 更新变量信息
            variables[var_name].type = 1;
            variables[var_name].length = total_expr_length;
            variables[var_name].expression.clear(); // 清空可能存在的旧表达式

        } else if (type == 2) { // 语句类型 2: 间接赋值
            std::string var_name;
            ss >> var_name;

            // 更新变量信息，存储表达式
            variables[var_name].type = 2;
            variables[var_name].length = 0; // 类型2不直接使用长度
            variables[var_name].expression.clear();

            std::string token;
            while (ss >> token) {
                Operand op;
                if (token[0] == '$') {
                    op.is_variable = true;
                    op.value = token.substr(1);
                } else {
                    op.is_variable = false;
                    op.value = token;
                }
                variables[var_name].expression.push_back(op);
            }

        } else if (type == 3) { // 语句类型 3: 输出
            std::string var_name;
            ss >> var_name;

            // 为本次输出创建一个新的缓存
            std::map<std::string, long long> memo;
            std::cout << get_variable_length(var_name, memo) << "\n";
        }
    }

    return 0;
}
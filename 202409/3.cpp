#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <climits>

using namespace std;

// 当补丁损坏时，调用此函数输出错误信息并退出程序
void fail() {
    cout << "Patch is damaged." << endl;
    exit(0);
}

// 定义一个结构体来存储每个补丁块的信息
struct Block {
    long long NN, MM, nn, mm;      // 从 @@ 行解析出的四个整数
    vector<string> original_fragment; // 由 '-' 和 ' ' 行组成的原始文件片段
    vector<string> new_fragment;      // 由 '+' 和 ' ' 行组成的新文件片段
};

int main() {
    // 加速 C++ 的 I/O 操作
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // --- 1. 读取输入 ---
    string line;
    getline(cin, line);
    int n = stoi(line); // 读取原始文件行数

    vector<string> original_file(n);
    for (int i = 0; i < n; ++i) {
        getline(cin, original_file[i]); // 读取原始文件内容
    }

    vector<string> patch_input;
    while (getline(cin, line)) {
        patch_input.push_back(line); // 读取所有补丁内容
    }

    // --- 2. 补丁预处理与基本验证 ---
    // 规则：如果补丁内容不为空，则必须至少包含一个 '@@' 开头的行
    bool has_block_header = false;
    for (const auto& l : patch_input) {
        if (l.rfind("@@", 0) == 0) { // 检查字符串是否以 "@@" 开头
            has_block_header = true;
            break;
        }
    }

    if (!patch_input.empty() && !has_block_header) {
        fail();
    }
    
    if (patch_input.empty()) { // 如果没有补丁，直接输出原文件
        for (const auto& l : original_file) {
            cout << l << endl;
        }
        return 0;
    }

    // 移除注释行 (以 '#' 开头)
    vector<string> processed_patch;
    for (const auto& l : patch_input) {
        if (l.empty() || l[0] != '#') {
            processed_patch.push_back(l);
        }
    }

    // 找到所有块的起始位置 (以 '@@' 开头的行)
    vector<int> block_starts;
    for (int i = 0; i < processed_patch.size(); ++i) {
        if (processed_patch[i].rfind("@@", 0) == 0) {
            block_starts.push_back(i);
        }
    }
    
    // 如果移除注释后没有块了，也视为损坏
    if (block_starts.empty()) {
        fail();
    }
    
    // --- 3. 解析与验证每个块 (第一轮) ---
    vector<Block> blocks;
    long long prev_NN_plus_MM = 1; // 用于检查块之间是否重叠，从第一行开始

    for (size_t i = 0; i < block_starts.size(); ++i) {
        Block current_block;
        int header_idx = block_starts[i];
        string header_line = processed_patch[header_idx];

        // a. 解析块头
        long long temp_NN, temp_MM, temp_nn, temp_mm;
        int chars_scanned = 0; // 用来存储被 sscanf 处理的字符数

        // 在格式字符串末尾加上 %n
        int assigned_count = sscanf(header_line.c_str(), "@@ -%lld,%lld +%lld,%lld @@%n", 
                                    &temp_NN, &temp_MM, &temp_nn, &temp_mm, &chars_scanned);

        // 检查：
        // 1. 是否成功赋值了 4 个变量
        // 2. 扫描过的字符数是否等于原始字符串的总长度
        // 3. 解析出的数字是否都为正数
        if (assigned_count != 4 || 
            chars_scanned != header_line.length() ||
            temp_NN <= 0 || temp_MM <= 0 || temp_nn <= 0 || temp_mm <= 0) 
        {
            fail();
        }

        current_block.NN = temp_NN;
        current_block.MM = temp_MM;
        current_block.nn = temp_nn;
        current_block.mm = temp_mm;

        // b. 检查块声明的起始行号是否与前一个块重叠
        if (current_block.NN < prev_NN_plus_MM) {
            fail();
        }

        // c. 解析块内容
        int content_start_idx = header_idx + 1;
        int content_end_idx = (i + 1 < block_starts.size()) ? block_starts[i + 1] : processed_patch.size();

        for (int j = content_start_idx; j < content_end_idx; ++j) {
            const string& content_line = processed_patch[j];
            if (content_line.empty()) fail(); // 块内容不允许有空行

            char type = content_line[0];
            string text = content_line.substr(1);

            if (type == '-') {
                current_block.original_fragment.push_back(text);
            } else if (type == '+') {
                current_block.new_fragment.push_back(text);
            } else if (type == ' ') {
                current_block.original_fragment.push_back(text);
                current_block.new_fragment.push_back(text);
            } else {
                fail(); // 非法行起始符
            }
        }

        // d. 检查解析出的内容行数是否与块头声明一致
        if (current_block.original_fragment.size() != current_block.MM) {
            fail();
        }
        if (current_block.new_fragment.size() != current_block.mm) {
            fail();
        }
        
        blocks.push_back(current_block);
        prev_NN_plus_MM = current_block.NN + current_block.MM;
    }

    // --- 4. 查找偏移(delta)并更新块信息 (第二轮) ---
    long long last_applied_end_line = 1; // 1-indexed

    for (size_t i = 0; i < blocks.size(); ++i) {
        Block& current_block = blocks[i];
        
        long long best_delta = LLONG_MAX;
        long long min_abs_delta = LLONG_MAX;

        // 搜索最佳偏移 delta, abs(delta) < MM
        for (long long delta = -(current_block.MM - 1); delta <= (current_block.MM - 1); ++delta) {
             if (abs(delta) >= current_block.MM) continue;
             
             long long start_line = current_block.NN + delta;
             
             // 检查应用补丁的区域是否合法
             if (start_line < 1) continue;
             if (start_line + current_block.MM - 1 > n) continue;
             if (start_line < last_applied_end_line) continue;

             // 检查内容是否完全匹配
             bool match_found = true;
             for (long long k = 0; k < current_block.MM; ++k) {
                 if (original_file[start_line - 1 + k] != current_block.original_fragment[k]) {
                     match_found = false;
                     break;
                 }
             }

             // 如果匹配，根据规则更新最佳delta
             if (match_found) {
                 if (abs(delta) < min_abs_delta) {
                     min_abs_delta = abs(delta);
                     best_delta = delta;
                 } else if (abs(delta) == min_abs_delta) {
                     best_delta = min(best_delta, delta);
                 }
             }
        }
        
        // 如果遍历完所有可能的delta都找不到匹配，则补丁损坏
        if (best_delta == LLONG_MAX) {
            fail();
        }

        // 将找到的最佳delta应用到当前及所有后续块的 NN 上
        for (size_t j = i; j < blocks.size(); ++j) {
            blocks[j].NN += best_delta;
        }

        // 更新下一个块允许的最小起始行
        last_applied_end_line = blocks[i].NN + blocks[i].MM;
    }

    // --- 5. 生成最终输出文件 ---
    vector<string> result_file;
    int original_idx = 0; // 0-indexed, 指向当前处理到的原始文件行

    for (const auto& block : blocks) {
        // 将当前块之前未被修改的行添加到结果中
        long long block_start_idx = block.NN - 1;
        while (original_idx < block_start_idx) {
            if (original_idx >= n) break; // 安全检查
            result_file.push_back(original_file[original_idx]);
            original_idx++;
        }

        // 将块的新内容添加到结果中
        for (const auto& l : block.new_fragment) {
            result_file.push_back(l);
        }

        // 跳过原始文件中被替换掉的行
        original_idx += block.MM;
    }

    // 将最后一个块之后所有剩余的行添加到结果中
    while (original_idx < n) {
        result_file.push_back(original_file[original_idx]);
        original_idx++;
    }

    // 打印最终生成的文件
    for (const auto& l : result_file) {
        cout << l << endl;
    }

    return 0;
}
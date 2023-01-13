/*
202112-2 序列查询新解
得分：100

不能从1到1e9枚举x，否则超时。只能枚举哪些f_i g_i变化的点
*/

#include <bits/stdc++.h>
using namespace std;

struct compare { // 函数对象
    bool operator()(const int& a, const int& b)const { // set要求此处为常类型
        return a < b; // 从小到大排列
    }
};

int main()
{
    int n, N;
    vector<int> A = { 0 };
    int a;
    cin >> n >> N;
    for (int i = 1; i <= n; ++i) {
        cin >> a;
        A.push_back(a);
    }
    int r = N / (n + 1);   
    /*把f_i g_i改变的点作为间隔点排序存起来*/
    set<int, compare> change_pos_f, change_pos_g, change_pos_all;
    for (int i = 1; i <= n; ++i) {
        change_pos_f.insert(A[i]);
        change_pos_all.insert(A[i]);
    }
    for (int i = r; i < N; i = i + r) {
        change_pos_g.insert(i);
        change_pos_all.insert(i);
    }
    /*增序遍历所有间隔点*/
    long long int error = 0;
    int pos = 0, f_i = 0, g_i = 0;
    for (auto it = change_pos_all.begin(); it != change_pos_all.end(); it++) {
        int next_pos = *it;   
        error += (next_pos - pos) * abs(g_i - f_i);
        pos = next_pos;
        if (change_pos_f.count(next_pos) > 0) { // 该间隔点在A中，需要f_i++
            f_i++;
        }
        if (change_pos_g.count(next_pos) > 0) { // 该间隔点可被r整除，需要g_i++
            g_i++;
        }     
        if (++it == change_pos_all.end()) {
            error += (N - pos) * abs(g_i - f_i); // 所有间隔点之后的error
        }
        it--;     
    }
    cout << error;
    return 0;
}


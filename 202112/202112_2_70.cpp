/*
202112-2 序列查询新解
得分：70 (运行超时，因为是从1到1e9枚举的)
*/

#include <bits/stdc++.h>
using namespace std;

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
    int f_i = 0;
    long long int error = 0;
    for (int i = 0; i < N; ++i) {
        int g_i = i / r;
        error += abs(g_i - f_i);
        if (A[f_i + 1] == i + 1 && f_i < n) { // 此时f_i需更新至增大一位（对应下一次迭代的正确的f_i）
            f_i++;
        }
    }
    cout << error;
    return 0;
}


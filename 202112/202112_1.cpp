/*
202112-1 序列查询
得分：100
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
    int sum = 0;
    for (int i = 1; i <= n; ++i) {
        if (i < n) {
            sum += i * (A[i + 1] - A[i]);
        }
        else {
            sum += i * (N - A[i]);
        }
    }
    cout << sum;
    return 0;
}

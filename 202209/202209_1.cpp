/*
CSP202209_1 如此编码
得分：100
*/
#include <bits/stdc++.h>
using namespace std;

int main()
{
    int n, m;
    cin >> n >> m;
    vector<int> a(n + 1), b(n + 1), c(n + 1);
    c[0] = 1;//初始化c[0]
    for (int i = 1; i <= n; i++)
    {
        cin >> a[i];
        c[i] = c[i - 1] * a[i];//根据定义求c[i]
    }

    for (int i = n; i >= 1; i--)
    {
        b[i] = m / c[i - 1];//b[i]为能选择的c[i-1]最大个数
        m -= b[i] * c[i - 1];
    }
    for (int i = 1; i <= n; i++)
    {
        cout << b[i] << " ";
    }
    return 0;
}

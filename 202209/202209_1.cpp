/*
CSP202209_1 如此编码
得分：100
*/
#include <iostream>
#include <algorithm>
using namespace std;
const int N = 30;
int n, m;
int a[N], b[N], c[N];//按照题目要求设置
int main()
{
    cin >> n >> m;
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

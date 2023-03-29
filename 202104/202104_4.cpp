/*
CCF202104-4 校门外的树
得分：100

令f[1,i]为在障碍物(a1,ai)的区间内种树的方案数，考虑将其划分的最后一个子区间为(aj,ai)，令F(j,i)为其中的合法方案数，则有DP递归表达式：f[1,i]=sum_{1<=j<i}f[1,j]*F(j,i)，其中f[1,1]=1.
最终解：f[1,n]/MOD

难点1：列出上述递归表达式
难点2：利用打表法快速地求F(j,i) for every 1<=j<i

解难点2的细节：首先列出针对每一个区间长度的约数集合。对给定的i，按递减的顺序枚举[1,i)里面的每一个j，
查看ai-aj区间长度的所有约数，如果一个约数没有被之前枚举的j用到过，则该约数对应ai-aj区间的一种可行方案；
如果该约数被之前枚举的j用到过，则使用该约数会在之前枚举的j障碍处种树，故该约数不对应ai-aj区间的一种可行方案。基于上述原理快速计算F(j,i)。
*/
#include <bits/stdc++.h>
using namespace std;

const int N = 1001;
const int M = 100001;
const int MOD = 1e9 + 7;
vector<int> a(N);   // a[i]表示障碍物i坐标
vector<int> f_mod(N);   // f_mod[i]表示截至第i个障碍物的方案数对MOD取模的结果，即f[1,i]%MOD，基于DP递归表达式有f_mod[i]=sum_{1<=j<i}f_mod[j]*F(j,i) % MOD，其中f_mod[1]=1.
vector<bool> s(M);   // s[i]表示是否使用过约数i
vector<vector<int>> cnt(M); // cnt[i]表示i的约数集合（不包括i本身）

void init() { // 求约数集合（打表）
    for (int i = 1; i < M; i++) {
        for (int j = 2 * i; j < M; j += i) {
            cnt[j].push_back(i);
        }
    }
}

int main() {
    init(); // 求约数集合
    int n;
    cin >> n; // 障碍物数量
    for (int i = 1; i <= n; i++) {
        cin >> a[i]; // a[i]表示障碍物i坐标（i从1开始）
    }

    f_mod[1] = 1; 
    for (int i = 2; i <= n; i++) { // 自底向上求f_mod[i]
        /*vector:: assign() is an STL in C++ which assigns new values to the vector 
        elements by replacing old ones. It can also modify the size of the vector if necessary.
        
        vectorname.assign(int size, int value)
        Parameters:
        size - number of values to be assigned
        value - value to be assigned to the vectorname
        */
        s.assign(M, 0); // 把s中的元素赋值为0

        for (int j = i - 1; j >= 1; j--) { // 难点2：快速计算F(j,i)
            int d = a[i] - a[j]; // 区间长度
            long long int num = 0; // F(j,i)； long long类型，不溢出
            for (int k : cnt[d]) { // 枚举d长度的每一个约数
                if (s[k]) continue; // 该约数已被使用过，说明使用该约数会在(j,i)之间的某个障碍物处种树，不可行
                s[k] = true; // 可行方案，更新s
                num++;
            }
            s[d] = true; // 区间长度也要标记（不能使用区间长度的约数种树，即此处不可行）
            f_mod[i] = (f_mod[i] + f_mod[j] * num) % MOD; // num类似为long long int，保证不溢出
        }
    }
    cout << f_mod[n] << endl;
    return 0;
}
#include <bits/stdc++.h>
using namespace std;

/*
i: 0...2^2-1

0: 0 0
1: 0 1
2: 1 0
3: 1 1



*/

int main()
{
    for (int i = 0; i < (1 << 2); i++) { // 2^2 choices
        for (int j = 0; j < 2; j++) { // 每个choice的两位（每一位是0或1）
            if (i & (1 << j)) { // 位运算
                cout << 1 << " ";
            }
            else {
                cout << 0 << " ";
            }
        }
        cout << endl;
    }
}

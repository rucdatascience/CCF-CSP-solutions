#include <bits/stdc++.h>
#define LL long long
const int kN = 2e5 + 10;

int n, a[kN], k[kN], f[kN];


//先后退 再前进算一步
int main()
{
    std::ios::sync_with_stdio(0), std::cin.tie(0), std::cout.tie(0);
    std::cin >> n;
    for (int i = 1; i <= n; ++i)
        std::cin >> a[i], f[i] = -1;
    for (int i = 1; i <= n; ++i)
        std::cin >> k[i];

    int R = 1;
    f[1] = 0; 
    /*
    考虑 i—>j->i   
    第一次到达i需要x步

    从i跳到j需要1步，总共x+1步
    
    从j跳回i又至少1步，总共x+2步
    
    这显然比第一次到达i时的x步更差*/
    
    

    //先后退 再前进算一步！！！
    for (int i = 1; i <= n; ++i)
    {
        if (f[i] == -1)
            break;
        for (int j = R + 1; j <= std::min(n, i - a[i] + k[i - a[i]]);++j)//i-a[i]是我跳到i后后退到达的格子 k[i - a[i]]是我能再前进的最远格子
        {
            f[j] = f[i] + 1;
            R = j;
        }
    }

    std::cout << f[n] << "\n";
    return 0;
}
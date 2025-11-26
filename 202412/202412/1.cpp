#include <bits/stdc++.h>
#define IOS ios::sync_with_stdio(0), cin.tie(0)
#define int long long
#define endl '\n'
#define lowbit(x) ((x) & (-x))
#define all(s) s.begin(), s.end()
#define pii pair<int, int>
#define ls(x) (x << 1)
#define rs(x) (x << 1 | 1)
using namespace std;
template <class T>
inline void read(T &x)
{
    x = 0;
    char c = getchar();
    bool f = 0;
    for (; !isdigit(c); c = getchar())
        f ^= (c == '-');
    for (; isdigit(c); c = getchar())
        x = (x << 3) + (x << 1) + (c ^ 48);
    x = f ? -x : x;
}
template <class T>
inline void write(T &x)
{
    if (x < 0)
        putchar('-'), x = -x;
    if (x < 10)
        putchar(x + 48);
    else
        write(x / 10), putchar(x % 10 + 48);
}
signed main()
{
    IOS;
    int n, k;
    cin >> n >> k;
    while (k--)
    {
        int x, y;
        cin >> x >> y;
        string a;
        cin>>a;
        for(auto z:a){
            if(z=='f')y=min(y+1,n);
            if(z=='b')y=max(y-1,1ll);
            if(z=='r')x=min(x+1,n);
            if(z=='l')x=max(x-1,1ll);
        }
        cout<<x<<" "<<y<<endl;
    }
}
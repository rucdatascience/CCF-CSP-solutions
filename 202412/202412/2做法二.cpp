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
int n;
int a[200005] = {0};
int b[200005] = {0};
int c[100005] = {0};
int pre_max[100005] = {0}; // c的前缀最大值
int suf_max[100005] = {0}; // c的后缀最大值
int ans[100005] = {0};
int now[100005] = {0};

signed main()
{
    IOS;
    int n;
    cin >> n;
    for (int i = 0; i <= n; i++)
        cin >> a[i];
    for (int i = 1; i <= n; i++)
        cin >> b[i];
    c[0] = a[0];
    for (int i = 1; i <= n; i++)
    {
        c[i] = c[i - 1] + a[i] - b[i];
    }
    pre_max[0] = c[0];//pre_max 是c的前缀最大值
    for (int i = 1; i <= n; i++)
    {
        pre_max[i] = max(pre_max[i - 1], c[i]);
    }
    suf_max[n] = c[n];//suf_max 是c的后缀最大值
    for (int i = n - 1; i >= 0; i--)
    {
        suf_max[i] = max(suf_max[i + 1], c[i]);
    }
    for (int i = 1; i <= n; i++)
    {
        ans[i] = max(pre_max[i - 1], suf_max[i] + b[i]);
    }
    for (int i = 1; i <= n; i++)
    {
        cout << ans[i] << " ";
    }
}
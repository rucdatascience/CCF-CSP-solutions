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
int sum[100005] = {0};
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
    sum[1] = -a[0];
    for (int i = 2; i <= n; i++)
    {
        sum[i] = sum[i - 1] + b[i - 1] - a[i - 1];
    }
    sum[0] = sum[n] + b[n] - a[n];
    now[n] = sum[0];
    for (int i = n - 1; i >= 1; i--)
    {
        now[i] = min(now[i + 1], sum[i + 1]);
    }
    int mx = 1e18;
    for (int i = 0; i <= n; i++)
        mx = min(mx, sum[i]);
    for (int i = 1; i <= n; i++)
    {
        ans[i] = max(0ll, -min(mx, now[i] - b[i]));
    }
    for (int i = 1; i <= n; i++)
    {
        cout << ans[i] << " ";
    }
    //  cout<<max(0ll,-mx)<<endl;
}
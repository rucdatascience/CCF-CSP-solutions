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
set<int> se[70000]; // huancunzu  li mian you shenme you ji ge dongxi

priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq[70000]; // shunxu
map<int, int> cnt;
map<int, int> change;
signed main()
{
    IOS;
    int n, N, q;
    cin >> n >> N >> q;
    int idx = 0;
    while (q--)
    { /// int idx=q+1;
        idx++;
        int op;
        int a;
        cin >> op >> a;
        int zu = (a / n) % N;
        if (op == 0)
        {
            if (!se[zu].count(a) && se[zu].size() < n) // have extra place
            {
                se[zu].insert(a);
                pq[zu].push({idx, a});
                cnt[a]++;
                cout << "0 " << a << endl;
                continue;
            }
            else if (!se[zu].count(a) && se[zu].size() == n) // don"t have extra place
            {
                while (cnt[pq[zu].top().second] != 1)
                {
                    cnt[pq[zu].top().second]--;
                    pq[zu].pop();
                }
                auto now = pq[zu].top();
                pq[zu].pop();
                cnt[now.second]--;
                se[zu].erase(now.second);
                if (change[now.second] == 1)
                {
                    cout << "1 " << now.second << endl;
                    change[now.second] = 0;
                }
                se[zu].insert(a);
                pq[zu].push({idx, a});
                cnt[a]++;
                cout << "0 " << a << endl;
                continue;
            }
            else
            {
                pq[zu].push({idx, a});
                cnt[a]++;
                continue;
            }
        }
        if (op == 1)
        {
            if (!se[zu].count(a) && se[zu].size() < n) // have extra place
            {
                se[zu].insert(a);
                pq[zu].push({idx, a});
                cnt[a]++;
                cout << "0 " << a << endl;
                change[a] = 1;
                continue;
            }
            else if (!se[zu].count(a) && se[zu].size() == n) // don"t have extra place
            {
                while (cnt[pq[zu].top().second] != 1)
                {
                    cnt[pq[zu].top().second]--;
                    pq[zu].pop();
                }
                auto now = pq[zu].top();
                pq[zu].pop();
                cnt[now.second]--;
                se[zu].erase(now.second);
                if (change[now.second] == 1)
                {
                    cout << "1 " << now.second << endl;
                    change[now.second] = 0;
                }
                se[zu].insert(a);
                pq[zu].push({idx, a});
                cnt[a]++;
                cout << "0 " << a << endl;
                change[a] = 1;
                continue;
            }
            else
            {
                pq[zu].push({idx, a});
                cnt[a]++;
                change[a] = 1;
                continue;
            }
        }
    }
}
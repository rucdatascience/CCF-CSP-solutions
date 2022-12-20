/* CCF202104-1 灰度直方图 */
#include <bits/stdc++.h>
using namespace std;

const int L = 256;
int cnt[L];

int main()
{
    std::ios::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);

    memset(cnt, 0, sizeof cnt);

    int n, m, l;
    cin >> n >> m >> l;
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= m; j++) {
            int x;
            cin >> x;
            cnt[x]++;
        }

    for (int i = 0; i < l; i++)
        cout << cnt[i] << ' ';
    cout << endl;

    return 0;
}

/*
CSP202109_3 脉冲神经网络
得分：100
*/
#include <bits/stdc++.h>
using namespace std;

const int N = 2010;
const double INF = 1e8;

int n, s, p, T;
double dt;
int h[N], e[N], D[N], ne[N], idx;
double W[N], v[N], u[N], a[N], b[N], c[N], d[N];
int r[N], cnt[N];
double I[1024][N / 2];

static unsigned long _next = 1;

/* RAND_MAX assumed to be 32767 */
int myrand(void) {
    _next = _next * 1103515245 + 12345;
    return((unsigned)(_next / 65536) % 32768);
}

void add(int a, int b, double c, int d)
{
    e[idx] = b, W[idx] = c, D[idx] = d, ne[idx] = h[a], h[a] = idx++;
}

int main()
{
    memset(h, -1, sizeof h);
    scanf("%d%d%d%d", &n, &s, &p, &T);
    scanf("%lf", &dt);
    for (int i = 0; i < n;)
    {
        int rn;
        scanf("%d", &rn);
        double vv, uu, aa, bb, cc, dd;
        scanf("%lf%lf%lf%lf%lf%lf", &vv, &uu, &aa, &bb, &cc, &dd);
        for (int j = 0; j < rn; j++, i++)
        {
            v[i] = vv, u[i] = uu, a[i] = aa, b[i] = bb, c[i] = cc, d[i] = dd;
        }
    }

    for (int i = n; i < n + p; i++) scanf("%d", &r[i]);

    int mod = 0;
    while (s--)
    {
        int a, b, d;
        double c;
        scanf("%d%d%lf%d", &a, &b, &c, &d);
        add(a, b, c, d);
        mod = max(mod, d + 1);
    }

    for (int i = 0; i < T; i++)
    {
        int t = i % mod;
        for (int j = n; j < n + p; j++)
            if (r[j] > myrand())
            {
                for (int k = h[j]; ~k; k = ne[k])
                {
                    int x = e[k];
                    I[(t + D[k]) % mod][x] += W[k];
                }
            }

        for (int j = 0; j < n; j++)
        {
            double vv = v[j], uu = u[j];
            v[j] = vv + dt * (0.04 * vv * vv + 5 * vv + 140 - uu) + I[t][j];
            u[j] = uu + dt * a[j] * (b[j] * vv - uu);

            if (v[j] >= 30)
            {
                for (int k = h[j]; ~k; k = ne[k])
                {
                    int x = e[k];
                    I[(t + D[k]) % mod][x] += W[k];
                }
                cnt[j] ++;
                v[j] = c[j], u[j] += d[j];
            }
        }

        memset(I[t], 0, sizeof I[t]);
    }

    double minv = INF, maxv = -INF;
    int minc = INF, maxc = -INF;

    for (int i = 0; i < n; i++)
    {
        minv = min(minv, v[i]);
        maxv = max(maxv, v[i]);
        minc = min(minc, cnt[i]);
        maxc = max(maxc, cnt[i]);
    }

    printf("%.3lf %.3lf\n", minv, maxv);
    printf("%d %d\n", minc, maxc);

    return 0;
}
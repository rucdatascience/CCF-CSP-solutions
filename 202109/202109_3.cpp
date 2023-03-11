/*
CSP202109_3 脉冲神经网络
得分：100

http://www.manongjc.com/detail/32-jplehuuexcqszce.html

https://blog.csdn.net/weixin_51275821/article/details/124170987
*/
#include <bits/stdc++.h>
using namespace std;

const int maxN = 2010;
const double INF = 1e8;

int N, S, P, T;
double dt;
int h[maxN], e[maxN], D[maxN], ne[maxN], idx = 0;
double W[maxN], v[maxN], u[maxN], a[maxN], b[maxN], c[maxN], d[maxN];
int r[maxN], cnt[maxN];
double I[1024][maxN / 2];

static unsigned long _next = 1;

/* RAND_MAX assumed to be 32767 */
int myrand(void) {
    _next = _next * 1103515245 + 12345;
    return((unsigned)(_next / 65536) % 32768);
}

int main()
{
    memset(h, -1, sizeof h);

    /*input*/
    cin >> N >> S >> P >> T;
    cin >> dt;
    for (int i = 0; i < N;)
    {
        int rn;
        cin >> rn;
        double vv, uu, aa, bb, cc, dd;
        cin >> vv >> uu >> aa >> bb >> cc >> dd;
        for (int j = 0; j < rn; j++, i++)
        {
            v[i] = vv, u[i] = uu, a[i] = aa, b[i] = bb, c[i] = cc, d[i] = dd;
        }
    }
    for (int i = N; i < N + P; i++) {
        cin >> r[i];
    }
    int mod = 0;
    for (int i = 0; i < S; i++) {
        int a, b, d;
        double c;
        cin >> a >> b >> c >> d;
        e[idx] = b, W[idx] = c, D[idx] = d, ne[idx] = h[a], h[a] = idx++;
        mod = max(mod, d + 1);
    }

    for (int i = 0; i < T; i++)
    {
        int t = i % mod;
        for (int j = N; j < N + P; j++)
            if (r[j] > myrand())
            {
                for (int k = h[j]; ~k; k = ne[k]) // ~是位运算的取反运算符
                {
                    int x = e[k];
                    I[(t + D[k]) % mod][x] += W[k];
                }
            }

        for (int j = 0; j < N; j++)
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

    for (int i = 0; i < N; i++)
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
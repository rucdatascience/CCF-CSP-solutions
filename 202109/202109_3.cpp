/*
CSP202109_3 脉冲神经网络
得分：100 （使用编译环境2满分，使用编译环境2、3超时）

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
double I[1024][maxN / 2]; // N*D的滚动数组

static unsigned long _next = 1;

/* RAND_MAX assumed to be 32767 */
int myrand(void) {
    _next = _next * 1103515245 + 12345;
    return((unsigned)(_next / 65536) % 32768);
}

int main()
{
    ios::sync_with_stdio(false), cin.tie(0), cout.tie(0);
    memset(h, -1, sizeof h); //h初值-1

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
        /*
        a是入结点，e[idx]表示这个突触连接的出结点b，W[idx]与D[idx]表示这个突触的脉冲强度和传播延迟，h[a]表示a向后的突触ID（idx），
        因为a后可能有多个突触，使用ne[idx]存储上一个a后的突触ID，这样使用递归方式（直到ID变为-1）可以查询所有a后的突触ID
        */
        mod = max(mod, d + 1); // mod为最大的传播延迟+1
    }

    for (int i = 0; i < T; i++)
    {
        int t = i % mod; //利用对mod的取余来使用滚动数组I
        for (int j = N; j < N + P; j++)
            if (r[j] > myrand()) //脉冲源在每个时刻以一定的概率发放一个脉冲
            {
                for (int k = h[j]; ~k; k = ne[k]) // k为j后的所有突触，~是位运算的取反运算符(-1的取反是0)
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
                cnt[j] ++; //cnt存储神经元的脉冲次数
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
#include <iostream>
#include <vector>

using namespace std;

const int MOD = 998244353;
const int MAXN = 10000005;

bool is_prime[MAXN];
int primes[MAXN / 10];
int pk[MAXN];

int f0_arr[MAXN], F1_arr[MAXN], F3_arr[MAXN], F4_arr[MAXN], F5_arr[MAXN];
int prime_cnt = 0;

void calc_all(long long p, int k, int &f0, int &F1, int &F3, int &F4, int &F5) {
    long long p_pow[100];
    p_pow[0] = 1;
    for (int i = 1; i <= 3 * k; ++i) p_pow[i] = (p_pow[i - 1] * p) % MOD;

    long long sum_f0 = 0;
    for (int c = 0; c <= 2 * k; ++c) {
        for (int e = 0; e <= 2 * k; ++e) {
            if (c + e <= 3 * k) sum_f0 = (sum_f0 + p_pow[3 * k - c - e]) % MOD;
        }
    }
    f0 = sum_f0;

    long long sum_F1 = 0;
    for (int i = 0; i <= 2 * k; ++i) sum_F1 = (sum_F1 + p_pow[i]) % MOD;
    F1 = sum_F1;

    long long sum_F3 = 0;
    for (int c = 0; c <= (3 * k) / 2; ++c) sum_F3 = (sum_F3 + p_pow[3 * k - 2 * c]) % MOD;
    F3 = sum_F3;

    F4 = (p_pow[k] * (2 * k + 1)) % MOD;

    long long sum_F5 = 0;
    for (int c = (k + 1) / 2; c <= (3 * k) / 2; ++c) sum_F5 = (sum_F5 + p_pow[c]) % MOD;
    F5 = sum_F5;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int op, n;
    if (!(cin >> op >> n)) return 0;

    fill(is_prime + 2, is_prime + n + 1, true);
    f0_arr[1] = F1_arr[1] = F3_arr[1] = F4_arr[1] = F5_arr[1] = 1;

    for (int i = 2; i <= n; ++i) {
        if (is_prime[i]) {
            primes[prime_cnt++] = i;
            pk[i] = i;
            calc_all(i, 1, f0_arr[i], F1_arr[i], F3_arr[i], F4_arr[i], F5_arr[i]);
        }
        for (int j = 0; j < prime_cnt; ++j) {
            int p = primes[j];
            if (p * i > n) break;
            is_prime[p * i] = false;
            if (i % p == 0) {
                pk[p * i] = pk[i] * p;
                if (i == pk[i]) {
                    int k = 0, temp = p * i;
                    while (temp % p == 0) { temp /= p; k++; }
                    calc_all(p, k, f0_arr[p * i], F1_arr[p * i], F3_arr[p * i], F4_arr[p * i], F5_arr[p * i]);
                } else {
                    int a = i / pk[i], b = pk[i] * p;
                    f0_arr[p * i] = (1LL * f0_arr[a] * f0_arr[b]) % MOD;
                    F1_arr[p * i] = (1LL * F1_arr[a] * F1_arr[b]) % MOD;
                    F3_arr[p * i] = (1LL * F3_arr[a] * F3_arr[b]) % MOD;
                    F4_arr[p * i] = (1LL * F4_arr[a] * F4_arr[b]) % MOD;
                    F5_arr[p * i] = (1LL * F5_arr[a] * F5_arr[b]) % MOD;
                }
                break;
            } else {
                pk[p * i] = p;
                f0_arr[p * i] = (1LL * f0_arr[i] * f0_arr[p]) % MOD;
                F1_arr[p * i] = (1LL * F1_arr[i] * F1_arr[p]) % MOD;
                F3_arr[p * i] = (1LL * F3_arr[i] * F3_arr[p]) % MOD;
                F4_arr[p * i] = (1LL * F4_arr[i] * F4_arr[p]) % MOD;
                F5_arr[p * i] = (1LL * F5_arr[i] * F5_arr[p]) % MOD;
            }
        }
    }

    long long ans = 0;
    for (int i = 1; i <= n; ++i) {
        if (op == 0) {
            ans = (ans + f0_arr[i]) % MOD;
        } else {
            long long bad = (2LL * F1_arr[i] + F3_arr[i] + F4_arr[i] + 2LL * F5_arr[i] - 5LL * i) % MOD;
            long long perf = (f0_arr[i] - bad) % MOD;
            ans = (ans + perf) % MOD;
        }
    }
    cout << (ans % MOD + MOD) % MOD << "\n";
    return 0;
}

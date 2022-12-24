/*
CSP202209_2 何以包邮？
得分：100
*/
# include <iostream>
# include <algorithm>
# define maxn 300010
using namespace std;
int n, x;
int v[40] = { 0 };
int f[40][maxn] = { {0} };
int main()
{
	cin >> n >> x;
	int sum = 0;
	for (int i = 1; i <= n; i++) {
		cin >> v[i];
		sum += v[i];
	}
	int y = sum - x;
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= y; j++) {
			f[i][j] = f[i - 1][j];//背包问题的核心公式
			if (j >= v[i]) {
				f[i][j] = max(f[i][j], f[i - 1][j - v[i]] + v[i]);//背包问题的核心公式
			}
		}
	}
	int r = sum - f[n][y];
	cout << r << endl;
	return 0;
}
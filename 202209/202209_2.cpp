/*
CSP202209_2 何以包邮？
得分：100

本问题是0-1背包问题的反问题的一个特例：删去的cost不超过m-x的情况下（未删去的cost超过x），最大化删去的cost
*/
#include <bits/stdc++.h>
using namespace std;

int main()
{
	int n, x;
	cin >> n >> x;

	vector<int> cost(n + 1);
	int m = 0;
	for (int i = 1; i <= n; i++) {
		cin >> cost[i];
		m += cost[i];
	}

	/*背包问题动态规划	*/
	vector<vector<int>> f(n + 1);
	int y = m - x; // 可删去的cost的上限
	for (int i = 0; i < n + 1; i++) {
		f[i].resize(y + 1, 0); // f[i][j]指在从a0到ai中删去的cost不超过j的情况下，最大的可删去的cost
	}
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= y; j++) {
			if (j >= cost[i]) {
				f[i][j] = max(f[i - 1][j], f[i - 1][j - cost[i]] + cost[i]);//背包问题的核心公式
			}
			else {
				f[i][j] = f[i - 1][j];//背包问题的核心公式
			}
		}
	}
	cout << m - f[n][y] << endl;
	return 0;
}
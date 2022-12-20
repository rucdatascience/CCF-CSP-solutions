/*
CCF202104-2 邻域均值

得分: 100
*/
#include <bits/stdc++.h>
using namespace std;

int main()
{
	int n, L, r, t;
	cin >> n >> L >> r >> t;
	vector<vector<int>> sum(n + 1); // 矩阵A的二维前缀和 
	for (int i = 0; i < n + 1; i++) {
		sum[i].resize(n + 1, 0);
	}

	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= n; j++) {
			int x;
			cin >> x;
			sum[i][j] = sum[i - 1][j] + sum[i][j - 1] - sum[i - 1][j - 1] + x; // 0行和0列的sum值为0不影响正确性
		}
	}

	int q = 0;
	for (int i = 1; i <= n; i++)
		for (int j = 1; j <= n; j++) {
			int column1 = max(i - r, 1), column2 = min(i + r, n),
				row1 = max(j - r, 1), row2 = min(j + r, n);
			int N = sum[column2][row2] - sum[column1 - 1][row2] - (sum[column2][row1 - 1] - sum[column1 - 1][row1 - 1]); // 注意 -1
			int num = (column2 - column1 + 1) * (row2 - row1 + 1);
			if (N <= t * num) {
				q++;
			}
		}
	cout << q << endl;

	return 0;
}

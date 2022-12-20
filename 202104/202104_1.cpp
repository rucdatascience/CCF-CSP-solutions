/*
CCF202104-1 灰度直方图

得分: 100
*/
#include <bits/stdc++.h> // 一个包含C/C++中很多常用头文件的头文件
using namespace std; // 默认命名空间：std

const int L = 256;
int cnt[L];

int main()
{
	int n, m, L;
	vector<int> h;

	cin >> n >> m >> L;
	h.resize(L, 0);

	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= m; j++) {
			int x;
			cin >> x;
			h[x]++;
		}
	}

	for (int i = 0; i < L; i++) {
		cout << h[i] << ' ';
	}
	return 0;
}

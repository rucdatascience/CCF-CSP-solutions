/*
CSP202206-2 寻宝！大冒险！
得分：100
*/
#include <bits/stdc++.h>
using namespace std;

#include <unordered_map> // 不在bits/stdc++.h内
#include <unordered_set> // 不在bits/stdc++.h内

vector<pair<int, int>> trees;
unordered_map<int, unordered_set<int>> A;

int check_x_y_in_A(int x, int y) { // 返回A[x][y]
	if (A.count(x)) {
		return A[x].count(y);
	}
	return 0;
}

int main()
{
	int n, L, S;
	cin >> n >> L >> S;

	for (int i = 0; i < n; i++) {
		int x, y;
		cin >> x >> y;
		trees.push_back({ x,y });
		A[x].insert(y);
	}

	vector<vector<int>> B(S + 1);
	for (int i = S; i >= 0; i--) {
		B[i].resize(S + 1);
		for (int j = 0; j <= S; j++) {
			cin >> B[i][j];
		}
	}

	int candidate_num = 0;
	for (int k = 0; k < n; k++) {
		int x = trees[k].first, y = trees[k].second;
		if (x + S > L || y + S > L) { // 超出绿化图边界，见样例2
			continue;
		}
		bool match = true;
		for (int i = S; i >= 0; i--) {
			for (int j = 0; j <= S; j++) {
				if (B[i][j] != check_x_y_in_A(x + i, y + j)) {
					match = false;
					break;
				}
			}
			if (!match) {
				break;
			}
		}
		if (match) {
			candidate_num++;
		}
	}
	cout << candidate_num;
	return 0;
}

#include<bits/stdc++.h>
#define tiii tuple<int, int, int>
#define mkt(x, y, z) make_tuple(x, y, z)
using namespace std;
int dir_x[8] = {-2, -2, -1, -1, 1, 1, 2, 2};
int dir_y[8] = {1, -1, 2, -2, 2, -2, 1, -1};
int tag[105][105];
int main() {
//	freopen("10.in", "r", stdin);
//	freopen("10.out", "w", stdout);
	int n, k, x, y;
	cin >> n >> k >> x >> y;
	queue<tiii> q;
	q.push(mkt(x, y, 0));
	while(!q.empty()) {
		tiii t = q.front();
		q.pop();
		int pos_x = get<0>(t), pos_y = get<1>(t), hop = get<2>(t);
		if (hop > k || tag[pos_x][pos_y] || pos_x < 1 || pos_x > n || pos_y < 1 || pos_y > n) {
			continue;
		}
		tag[pos_x][pos_y] = 1;
		for (int i = 0; i < 8; i ++){
			pos_x += dir_x[i], pos_y += dir_y[i];
			q.push(mkt(pos_x, pos_y, hop + 1));
			pos_x -= dir_x[i], pos_y -= dir_y[i];
		}
	}
	int cnt = 0;
	for (int i = 1; i <= n; i ++) {
		for (int j = 1; j <= n; j ++) {
			if (tag[i][j]) {
				cnt ++;
			}
		}
	}
	cout << cnt << endl;
	return 0;
}

#include<bits/stdc++.h>
using namespace std;
int main() {
//	freopen("10.in", "r", stdin);
//	freopen("10.out", "w", stdout);
	int k;
	double miu, sig, n;
	cin >> k;
	while(k --) {
		cin >> miu >> sig >> n;
		int z = (n - miu) * (100 / sig);
		int j = z % 10 + 1;
		int i = z / 10 + 1;
		cout << i << " " << j << endl;
	}
	return 0;
}
/*
6
0 1 1
2 10 127
2 50 227
5 100 350
123 100 1000
123 8 77
*/

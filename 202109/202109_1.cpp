/*
CSP202109_1 数组推导
得分：100
*/
#include <bits/stdc++.h>
using namespace std;

int main() {
	int n, b[110], i, amin[101], amax[101];
	cin >> n;
	for (i = 0; i < n; i++) {
		cin >> b[i];
	}

	amin[0] = b[0];
	amax[0] = b[0];
	for (i = 1; i < n; i++) {
		if (b[i] > b[i - 1]) { //出现新的最大值
			amin[i] = b[i];
			amax[i] = b[i];
		}
		else {//没有出现新的最大值
			amin[i] = 0;
			amax[i] = b[i];
		}
	}

	//计算累计和
	int sum1 = 0, sum2 = 0;
	for (i = 0; i < n; i++) {
		sum1 += amin[i];
		sum2 += amax[i];
	}
	cout << sum2 << endl << sum1 << endl;
	return 0;
}

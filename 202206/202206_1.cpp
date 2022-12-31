/*
CSP202206-1 归一化处理
得分：100
*/
#include <bits/stdc++.h>
using namespace std;

int N = 1001;
int main() {
	int n = 0;
	cin >> n;
	double sum = 0;
	vector<int> a(n + 1);
	for (int i = 1; i <= n; i++) {
		cin >> a[i];
		sum += a[i];
	}
	double avg = sum / n; // 平均数 
	double Da = 0; // 方差
	for (int i = 1; i <= n; i++) {
		Da += pow(a[i] - avg, 2) / n;
	}
	double m = sqrt(Da); // 标准差
	for (int i = 1; i <= n; i++) {
		double fa = (a[i] - avg) / m;
		printf("%f\n", fa);
	}
	return 0;
}

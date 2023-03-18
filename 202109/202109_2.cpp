/*
CSP202109_2 非零段划分
得分：100

https://blog.csdn.net/qq_52156445/article/details/120601455

前缀和+差分思想；非零段看作是独立的岛屿，海平面下降，出现凸点时+1，出现凹点时-1 ；连续的数字可看作是平面
*/
#include <bits/stdc++.h>
using namespace std;

int main()
{
	vector<int> a(500001, 0), b(10001, 0); //海平面下降到b[i]时，答案的变化量 
	int i, n;
	cin >> n;
	for (i = 1; i <= n; i++)
	{
		cin >> a[i];
	}
	/*
	unique:
	Eliminates all except the first element from every consecutive group of equivalent elements from the range 
	[first, last) and returns a past-the-end iterator for the new logical end of the range.
	*/
	n = unique(a.begin(), a.end()) - a.begin(); // n为unique去重后的a元素个数
	for (i = 1; i < n - 1; i++)
	{
		if ((a[i - 1] < a[i]) && (a[i + 1] < a[i])) //凸点
		{
			b[a[i]]++;
		}
		else if ((a[i - 1] > a[i]) && (a[i + 1] > a[i])) //凹点 
		{
			b[a[i]]--;
		}
	}
	int ans = 0, sum = 0;
	for (i = 10001; i; i--)
	{
		sum += b[i]; // sum记录了p=i时的非零段数
		ans = max(sum, ans); // ans记录最大的非零段数
	}
	cout << ans << endl;
	return 0;
}

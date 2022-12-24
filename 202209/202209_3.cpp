/*
CSP202209_3 防疫大数据
得分：100
*/
#include<iostream>
#include<vector>
#include<set>
#include<algorithm>

using namespace std;

typedef long long LL;
set<LL> sd[1050], su;
struct node {
	LL d, u, r;
}a;
vector<node> v[1050];
LL n, ri, mi, d, u, r, pi;

int main()
{
	cin >> n; // 输入数据的天数
	for (int i = 0; i < n; ++i)
	{
		cin >> ri >> mi;
		for (int j = 1; j <= ri; ++j)
		{
			cin >> pi;
			for (int k = i; k < i + 7; ++k)
			{
				sd[k].insert(pi); // 表示从i天pi地区一直到i+7(不包括)时间内为风险地区
			}
		}
		for (int j = 0; j < mi; ++j) // 输入漫游记录
		{
			cin >> d >> u >> r;
			if (d < 0) {
				continue;
			}
			// if (!sd[d].count(r)) {
			// 	continue; // 在d天查询有无风险地区，如果没有，说明u必定不是风险者，直接continue,count用来查询有无r
			// }
			a = { d,u,r };
			v[i].push_back(a);
		}
		for (int j = max(0, i - 6); j <= i; ++j)
		{
			for (int k = 0; k < v[j].size(); ++k)
			{
				d = v[j][k].d;
				r = v[j][k].r;
				u = v[j][k].u;

				if (d < i - 6) {
					continue;
				}
				bool flag = 1;
				for (int day = d; day <= i; ++day) {
					flag &= sd[day].count(r);
				}
				if (flag) {
					su.insert(u);
				}
			}
		}
		cout << i << " ";
		for (set<LL>::iterator it = su.begin(); it != su.end(); ++it)
		{
			cout << *it << " ";
		}
		cout << endl;
		su.clear();
	}
	return 0;
}

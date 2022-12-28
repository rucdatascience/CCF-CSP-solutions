/*
CSP202209_3 防疫大数据
得分：100
*/
#include <bits/stdc++.h>
using namespace std;

struct node {
	int d, u, r;
};

int main()
{
	int n, ri, mi, pi, d, u, r;
	cin >> n; // 输入数据的天数
	vector<set<int>> locations(n + 7); // locations[i]表示第i天的风险地区集合，下面最多记录第n+6天的风险地区集合
	vector<vector<node>> records(n); // records[i]表示第i天的记录
	for (int i = 0; i < n; i++)
	{
		set<int> persons; // 第i日的疫情风险人名单
		cin >> ri >> mi;
		for (int j = 1; j <= ri; j++)
		{
			cin >> pi;
			for (int k = i; k < i + 7; k++) // 最多记录第n+6天的风险地区集合
			{
				locations[k].insert(pi); // 表示pi从i天pi地区一直到i+7(不包括)时间内为风险地区
			}
		}
		for (int j = 0; j < mi; j++) // 输入漫游记录
		{
			cin >> d >> u >> r;
			if (d < 0) { // 没有负数天的风险地区记录，忽略
				continue;
			}
			node a = { d,u,r };
			records[i].push_back(a);
		}
		for (int j = max(0, i - 6); j <= i; j++) // 7天内
		{
			for (int k = 0; k < records[j].size(); k++) // 7天内的记录
			{
				d = records[j][k].d;
				r = records[j][k].r;
				u = records[j][k].u;
				if (d < i - 6) { // 7天以前的走动数据忽略
					continue;
				}
				bool flag = 1;
				for (int day = d; day <= i; day++) {
					flag *= locations[day].count(r);
				}
				if (flag) { // 地区r自到访日至生成名单当日持续处于风险状态
					persons.insert(u);
				}
			}
		}
		cout << i << " ";
		for (auto it = persons.begin(); it != persons.end(); it++)
		{
			cout << *it << " ";
		}
		cout << endl;
	}
	return 0;
}

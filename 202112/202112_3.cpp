/*
202112-3 登机牌条码
得分：100

https://blog.csdn.net/m0_53641110/article/details/123649159
*/

#include <bits/stdc++.h>
using namespace std;

const int mod = 929;
vector<int> compute_data_digits(string& str);
vector<int> compute_check_digits(int k, vector<int>& data_digits);

int main()
{
	int w, s, k = 0;
	string str;//输入字符串
	cin >> w >> s;
	cin >> str;

	if (s != -1) { //s==-1时，不需要计算校验字
		k = pow(2, s + 1);
	}

	vector<int> data_digits = compute_data_digits(str);

	int x = (data_digits.size() + 1 + k) % w;//判读全部的码字数量是否可以被行宽整除
	if (x != 0) {
		for (int i = 0; i < w - x; i++) {
			data_digits.push_back(900);
		}
	}
	int n = data_digits.size() + 1; //长度码字

	vector<int> check_digits = compute_check_digits(k, data_digits);

	/*print*/
	cout << n << endl;
	for (int i :data_digits){
		cout << i << endl;
	}
	for (int i : check_digits) {
		cout << i << endl;
	}
	return 0;
}

vector<int> compute_data_digits(string& str) {

	vector<int> string_numbers, data_digits;

	int pre = 1;//记录字符串当前的状态的上一个状态，1为大写字母，2为小写字母，3为数字
	for (int i = 0; i < str.length(); i++)
	{
		if (str[i] >= 'A' && str[i] <= 'Z') //当前字符为大写字母
		{
			if (pre == 1) { //上一个字符为大写字母
				string_numbers.push_back(str[i] - '0' - 17);
			}
			else if (pre == 2) { //上一个字符为小写字母
				string_numbers.push_back(28); //先转换成数字模式
				string_numbers.push_back(28); //再转换成大写模式
				string_numbers.push_back(str[i] - '0' - 17);
				pre = 1;
			}
			else if (pre == 3) { //上一个字符为数字
				string_numbers.push_back(28); //转换成大写模式
				string_numbers.push_back(str[i] - '0' - 17);
				pre = 1;
			}
		}
		else if (str[i] >= 'a' && str[i] <= 'z') { //当前字符为小写字母
			if (pre == 1) { //上一个字符为大写字母
				string_numbers.push_back(27); //转换成小写模式
				string_numbers.push_back(str[i] - '0' - 49);
				pre = 2;
			}
			else if (pre == 2) { //上一个字符为小写字母
				string_numbers.push_back(str[i] - '0' - 49);
			}
			else if (pre == 3) { //上一个字符为数字
				string_numbers.push_back(27); //转换成小写模式
				string_numbers.push_back(str[i] - '0' - 49);
				pre = 2;
			}
		}
		else if (str[i] >= '0' && str[i] <= '9') { //当前字符为数字
			if (pre == 1 || pre == 2) { //上一个字符为大小写字母
				string_numbers.push_back(28); //转换成数字模式
				string_numbers.push_back(str[i] - '0');
				pre = 3;
			}
			else if (pre == 3) { //上一个字符为数字
				string_numbers.push_back(str[i] - '0');
			}
		}
	}
	if (string_numbers.size() % 2 == 1) { //如果有奇数个数字，结尾添加29
		string_numbers.push_back(29);
	}

	for (int i = 0; i < string_numbers.size(); i += 2)
	{
		data_digits.push_back(string_numbers[i] * 30 + string_numbers[i + 1]);
	}

	return data_digits;
}

vector<int> compute_check_digits(int k, vector<int>& data_digits) {

	int N = 1e4; // 全部数据码字的个数少于929, 全部校验码字的个数不大于2^9=512，所有码字的个数不大于1e4
	vector<int> g(N), d(N); //函数g(x),d(x)

	/*计算g(x)，按照降次存入系数
	
	x-3  // [1 -3]
	(x-3)(x-3^2) // [1 (-3-3^2) (-3 * -3^2)]，即[1 -3 0] + [0 1 -3]*(-3^2)
	...
	*/
	g[0] = 1;
	int a = -3;
	for (int i = 1; i <= k; a = a * 3 % mod, i++) {
		for (int j = i - 1; j >= 0; j--) {//逆序计算
			g[j + 1] = (g[j + 1] + g[j] * a) % mod; //为了避免数据溢出，系数对mod取模
		}
	}

	/*计算d(x)*/
	d[0] = data_digits.size() + 1; //长度码字
	for (int i = 1; i <= data_digits.size(); i++)	{
		d[i] = data_digits[i - 1];
	}

	/*???*/
	for (int i = 0; i <= data_digits.size(); i++)	{
		int x = d[i];
		d[i] = 0;
		for (int j = 1; j <= k; j++)		{
			d[i + j] = (d[i + j] - x * g[j]) % mod; //关键代码，建议自己实际模拟一次
		}
	}

	vector<int> check_digits;
	for (int i = data_digits.size() + 1; i <= data_digits.size() + k; i++) {
		check_digits.push_back((-d[i] % mod + mod) % mod); //取反后输出，注意取模时要加上mod，是为了取模后取正值
	}

	return check_digits;
}


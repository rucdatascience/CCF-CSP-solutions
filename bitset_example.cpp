#include <iostream>
#include <bitset>
using namespace std;

int main()
{
	bitset<4> a1{ "0011" };
	bitset<4> a2 = 4; // 0100
	bitset<4> a3;
	a3[0] = 0, a3[1] = 1, a3[2] = 0, a3[3] = 1; // 1010   a3[0]是最右端的一位

	cout << a1 << " " << a2 << " " << a3 << " " << a1.count() << " " << a1.size() << endl;
	cout << (a1 & a2) << " " << (a1 & a3) << endl; //与运算
	cout << (a1 | a2) << " " << (a1 | a3) << endl; //或运算
	cout << (a1 ^ a2) << " " << (a1 ^ a3) << endl; //异或运算
	cout << ~a1 << " " << ~a2 << endl; //取反运算
	cout << (a1 >> 2) << " " << (a2 << 1) << endl; //移位运算
}
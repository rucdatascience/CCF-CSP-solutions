#include<bits/stdc++.h>
using namespace std;
int n;
string meg, meg1, meg2, meg3;
unsigned long long num1, num2, tag;
unsigned long long str_to_ll (string x) {
	unsigned long long ret = 0;
	for (int i = 0; i < x.length(); i ++) {
		ret = (ret << 1) + (x[i] - '0');
	}
	return ret;
}
unsigned long long get_hash (unsigned long long x, int n) {
	x = x * 47055833459;
	x = x >> (64 - n);
	x %= (1 << n);
	return x;
}
string get_code (unsigned long long x) {
	string ret = "";
	for (int i = 0; i < 11; i ++, x /= 38) {
		int y = x % 38;
		if (y == 0) ret = ret;
		else if (y <= 10) ret = (char)('0' + y - 1) + ret;
		else if (y <= 36) ret = (char)('A' + y - 11) + ret;
		else ret = "_" + ret;
	}
	return ret;
}
string get_typical_code (unsigned long long x) {
	string ret = "";
	int num[6];
	num[0] = x % 26; x /= 26;
	num[1] = x % 26; x /= 26;
	num[2] = x % 26; x /= 26;
	num[3] = x % 10; x /= 10;
	num[4] = x % 36; x /= 36;
	num[5] = x;
	ret = (char)(num[0] + 'A') + ret;
	ret = (char)(num[1] + 'A') + ret;
	ret = (char)(num[2] + 'A') + ret;
	ret = (char)(num[3] + '0') + ret;
	if (num[4] <= 9) ret = (char)(num[4] + '0') + ret;
	else ret = (char)(num[4] + 'A' - 10) + ret;
	if (num[5] == 0) ret = ret;
	else if (num[5] <= 10) ret = (char)('0' + num[5] - 1) + ret;
	else ret = (char)('A' + num[5] - 11) + ret;
	return ret;
}
unsigned long long typical_to_oridinary(string s) {
	unsigned long long ret = 0;
	for (int i = 0; i < s.length(); i ++) {
		if ('0' <= s[i] && s[i] <= '9') {
			ret = ret * 38 + (s[i] - '0') + 1;
		} else {
			ret = ret * 38 + (s[i] - 'A') + 11;
		}
	}
	for (int i = 0; i < 11 - s.length(); i ++){
		ret *= 38;
	}
	return ret;
}
map<unsigned long long, string> mp12, mp25;
int main() {
//	freopen("1.in", "r", stdin);
//	freopen("1.out", "w", stdout);
	cin >> n;
	while (n --) {
		cin >> meg;
		if (meg[0] == '0') {
			meg1 = meg.substr(1, 28);
			meg2 = meg.substr(29, 28);
			meg3 = meg.substr(57, 15);
			num1 = str_to_ll(meg1);
			num2 = str_to_ll(meg2);
			tag = str_to_ll(meg3);
			if (num1 >= (1 << 25)) {
				meg1 = get_typical_code(num1 - (1 << 25));
			} else {
				meg1 = (mp25.count(num1)) ? "#" + mp25[num1] : "###";
			}
			if (num2 >= (1 << 25)) {
				meg2 = get_typical_code(num2 - (1 << 25));
			} else {
				meg2 = (mp25.count(num2)) ? "#" + mp25[num2] : "###";
			}
			
			if (num1 >= (1 << 25)) {
				unsigned long long code = typical_to_oridinary(meg1);
				mp25[get_hash(code, 25)] = mp12[get_hash(code, 12)] = meg1;
			}
			if (num2 >= (1 << 25)) {
				unsigned long long code = typical_to_oridinary(meg2);
				mp25[get_hash(code, 25)] = mp12[get_hash(code, 12)] = meg2;
			}
			cout << meg1 << " " << meg2;
			if (tag) {
				cout << " " << tag << endl;
			} else {
				cout << endl;
			}
		} else {
			meg1 = meg.substr(1, 58);
			meg2 = meg.substr(59, 12);
			meg3 = meg.substr(71, 1);
			num1 = str_to_ll(meg1);
			num2 = str_to_ll(meg2);
			tag = str_to_ll(meg3);

			meg1 = get_code(num1);
			meg2 = (mp12.count(num2)) ? "#" + mp12[num2] : "###";
			mp25[get_hash(num1, 25)] = mp12[get_hash(num1, 12)] = meg1;
			if (!tag) swap(meg1, meg2); 
			
			cout << meg1 << " " << meg2 << endl;
		}
	}
	return 0;
}
/*
7
100111111001001101000001000100101001010001000000011011110000011100011010
000000111011010010001111000110010000110101101010001111011000000001111011
100111111001001101000001000100101001010001000001001000111000011101101000
110010110001000111010011000010110001101000100110000001111000011110010101
100111111001001101000001000100101001010001000001001000111000011101101000
100111111001001101000001000100101001010001000001110110000000011101101000
100111111001001101000001000100101001010001000001110110000000011101101000
*/ 

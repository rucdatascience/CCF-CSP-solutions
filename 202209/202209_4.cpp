/*
CSP202209_4 吉祥物投票
得分：100

测试时n最大为1e9，这么大的数组超过了空间限制，必须把连续的投票意愿相同的人看作一个线段，用std::set维护线段树（区间树）。
*/
#include<bits/stdc++.h>
using namespace std;

int ID = 0;
vector<int> parent_ID(200010), item_aID(200010), aID_item(200010);
/*
初始时，作品i有ID i，且parent_ID[i]=i。若parent_ID[i]=i，则i是一个ancestor ID（aID）。
item_aID[x]=代表作品x的aID。aID_item[i]=一个aID i所代表的作品。

操作一：每个线段投票有一个投票意愿ID i。
通过改变parent_ID[i]来改变该线段所投的作品（并查集find[i]来找对应的aID）。

操作二：parent_ID[item_aID[x]]=item_aID[w]，并给item_aID[x]一个新aID（之所以上面的大小为200010）。

操作三：swap(item_aID[x], item_aID[y])

如果不适用上述并查集方法会超时！
*/

struct node1 { // 用来维护作品投票排序
	int item_name, votes_num;
};
struct compare1 { // 函数对象，用以将node1按如下规则排序：先按votes_num从大到小排序，votes_num相等时按item_name从小到大排序
	bool operator()(const node1& a, const node1& b)const { // set要求此处为常类型
		if (a.votes_num == b.votes_num) {
			return a.item_name < b.item_name; // item_id from small to large
		}
		else {
			return a.votes_num > b.votes_num; // votes_num from large to small
		}
	}
};

struct node2 { // 线段投票
	int left, right, ID;
};
struct compare2 { // 函数对象，用以将node2按left从小到大的规则排序
	bool operator()(const node2& a, const node2& b)const { // set要求此处为常类型
		return a.left < b.left; // left from small to large
	}
};

int find_aID(int x); // 寻找一个ID的aID（并查集的find方法，带路径压缩）
void option1(int l, int r, int x);
void option2(int x, int w);
void option3(int x, int y);
void option4(int w);
void option5();

vector<int> votes(100001, 0); // 每件作品的票数，初始为0
set<node1, compare1> votes_rank; // 降序地记录作品的票数，用以快速查询最大票数的作品
set<node2, compare2> votes_segment_tree; // 投票区间树

int main() {
	int n, m, q;
	cin >> n >> m >> q;
	votes[0] = n;
	votes_rank.insert({ 0,n });
	votes_segment_tree.insert({ 1,n,ID }); // 所有人投票作品0（没有投票）
	while (ID <= m) {
		parent_ID[ID] = item_aID[ID] = aID_item[ID] = ID; // 初始化ID信息
		ID++;
	}

	while (q--) {
		int option;
		cin >> option;
		if (option == 1) {
			int l, r, x;
			cin >> l >> r >> x;
			option1(l, r, x);
		}
		else if (option == 2) {
			int x, w;
			cin >> x >> w;
			option2(x, w);
		}
		else if (option == 3) {
			int x, y;
			cin >> x >> y;
			option3(x, y);
		}
		else if (option == 4) {
			int w;
			cin >> w;
			option4(w);
		}
		else if (option == 5) {
			option5();
		}
	}
}

int find_aID(int x) { // 寻找一个ID的aID
	if (x != parent_ID[x]) parent_ID[x] = find_aID(parent_ID[x]); // 路径压缩，见并查集相关知识
	return parent_ID[x];
}

void option1(int l, int r, int x) {
	vector<node2> to_be_deleted, to_be_inserted; // 记录需要去除和添加的线段（在下面用指针遍历votes_segment_tree的过程中不能修改votes_segment_tree）
	node2 a = { r,1,1 };
	auto it_begin = votes_segment_tree.upper_bound(a); // 寻找left值大于r的第一个已有线段
	if (it_begin == votes_segment_tree.end()) {
		it_begin--; // 若寻找到了线段树的end处（votes_segment_tree.end()是最后一个线段的后一个无效node），则it_begin指向最后一个线段
	}
	for (auto it = it_begin;; it--) { // 向left值更小的方向遍历线段，去除与[l,r]重叠的线段
		if (it->left > r) {
			continue; // 该线段与[l,r]不重叠
		}
		else {
			if (it->right < l) {
				break; // 该线段及left值更小的线段与[l,r]不重叠
			}
			to_be_deleted.push_back(*it); // 该线段与[l,r]（部分）重叠
			if (it->left < l) {
				to_be_inserted.push_back({ it->left , l - 1, it->ID }); // 将该线段与[l,r]不重叠的部分生成新线段
			}
			if (it->right > r) {
				to_be_inserted.push_back({ r + 1 , it->right, it->ID }); // 将该线段与[l,r]不重叠的部分生成新线段
			}
		}
		if (it == votes_segment_tree.begin()) {
			break; // 遍历完线段
		}
	}
	for (auto it = to_be_deleted.begin(); it != to_be_deleted.end(); it++) {
		votes_segment_tree.erase(*it);
		int item_name = aID_item[find_aID(it->ID)];
		votes[item_name] -= it->right - it->left + 1; // 去除线段并更新votes_rank、votes信息
		votes_rank.insert({ item_name, votes[item_name] });
	}
	for (auto it = to_be_inserted.begin(); it != to_be_inserted.end(); it++) {
		votes_segment_tree.insert(*it);
		int aID = find_aID(it->ID);
		int item_name = aID_item[aID];
		votes[item_name] += it->right - it->left + 1; // 添加线段并更新votes_rank、votes信息
		votes_rank.insert({ item_name, votes[item_name] });
	}

	votes_segment_tree.insert({ l,r,item_aID[x] }); // 添加线段[l,r]并更新votes_rank、votes信息
	votes_rank.erase({ x, votes[x] });
	votes[x] += r - l + 1; // 假设[l,r]之前没投x
	votes_rank.insert({ x, votes[x] });
}

void option2(int x, int w) {
	votes[w] += votes[x]; // 更新votes_rank、votes信息
	votes[x] = 0;
	votes_rank.insert({ w, votes[w] });

	parent_ID[item_aID[x]] = item_aID[w]; // 更新parent_ID、item_aID、aID_item信息
	item_aID[x] = ID; //  赋予x一个新的aID（以前的aID指向w的aID了）
	parent_ID[ID] = ID;
	aID_item[ID] = x;
	ID++;
}

void option3(int x, int y) {
	swap(votes[x], votes[y]); // 更新votes_rank、votes信息
	votes_rank.insert({ x, votes[x] });
	votes_rank.insert({ y, votes[y] });

	swap(item_aID[x], item_aID[y]); // 更新item_aID、aID_item信息
	aID_item[item_aID[x]] = x;
	aID_item[item_aID[y]] = y;
}

void option4(int w) {
	cout << votes[w] << endl;
}

void option5() {
	/*votes_rank可能包含过大的错误票数*/
	while (votes_rank.size() > 0) {
		int item = votes_rank.begin()->item_name;
		int votes_num = votes_rank.begin()->votes_num;
		if (votes[item] == votes_num && item && votes_num) { // 此记录的票数是对的，且是票数为正的非0作品
			cout << item << endl;
			return;
		}
		else {
			votes_rank.erase(votes_rank.begin()); // 删除无效记录
		}
	}
	cout << 0 << endl; // votes_rank.size() == 0
}
/*
CSP202209_4 吉祥物投票
得分：55 （此代码没有使用并查集加速）

测试时n最大为1e9，这么大的数组超过了空间限制，所以考虑把连续的投票意愿相同的人看作一个线段，用std::set维护线段树。
*/
#include<bits/stdc++.h>
using namespace std;

int vote_ID = 0;
map<int, int> vote_id_to_item_id;
vector<set<int>> vote_ids_for_items(100000 + 1);

struct node1 {
	int item_id, votes_num;
};
struct compare1 { // 函数对象
	bool operator()(const node1& a, const node1& b)const { // set要求此处为常类型
		if (a.votes_num == b.votes_num) {
			return a.item_id < b.item_id; // item_id from small to large
		}
		else {
			return a.votes_num > b.votes_num; // votes_num from large to small
		}
	}
};

struct node2 {
	int left, right, vote_id;
};
struct compare2 { // 函数对象
	bool operator()(const node2& a, const node2& b)const { // set要求此处为常类型
		return a.left < b.left; // left from small to large
	}
};

void option1(int l, int r, int x);
void option2(int x, int w);
void option3(int x, int y);
void option4(int w);
void option5();

vector<int> votes(100000 + 1, 0); // 每件作品的票数
set<node1, compare1> votes_rank; // 有序地记录作品的票数（里面的元素和votes对应上才为真）
set<node2, compare2> votes_segment_tree;

int main() {
	int n, m, q;
	cin >> n >> m >> q;
	votes[0] = n;
	votes_rank.insert({ 0,n });
	votes_segment_tree.insert({ 1,n, vote_ID });
	vote_id_to_item_id[vote_ID] = 0;
	vote_ids_for_items[0].insert(vote_ID);
	vote_ID++;

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

void option1(int l, int r, int x) {
	vector<node2> to_be_deleted, to_be_inserted;
	node2 a = { r,1,1 };
	auto it_begin = votes_segment_tree.upper_bound(a);
	if (it_begin == votes_segment_tree.end()) {
		it_begin--;
	}
	//cout << "votes_segment_tree.size():" << votes_segment_tree.size() << endl;
	//cout << "it_begin:" << it_begin->left << " " << it_begin->right << " " << it_begin->vote_id << endl;
	for (auto it = it_begin;; it--) {
		if (it->left > r) {
			continue;
		}
		else {
			if (it->right < l) {
				break;
			}
			to_be_deleted.push_back(*it);
			if (it->left < l) {
				to_be_inserted.push_back({ it->left , l - 1, it->vote_id });
			}
			if (it->right > r) {
				to_be_inserted.push_back({ r + 1 , it->right, it->vote_id });
			}
		}
		if (it == votes_segment_tree.begin()) {
			break;
		}
	}
	for (auto it = to_be_deleted.begin(); it != to_be_deleted.end(); it++) {
		//cout << "to_be_deleted:" << it->left << " " << it->right << " " << it->vote_id << endl;
		votes_segment_tree.erase(*it);
		int item_id = vote_id_to_item_id[it->vote_id];
		votes[item_id] -= it->right - it->left + 1;
		votes_rank.insert({ item_id, votes[item_id] });
	}
	for (auto it = to_be_inserted.begin(); it != to_be_inserted.end(); it++) {
		//cout << "to_be_inserted:" << it->left << " " << it->right << " " << it->vote_id << endl;
		votes_segment_tree.insert(*it);
		int item_id = vote_id_to_item_id[it->vote_id];
		votes[item_id] += it->right - it->left + 1;
		votes_rank.insert({ item_id, votes[item_id] });
	}

	votes_segment_tree.insert({ l,r,vote_ID });
	votes[x] += r - l + 1; // 假设[l,r]之前没投x
	votes_rank.insert({ x, votes[x] });
	vote_id_to_item_id[vote_ID] = x;
	vote_ids_for_items[x].insert(vote_ID);
	vote_ID++;
}

void option2(int x, int w) {
	/*update votes and votes_rank*/
	votes_rank.erase({ w, votes[w] });
	votes[w] += votes[x];
	votes[x] = 0;
	votes_rank.insert({ w, votes[w] });
	
	for (auto it = vote_ids_for_items[x].begin(); it != vote_ids_for_items[x].end(); it++) {
		vote_ids_for_items[w].insert(*it);
		vote_id_to_item_id[*it] = w;
	}
	vote_ids_for_items[x].clear();
}

void option3(int x, int y) {
	/*update votes and votes_rank*/
	votes_rank.erase({ x, votes[x] });
	votes_rank.erase({ y, votes[y] });
	swap(votes[x], votes[y]);
	votes_rank.insert({ x, votes[x] });
	votes_rank.insert({ y, votes[y] });

	auto middle2 = vote_ids_for_items[x];
	vote_ids_for_items[x] = vote_ids_for_items[y];
	vote_ids_for_items[y] = middle2;
	for (auto it = vote_ids_for_items[x].begin(); it != vote_ids_for_items[x].end(); it++) {
		vote_id_to_item_id[*it] = x;
	}
	for (auto it = vote_ids_for_items[y].begin(); it != vote_ids_for_items[y].end(); it++) {
		vote_id_to_item_id[*it] = y;
	}
}

void option4(int w) {
	cout << votes[w] << endl;
}

void option5() {
	while (votes_rank.size() > 0) {
		int item = votes_rank.begin()->item_id;
		int votes_num = votes_rank.begin()->votes_num;
		if (votes[item] == votes_num && item && votes_num) { // this is a true record
			cout << item << endl;
			return;
		}
		else {
			votes_rank.erase(votes_rank.begin()); // remove the false record
		}
	}
	cout << 0 << endl; // votes_rank.size() == 0
}
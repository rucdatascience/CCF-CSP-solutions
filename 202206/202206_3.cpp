/*
CSP202206-3 角色授权
得分：100
*/
#include<bits/stdc++.h>
using namespace std;

#include<unordered_set> // 在此题中比set快

class role {
public:
	unordered_set<string> operations;
	unordered_set<string> resource_types;
	unordered_set<string> resource_names;
	unordered_set<string> linked_users_and_userGroups;
};

map<string, role> roles;

/*判断一个角色能否对某个资源执行某个操作*/
bool role_permission(role& r, string& operation, string& resource_type, string& resource_name) {
	if (r.operations.count(operation) == 0 && r.operations.count("*") == 0) {
		return false;
	}
	if (r.resource_types.count(resource_type) == 0 && r.resource_types.count("*") == 0) {
		return false;
	}
	if (r.resource_names.count(resource_name) == 0 && r.resource_names.size() > 0) {
		return false;
	}
	return true;
}

/*判断一个用户能否执行某个操作*/
bool user_permission(vector<string>& user_and_userGroups, string& operation, string& resource_type, string& resource_name) {
	for (auto it = roles.begin(); it != roles.end(); it++) {
		if (!role_permission(it->second, operation, resource_type, resource_name)) {
			continue; // 此处判断为常数时间，必须放在下面的判断之前，否则超时
		}
		bool role_linked = false;
		for (int i = user_and_userGroups.size() - 1; i >= 0; i--) {
			if (it->second.linked_users_and_userGroups.count(user_and_userGroups[i]) > 0) {
				role_linked = true;
				break;
			}
		}
		if (role_linked) {
			return true;
		}
	}
	return false;
}


int main() {
	/*
	提高cin,cout的速度
	https://blog.csdn.net/qq_45475271/article/details/107675845
	没有该行代码会运行超时
	*/
	ios::sync_with_stdio(false), cin.tie(0), cout.tie(0);

	int n, m, q, nv, no, nn, ns, ng;
	string name, rname, uname, x, y, z, ch;
	cin >> n >> m >> q;

	for (int i = 0; i < n; i++) {
		cin >> name >> nv;
		while (nv--) {
			cin >> x;
			roles[name].operations.emplace(x);
		}		
		cin >> no;
		while (no--) {
			cin >> x;
			roles[name].resource_types.emplace(x);
		}
		cin >> nn;
		while (nn--) {
			cin >> x;
			roles[name].resource_names.emplace(x);
		}		
	}

	for (int i = 0; i < m; i++) {
		cin >> name >> ns;
		for (int j = 0; j < ns; j++) {
			cin >> ch >> rname;
			roles[name].linked_users_and_userGroups.emplace(rname);
		}
	}

	/*每个待授权的行为都会包含主体用户和其关联的用户组的信息。由于鉴权过程中的其它因素，
	同一个名称的用户在先后两次待授权的行为中所属的用户组可能有区别，
	不能存储或记忆此前每个待授权的行为中，用户与用户组的关联情况，而是要按照每次待授权的行为中给出的信息独立判断。*/
	for (int i = 0; i < q; i++) {
		cin >> name >> ng;
		vector<string> user_and_userGroups = { name };
		for (int j = 0; j < ng; j++) {
			cin >> rname;
			user_and_userGroups.push_back(rname);
		}		
		cin >> x >> y >> z;
		cout << user_permission(user_and_userGroups, x, y, z) << endl;
	}
	return 0;
}
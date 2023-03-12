/*
CSP202206-4 光线追踪
得分：100

https://blog.csdn.net/fmy_xfk/article/details/126649981

光线总是在平行于坐标轴的方向上传播：因为镜面角度为45度，光线出发方向总是平行于坐标轴。
反射点的坐标总是整数：因为镜面两端点、光源的坐标均为整数。
反射次数至多为log 0.8 10^-9 = 93次。
本题最精巧之处即是“所有反射面的∣x1−x2∣之和不超过3×10^5”这个条件，这意味着，反射点的数量不会超过3×10^5， 此即解题关键。（可以用map暴力存储所有反射点!）
基于上述观察，用map排序存储所有反射点，继而二分查找下一个反射点，进行光线追踪
*/
#include<bits/stdc++.h>
using namespace std;

#include<unordered_map>

enum Directions { // 方向赋值
	X_POS = 0,
	Y_POS = 1,
	X_NEG = 2,
	Y_NEG = 3
};
int dx[] = { 1,0,-1,0 }, dy[] = { 0,1,0,-1 }; // 上述方向对x y的变化速度：dx[0]=1, dx[3]=-1

struct point {
	int x, y;
	bool operator==(const point& p) const { // 重载==
		return x == p.x && y == p.y;
	}
};
typedef pair<point, int> point_with_id; // 反射点和反射面编号
const point NULL_POINT = { INT_MAX, INT_MAX }; // 光线绝不会达到该无效坐标
const point_with_id NULL_PD = { NULL_POINT, -1 };

struct mirror {
	int x1, y1, x2, y2, y_change_ratio; // 该镜面上，x每+1，y+y_change_ratio （y_change_ratio = 1 / -1）
	double a;
	void read() {
		cin >> x1 >> y1 >> x2 >> y2 >> a;
		if (x1 > x2) { // 保证x1<x2，继而可以用y_change_ratio判断反射方向
			swap(x1, x2); swap(y1, y2);
		}
		y_change_ratio = (y2 - y1) / (x2 - x1);
	}
};
unordered_map<int, mirror> mirrors;

class space {
	/*
	* psx = 所有镜面整点按x方向排序；
	* key存放所有点的y值，value递增存储对应x坐标和镜面ID
	* psy = 所有镜面整点按y方向排序；
	* key存放所有点的x值，value递增存储对应y坐标和镜面ID
	*/
	map<int, map<int, int> > psx, psy; // 需要用map的upper_bound函数

	void add_point(int x, int y, int id) {
		auto it = psx.find(y);
		if (it == psx.end()) {
			map<int, int> add;
			add.insert({ x,id }); psx.insert({ y, add });
		}
		else {
			it->second.insert({ x,id });
		}
		it = psy.find(x);
		if (it == psy.end()) {
			map<int, int> add;
			add.insert({ y,id }); psy.insert({ x, add });
		}
		else {
			it->second.insert({ y,id });
		}
	}

	void del_point(int x, int y) {
		psx[y].erase(x);
		psy[x].erase(y);
	}

public:
	void add_mirror(mirror& m, int mirror_id) {
		for (int x = m.x1 + 1, y = m.y1 + m.y_change_ratio; x < m.x2; x++, y += m.y_change_ratio) { // 反射面不含端点
			add_point(x, y, mirror_id);
		}
	}
	void del_mirror(mirror& m) {
		for (int x = m.x1 + 1, y = m.y1 + m.y_change_ratio; x < m.x2; x++, y += m.y_change_ratio) { // 反射面不含端点
			del_point(x, y);
		}
	}
	//返回反射点和反射面编号
	point_with_id find_nearst_reflect_point(point p, int d) {
		if (d == X_POS || d == X_NEG) {
			auto it = psx.find(p.y);
			if (it == psx.end()) return NULL_PD; // 不会遇到反射点
			map<int, int>::iterator it2;
			if (d == X_POS) {
				it2 = it->second.upper_bound(p.x); // upper_bound返回第一个大于p.x的元素
				if (it2 == it->second.end()) return NULL_PD;
			}
			else {
				it2 = it->second.lower_bound(p.x); // lower_bound返回第一个大于等于p.x的元素
				if (it2 == it->second.begin()) return NULL_PD;
				--it2;//技巧：lower_bound的前一个就是第一个比p.x小的数
			}
			return { {it2->first,p.y}, it2->second };
		}
		else {
			auto it = psy.find(p.x);
			if (it == psy.end()) return NULL_PD;
			map<int, int>::iterator it2;
			if (d == Y_POS) {
				it2 = it->second.upper_bound(p.y);
				if (it2 == it->second.end()) return NULL_PD;
			}
			else {
				it2 = it->second.lower_bound(p.y);
				if (it2 == it->second.begin()) return NULL_PD;
				--it2;
			}
			return { {p.x,it2->first}, it2->second };
		}
	}
};
space SPACE;

pair<point, int> trace_light(point p, int d, double I, int T) {
	do {
		point_with_id ret = SPACE.find_nearst_reflect_point(p, d);
		point p2 = ret.first; 
		long long int dist = (long long int)abs(p.x - p2.x) + abs(p.y - p2.y);
		if (p2 == NULL_POINT || dist > T) { // 光线无法到达反射点p2
			p.x += dx[d] * T;
			p.y += dy[d] * T;
			return { p,(int)I }; // 向下取整
		}
		p = p2; // update p

		/*update d; next_direction*/
		if (d == X_POS) {
			d = mirrors[ret.second].y_change_ratio == 1 ? Y_POS : Y_NEG;
		}
		else if (d == X_NEG) {
			d = mirrors[ret.second].y_change_ratio == -1 ? Y_POS : Y_NEG;
		}
		else if (d == Y_POS) {
			d = mirrors[ret.second].y_change_ratio == 1 ? X_POS : X_NEG;
		}
		else {
			d = mirrors[ret.second].y_change_ratio == -1 ? X_POS : X_NEG;
		}

		I = I * mirrors[ret.second].a; // update I
		if (I < 1.0) {
			return { {0,0},0 };
		}
		T -= dist; // update T
	} while (T > 0);
	return { p,(int)I }; // 向下取整
}

int main() {
	/*
	提高cin,cout的速度
	https://blog.csdn.net/qq_45475271/article/details/107675845
	*/
	ios::sync_with_stdio(false), cin.tie(0), cout.tie(0);
	int m;
	cin >> m;
	for (int i = 1; i <= m; i++) {
		int option; 
		cin >> option;
		if (option == 1) {
			mirrors[i].read();
			SPACE.add_mirror(mirrors[i], i);
		}
		else if (option == 2) {
			int k; 
			cin >> k;
			SPACE.del_mirror(mirrors[k]);
		}
		else {
			int x, y, d, t;
			double I;
			cin >> x >> y >> d >> I >> t;
			auto ans = trace_light({ x,y }, d, I, t);
			cout << ans.first.x << ' ' << ans.first.y << ' ' << ans.second << endl;
		}
	}
	return 0;
}

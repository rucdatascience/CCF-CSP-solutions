/*
CCF202104-5 疫苗运输
得分：95
https://blog.csdn.net/qq_45734984/article/details/119650624
*/
#include <bits/stdc++.h>
using namespace std;


typedef long long LL;

const int N = 510;
const LL INF = 0x3f3f3f3f3f3f3f3fll; // 为何程序员喜欢将INF设置为0x3f3f3f3f https://zhuanlan.zhihu.com/p/57512786

int n, m;   // n个物流站，m条线路
int len[N];  // 每一条线路的总用时
struct Node {  // 存储线路信息
	int cid;  // 线路id   
	int distance;  // 当前线路内的站点pid到起点的距离   
	int pid;  // 线路上的站点赋予的pid,从0开始编号(起点为0,但是站点号不一定和编号相同)
};

// ps[ver] = {{1, 100, j}, {4, 150, k}}: 表示经过站点ver的线路有线路1和线路4
// 线路1编号为j的点经过此站点，j距离线路1的起点(编号为0)为100
vector<Node> ps[N];

vector<pair<int, int>> line[N]; // line[i][j] = {ver, y}: 表示线路i上编号(pid)为j的站点ver到下一个站点的距离(时间)是y
vector<LL> dist(N, INF);  // 每辆车最早拿到疫苗的时间, 通过该时间可以更新该线路上其他点最早拿到疫苗的时间
vector<LL> ans(N, INF);  // 每个站点最早拿到疫苗的时间
bool st[N];  // dijkstra中的判重数组
int pid[N]; // 存储每条线路最早拿到疫苗的点,例如pid[2]=1: 表示线路2最早拿到疫苗的点编号为1(Node中的pid)


/*
最大公约数（gcd）
https://blog.csdn.net/Ljnoit/article/details/104730787
*/
LL cal_gcd(LL a, LL b, LL& x, LL& y) {  // 求x, y, 使得ax + by = gcd(a, b)
	if (!b) {
		x = 1, y = 0;
		return a;
	}
	LL d = cal_gcd(b, a % b, y, x);
	y -= (a / b) * x;
	return d;
}

void dijkstra()  // 求1号点到n号点的最短路距离
{
	for (int i = 0; i < m; i++) {
		int d = 0;
		for (int j = 0; j < line[i].size(); j++) {
			if (line[i][j].first == 1) { // 如果线路i上的第j个点站点号是1的话，说明可以作为起点
				dist[i] = d;    // 类似于迪杰斯特拉算法图中的点和起点直接相连，权重为时间
				pid[i] = j;     // 线路i上最早获得疫苗的点的编号是j
				break;
			}
			d += line[i][j].second;
		}
	}
	for (int i = 0; i < m; i++) {
		int tmp = -1;
		for (int j = 0; j < m; j++) {   //选取一个不在集合中的，到起点距离最短的“点”，此处判断条件颇耐人寻味~
			if (!st[j] && (tmp == -1 || dist[j] < dist[tmp]))
				tmp = j;
		}
		st[tmp] = true;
		auto d = dist[tmp];
		for (int j = pid[tmp], k = 0; k < line[tmp].size(); k++, j = (j + 1) % line[tmp].size()) {
			for (auto& c : ps[line[tmp][j].first]) {
				if (st[c.cid]) continue;
				LL a = d, b = len[tmp];
				LL x = c.distance, y = len[c.cid];
				// 扩展欧几里得算法
				LL X, Y;
				LL D = cal_gcd(b, y, X, Y);
				if ((x - a) % D) continue; //不能同时到达交点，则迪杰斯特拉图中的两点无连线
				X = (x - a) / D * X;
				y /= D;
				X = (X % y + y) % y;
				if (dist[c.cid] > a + b * X) {//dijkstra更新路径
					dist[c.cid] = a + b * X;
					pid[c.cid] = c.pid;
				}
			}
			d += line[tmp][j].second;
		}
	}
}


int main()
{
	cin >> n >> m;
	for (int i = 0; i < m; i++) {
		int cnt, sum = 0;
		cin >> cnt;
		for (int j = 0; j < cnt; j++) {
			int ver, t;
			cin >> ver >> t;
			ps[ver].push_back({ i, sum, j });
			line[i].push_back({ ver, t });
			sum += t;
		}
		len[i] = sum;
	}

	dijkstra();
	for (int i = 0; i < m; i++) {
		if (dist[i] == INF) continue;
		LL d = dist[i];
		for (int j = pid[i], k = 0; k < line[i].size(); j = (j + 1) % line[i].size(), k++) {
			int ver = line[i][j].first;
			ans[ver] = min(ans[ver], d);
			d += line[i][j].second;
		}
	}

	for (int i = 2; i <= n; i++) {
		if (ans[i] == INF) {
			cout << "inf" << endl;
		}
		else {
			cout << ans[i] << endl;
		}
	}

	return 0;
}

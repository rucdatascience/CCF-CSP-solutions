/*
CCF202104-3 DHCP服务器
得分：100
*/
#include <bits/stdc++.h>
using namespace std;

class IP
{
public:
	int expire_time = 0; // 过期时间初始为0
	int state = 1;      // 1:未分配（初始状态） 2:待分配 3:占用 4:过期
	string occupier = ""; // 占用者初始为空
};


int main()
{
	int N, Tdef, Tmax, Tmin;
	string H;
	cin >> N >> Tdef >> Tmax >> Tmin >> H;

	/*实现细节第一步：初始化IP池*/
	vector<IP> ip_pool(N+1); // use IP 1,2,...,N
	
	string shost, rhost, type;
	int n, ti, ip, exp_time;
	cin >> n;
	for (int k = 0; k < n; k++) {	
		cin >> ti >> shost >> rhost >> type >> ip >> exp_time;

		/*处于待分配和占用状态的 IP 地址拥有一个大于零的过期时刻。在到达该过期时刻时，
		若该地址的状态是待分配，则该地址的状态会自动变为未分配，且占用者清空，过期时刻清零；
		否则该地址的状态会由占用自动变为过期，且过期时刻清零。*/
		for (int i = 1; i <= N; i++) {
			if (ip_pool[i].state == 2 && ip_pool[i].expire_time <= ti) {
				ip_pool[i].state = 1;
				ip_pool[i].occupier = "";
				ip_pool[i].expire_time = 0;
			}
			else if (ip_pool[i].state == 3 && ip_pool[i].expire_time <= ti) {
				ip_pool[i].state = 4;
				ip_pool[i].expire_time = 0;
			}
		}

		/*判断接收主机是否为本机，或者为 *，若不是，则判断类型是否为 Request，若不是，则不处理*/
		if (rhost != H && rhost != "*" && type != "REQ") {
			continue;
		}
		/*若类型不是 Discover、Request 之一，则不处理*/
		if (type != "DIS" && type != "REQ") {
			continue;
		}
		/*若接收主机为 *，但类型不是 Discover，或接收主机是本机，但类型是 Discover，则不处理*/
		if ((rhost == "*" && type != "DIS") || (rhost == H && type == "DIS")) {
			continue;
		}

		if (type == "DIS") {
			/*
			检查是否有占用者为发送主机的 IP 地址：
            若有，则选取该 IP 地址；
            若没有，则选取最小的状态为未分配的 IP 地址；
            若没有，则选取最小的状态为过期的 IP 地址；
			若没有，则不处理该报文，处理结束；
			*/
			int selected_ip = 0, smallest_state1 = N + 1, smallest_state4 = N + 1;
			for (int i = 1; i <= N; i++) {
				if (ip_pool[i].occupier == shost) {
					selected_ip = i;
					break;
				}
				if (ip_pool[i].state == 1 && smallest_state1 > i) {
					smallest_state1 = i;
				}
				if (ip_pool[i].state == 4 && smallest_state4 > i) {
					smallest_state4 = i;
				}
			}	
			if (selected_ip == 0 && smallest_state1 <= N) {
				selected_ip = smallest_state1;
			}
			if (selected_ip == 0 && smallest_state4 <= N) {
				selected_ip = smallest_state4;
			}
			if (selected_ip == 0) {
				continue;
			}

			/*将该 IP 地址状态设置为待分配，占用者设置为发送主机*/
			ip_pool[selected_ip].state = 2;
			ip_pool[selected_ip].occupier = shost;

			/*若报文中过期时刻为 0 ，则设置过期时刻为t + Tdef；
			否则根据报文中的过期时刻和收到报文的时刻计算过期时间，判断是否超过上下限：
			若没有超过，则设置过期时刻为报文中的过期时刻；
			否则则根据超限情况设置为允许的最早或最晚的过期时刻*/
			if (exp_time == 0) {
				ip_pool[selected_ip].expire_time = ti + Tdef;
			}
			else if (Tmin <= exp_time - ti && exp_time - ti <= Tmax) {
				ip_pool[selected_ip].expire_time = exp_time;
			}
			else if (exp_time - ti < Tmin) {
				ip_pool[selected_ip].expire_time = ti + Tmin;
			}
			else if (exp_time - ti > Tmax) {
				ip_pool[selected_ip].expire_time = ti + Tmax;
			}

			/*向发送主机发送 Offer 报文，其中，IP 地址为选定的 IP 地址，过期时刻为所设定的过期时刻*/
			cout << H << " " << shost << " OFR " << selected_ip << " " << ip_pool[selected_ip].expire_time << endl;
		}
		else { // type == "REQ"
			/*检查接收主机是否为本机：
			若不是，则找到占用者为发送主机的所有 IP 地址，对于其中状态为待分配的，
			将其状态设置为未分配，并清空其占用者，清零其过期时刻，处理结束；*/
			if (rhost != H) {
				for (int i = 1; i <= N; i++) {
					if (ip_pool[i].occupier == shost && ip_pool[i].state == 2) {
						ip_pool[i].state = 1;
						ip_pool[i].occupier = "";
						ip_pool[i].expire_time = 0;
					}
				}
				continue;
			}

			/*检查报文中的 IP 地址是否在地址池内，且其占用者为发送主机，若不是，则向发送主机发送 Nak 报文，处理结束*/
			if (ip < 1 || ip > N || ip_pool[ip].occupier != shost) {
				cout << H << " " << shost << " NAK " << ip << " 0" << endl;
				continue;
			}

			/*无论该 IP 地址的状态为何，将该 IP 地址的状态设置为占用*/
			ip_pool[ip].state = 3;

			/*与 Discover 报文相同的方法，设置 IP 地址的过期时刻*/
			if (exp_time == 0) {
				ip_pool[ip].expire_time = ti + Tdef;
			}
			else if (Tmin <= exp_time - ti && exp_time - ti <= Tmax) {
				ip_pool[ip].expire_time = exp_time;
			}
			else if (exp_time - ti < Tmin) {
				ip_pool[ip].expire_time = ti + Tmin;
			}
			else if (exp_time - ti > Tmax) {
				ip_pool[ip].expire_time = ti + Tmax;
			}

			/*向发送主机发送 Ack 报文*/
			cout << H << " " << shost << " ACK " << ip << " " << ip_pool[ip].expire_time << endl;
		}
	}

	return 0;
}

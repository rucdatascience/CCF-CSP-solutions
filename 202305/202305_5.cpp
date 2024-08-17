#pragma GCC optimize(3)
#include<bits/stdc++.h>
using namespace std;
struct edge{
	int to,zt,dis;
};
struct node{
	int to,zt,dis;
	inline bool operator <(const node &x)const{
        return dis>x.dis;
    }
};
int st[100005],ed[100005],zt[100005];
vector<edge> edg[100005];
int N;
string t,s[100005];
int tag[150];
int dis[26][26][3000],inq[26][26][3000];
priority_queue<node> q;
void solve(){
	memset(dis,0x3f,sizeof(dis));
	for(int i=0;i<26;i++){
		dis[i][i][0]=0;
		q.push({i,0,0});
		while(!q.empty()){
			node x=q.top();
			q.pop();
			if(inq[i][x.to][x.zt]) continue;
			inq[i][x.to][x.zt]=1;
			for(int j=0;j<edg[x.to].size();j++){
				node y;
				y.to=edg[x.to][j].to;
				y.zt=x.zt|edg[x.to][j].zt;
				y.dis=x.dis+edg[x.to][j].dis;
				if(y.dis<dis[i][y.to][y.zt]){
					q.push(y);
					dis[i][y.to][y.zt]=y.dis;
				}
			}
		}
	}
}
int main(){
	ios::sync_with_stdio(0),cin.tie(0),cout.tie(0);
	cin>>N>>t;
	for(int i=0;i<t.length();i++){
		tag[t[i]]=i+1;
	}
	for(int i=1;i<=N;i++){
		cin>>s[i];
		st[i]=s[i][0]-'a';
		ed[i]=s[i][s[i].length()-1]-'a';
		for(int j=0;j<s[i].length();j++){
			if(tag[s[i][j]]) zt[i]|=(1<<(tag[s[i][j]]-1));
		}
		edg[st[i]].push_back({ed[i],zt[i],s[i].length()-1});
	}
	solve();
	int all=(1<<t.length())-1;
	for(int i=1;i<=N;i++){
		int u=st[i],v=ed[i],sta=zt[i];
		int ans=dis[v][u][all]+(int)s[i].length()-1;
		for(int j=sta;j;j=(j-1)&sta){
			ans=min(ans,dis[v][u][all^j]+(int)s[i].length()-1);
		}
		if(ans>=1e9)cout<<-1<<endl;
		else cout<<ans<<endl;
	}
	return 0;
}

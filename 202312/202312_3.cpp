#include<bits/stdc++.h>
using namespace std;
int n,m;
long long sum[2005],w[2005];
int fa[2005],vis[2005],ques[2005],sz[2005],mp[2005][2005];
vector<int> que,anss,ans[2005],son[2005];;
void dfs(int x,int rt){
	mp[rt][x]=1;
	for(int i=0;i<son[x].size();i++){
		dfs(son[x][i],rt);
	}
}
void dfs1(int x){
	sum[x]=w[x];
	sz[x]=1;
	for(int i=0;i<son[x].size();i++){
		if(!vis[son[x][i]]){
			dfs1(son[x][i]);
			sum[x]+=sum[son[x][i]];
			sz[x]+=sz[son[x][i]];
		}
	}
}
void dfs2(int x,int rt,int &v){
	if(abs(sum[rt]-2ll*sum[x])<abs(sum[rt]-2ll*sum[v])){
		v=x;
	}
	else if(abs(sum[rt]-2ll*sum[x])==abs(sum[rt]-2ll*sum[v])&&x<v){
		v=x;
	}
	for(int i=0;i<son[x].size();i++){
		if(!vis[son[x][i]]){
			dfs2(son[x][i],rt,v);
		}
	}
}
void solve(int x,vector<int>& q){
	dfs1(x);
	if(sz[x]==1){
		vis[x]=1;
		if(q.size()){
			for(int j=0;j<anss.size();j++){
				ans[q[0]].push_back(anss[j]);
			}
		}
		return;
	}
	int cen=x;
	dfs2(x,x,cen);
	vector<int> l,r;
	l.clear(),r.clear();
	for(int i=0;i<q.size();i++){
		if(mp[cen][q[i]]){
			l.push_back(q[i]);
		}else{
			r.push_back(q[i]); 
		}
	}
	anss.push_back(cen);
	solve(cen,l);
	vis[cen]=1;
	solve(x,r);
	anss.pop_back();
}
int main(){
	scanf("%d%d",&n,&m);
	for(int i=1;i<=n;i++){
		scanf("%lld",&w[i]);
	}
	for(int i=2;i<=n;i++){
		scanf("%d",&fa[i]);
		son[fa[i]].push_back(i);
	}
	for(int i=1;i<=n;i++){
		dfs(i,i);
	}
	for(int i=1;i<=m;i++){
		int v;
		scanf("%d",&v);
		ques[i]=v;
		que.push_back(v);
	}
	solve(1,que);
	for(int i=1;i<=m;i++){
		for(int j=0;j<ans[ques[i]].size();j++){
			printf("%d ",ans[ques[i]][j]);
		}
		puts("");
	}
	return 0;
}

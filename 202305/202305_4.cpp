#pragma GCC optimize(3)
#include<bits/stdc++.h>
using namespace std;
int n,m,k;
long long t[30005][105],cost[10005][15];
long long dp[10005][15];
vector<int> ed[10005];
set<int> edg[10005];
int uu[1000005],vv[1000005];
map<int,int> mp[10005];
long long ans=1e15;
int du[10005];
int col[10005];
long long gett(int a,int b,int x,int y){
	if(mp[a].count(b)){
		return t[mp[a][b]][(x-1)*k+y];
	}else{
		return t[mp[b][a]][(y-1)*k+x];
	}
}
void dfs0(int x){
	if(x==n){
		long long tmp=0;
		for(int i=1;i<=m;i++){
			tmp+=t[mp[uu[i]][vv[i]]][(col[uu[i]]-1)*k+col[vv[i]]];
		}
		for(int i=0;i<n;i++){
			tmp+=cost[i][col[i]];
		}
		ans=min(ans,tmp);
		return;
	}
	for(int j=1;j<=k;j++){
		col[x]=j;
		dfs0(x+1);
	}
}

void dfs1(int x,int fa){
	for(int j=1;j<=k;j++) dp[x][j]=cost[x][j];
	for(int i=0;i<ed[x].size();i++){
		if(ed[x][i]!=fa&&ed[x][i]!=-1){
			dfs1(ed[x][i],x);
			for(int j=1;j<=k;j++){
				long long tmp=1e15;
				for(int kk=1;kk<=k;kk++){
					tmp=min(tmp,dp[ed[x][i]][kk]+gett(x,ed[x][i],j,kk));
				}
				dp[x][j]+=tmp;
			}
		}
	}
}

int a,b;
void dfs2(int x,int fa){
	for(int j=1;j<=k;j++) dp[x][j]=cost[x][j];
	for(int i=0;i<ed[x].size();i++){
		if(ed[x][i]!=fa&&ed[x][i]!=-1){
			dfs2(ed[x][i],x);
			for(int j=1;j<=k;j++){
				long long tmp=1e15;
				for(int kk=1;kk<=k;kk++){
					tmp=min(tmp,dp[ed[x][i]][kk]+gett(x,ed[x][i],j,kk));
				}
				dp[x][j]+=tmp;
			}
		}
	}
	if(x==b){
		for(int i=1;i<=k;i++){
			if(i!=col[x]){
				dp[x][i]=1e15;
			}
		}
	}
}

int fa[10005];
int findfa(int x){
	if(x==fa[x])return x;
	return fa[x]=findfa(fa[x]);
}

void solve0(){
	dfs0(0);
	cout<<ans<<endl;
}
void solve1(){
	dfs1(0,0);
	for(int i=1;i<=k;i++){
		ans=min(ans,dp[0][i]);
	}
	cout<<ans<<endl;
}
void solve2(){
	for(int i=0;i<=n-1;i++) fa[i]=i;
	for(int i=1;i<=m;i++){
		int fax=findfa(uu[i]),fay=findfa(vv[i]);
		if(fax==fay){
			a=uu[i],b=vv[i];
			break;
		}
		fa[fax]=fay;
	}
	for(int i=0;i<ed[b].size();i++){
		if(ed[b][i]==a) ed[b][i]=-1;
	}
	for(int i=0;i<ed[a].size();i++){
		if(ed[a][i]==b) ed[a][i]=-1;
	}
	for(int i=1;i<=k;i++){
		col[b]=i;
		for(int j=1;j<=k;j++){
			cost[a][j]+=gett(b,a,i,j);
		}
		dfs2(a,a);
		for(int j=1;j<=k;j++){
			ans=min(ans,dp[a][j]);
			cost[a][j]-=gett(b,a,i,j);
		}
	}
	cout<<ans<<endl;
}
void solve3(){
	for(int i=0;i<=n;i++){
		if(b==i) continue;
		for(int j=0;j<ed[i].size();j++){
			if(ed[i][j]==b) ed[i][j]=-1;
		}
	}
	if(b==0) a=b+1;
	for(int i=1;i<=k;i++){
		col[b]=i;
		for(int j=0;j<ed[b].size();j++){
			for(int kk=1;kk<=k;kk++){
				cost[ed[b][j]][kk]+=gett(b,ed[b][j],i,kk);
			}
		}
		dfs1(a,a);
		for(int j=1;j<=k;j++){
			ans=min(ans,dp[a][j]+cost[b][i]);
		}
		for(int j=0;j<ed[b].size();j++){
			for(int kk=1;kk<=k;kk++){
				cost[ed[b][j]][kk]-=gett(b,ed[b][j],i,kk);
			}
		}
	}
	cout<<ans<<endl;
}
int vis[10005];
struct node{
	int du,id;
	inline bool operator <(const node &x)const{
        return du>x.du;
    }
};
int tot;
long long aa[15][15],bb[15][15];
int now_node[15],tot_node;
priority_queue<node> q;
void dfs3(int x){
	if(x==tot_node+1){
		long long tmp=0;
		for(int i=1;i<=tot_node-1;i++){
			for(int j=i+1;j<=tot_node;j++){
				int ii=now_node[i],jj=now_node[j];
				if(mp[ii].count(jj)||mp[jj].count(ii)) tmp+=gett(ii,jj,col[i],col[j]);
			}
		}
		for(int i=1;i<=tot_node;i++){
			tmp+=cost[now_node[i]][col[i]];
		}
		ans=min(ans,tmp);
		return;
	}
	for(int j=1;j<=k;j++){
		col[x]=j;
		dfs3(x+1);
	}
}
void solve4(){
	for(int i=1;i<=m;i++){
		edg[uu[i]].insert(vv[i]);
		edg[vv[i]].insert(uu[i]);
	}
	for(int i=0;i<n;i++){
		du[i]=ed[i].size();
		if(du[i]<=2) q.push({du[i],i});
	}
	while(!q.empty()){
		node x=q.top();
		q.pop();
		if(x.du!=du[x.id]) continue;
		if(vis[x.id]) continue;
		if(x.du==0) break;
		vis[x.id]=1;
		if(x.du==1){
			set<int>::iterator it=edg[x.id].begin();
			int to=(*it);
			for(int i=1;i<=k;i++){
				long long tmp=1e15;
				for(int j=1;j<=k;j++){
					tmp=min(tmp,cost[x.id][j]+gett(to,x.id,i,j));
				}
				cost[to][i]+=tmp;
			}
			if(--du[to]<=2) q.push({du[to],to});
			edg[to].erase(x.id);
			edg[x.id].clear();
			du[x.id]=0;
		}else if(x.du==2){
			int u,v;
			for(set<int>::iterator it=edg[x.id].begin();it!=edg[x.id].end();it++){
				if(it==edg[x.id].begin()) u=(*it);
				else v=(*it);
			}
			if((!mp[u].count(v))&&(!mp[v].count(u))){
				mp[u][v]=++tot;
				du[u]++,du[v]++;
				edg[u].insert(v),edg[v].insert(u);
			}else{
				if(!mp[u].count(v)) swap(u,v);
			}
			int xxx=mp[u][v];
			for(int i=1;i<=k;i++){
				for(int kk=1;kk<=k;kk++){
					aa[i][kk]=gett(u,x.id,i,kk);
					bb[i][kk]=gett(v,x.id,i,kk);
				}
			}
			for(int i=1;i<=k;i++){
				for(int j=1;j<=k;j++){
					long long tmp=1e15;
					for(int kk=1;kk<=k;kk++){
						tmp=min(tmp,cost[x.id][kk]+aa[i][kk]+bb[j][kk]);
					}
					t[xxx][(i-1)*k+j]+=tmp;
				}
			}
			edg[u].erase(x.id);
			if(--du[u]<=2)q.push({du[u],u});
			
			edg[v].erase(x.id);
			if(--du[v]<=2)q.push({du[v],v});
			
			du[x.id]=0;
			edg[x.id].clear();
		}
	}
	for(int i=0;i<n;i++){
		if(!vis[i]){
			now_node[++tot_node]=i;
		}
	}
	dfs3(1);
	cout<<ans<<endl;
}

int main(){
	ios::sync_with_stdio(0),cin.tie(0),cout.tie(0);
	cin>>n>>m>>k;
	for(int i=1;i<=n;i++){
		for(int j=1;j<=k;j++){
			cin>>cost[i-1][j];
		}
	}
	tot=m;
	for(int i=1;i<=m;i++){
		int u,v;
		cin>>u>>v;
		du[u]++,du[v]++;
		uu[i]=u,vv[i]=v;
		ed[u].push_back(v);
		ed[v].push_back(u);
		mp[u][v]=i;
		for(int j=1;j<=k*k;j++){
			cin>>t[i][j];
		}
	}
	if(n<=6){
		solve0();
	}else if(m==n-1){
		solve1();
	}else if(m==n){
		solve2();
	}else{
		b=-1;
		for(int i=0;i<n;i++){
			du[i]=ed[i].size(); 
			if(m-du[i]==n-2){
				int tag=1;
				for(int j=0;j<ed[i].size();j++){
					if(du[ed[i][j]]!=2){
						tag=0;
						break;
					}
				}
				if(tag){
					b=i;
					break;
				}
			}
		}
		if(b!=-1){
			solve3();
		}else{
			solve4();
		}
	}
	return 0;
}

#include<bits/stdc++.h>
using namespace std;
#define N 200005
int n,m,p[N],dep[N],rt[N],idx,f[22][N];
vector<int> e[N];
struct node{
	int ls,rs,siz;
}t[N<<6];
#define lc(x) t[x].ls
#define rc(x) t[x].rs
inline void update(int &x,int y,int l,int r,int pos){
	t[x=++idx]=t[y],t[x].siz++;
	if(l==r) return;
	int mid=l+r>>1;
	if(pos<=mid) update(lc(x),lc(y),l,mid,pos);
	else update(rc(x),rc(y),mid+1,r,pos);
}
inline int query(int a,int b,int c,int d,int l,int r){
	if(l==r) return l;
	int mid=l+r>>1,val=t[lc(a)].siz+t[lc(b)].siz-t[lc(c)].siz-t[lc(d)].siz;
	if(val<mid-l+1) return query(lc(a),lc(b),lc(c),lc(d),l,mid);
	return query(rc(a),rc(b),rc(c),rc(d),mid+1,r);
}
inline void dfs(int x,int fa){
	f[0][x]=fa,dep[x]=dep[fa]+1,update(rt[x],rt[fa],0,n,p[x]);
	for(int i=1;i<=19;i++) f[i][x]=f[i-1][f[i-1][x]];
	for(auto v:e[x])
		if(v!=fa) dfs(v,x);
}
inline int lca(int x,int y){
	if(dep[x]<dep[y]) swap(x,y);
	for(int i=19;~i;i--)
		if(dep[f[i][x]]>=dep[y]) x=f[i][x];
	if(x==y) return x;
	for(int i=19;~i;i--)
		if(f[i][x]!=f[i][y]) x=f[i][x],y=f[i][y];
	return f[0][x];
}
int main(){
	scanf("%d%d",&n,&m);
	for(int i=1;i<=n;i++) scanf("%d",&p[i]);
	for(int i=1,u,v;i<n;i++)
		scanf("%d%d",&u,&v),e[u].emplace_back(v),e[v].emplace_back(u);
	dfs(1,0);
	for(int i=1,x,y;i<=m;i++){
		scanf("%d%d",&x,&y);
		int k=lca(x,y);
		printf("%d\n",query(rt[x],rt[y],rt[k],rt[f[0][k]],0,n));
	}
	return 0;
}
#pragma GCC optimize(3)
#include<bits/stdc++.h>
#define pii pair<int,int>
#define mp(a,b) make_pair(a,b)
#define fir first
#define sec second
using namespace std;
int n,m,l,k;
int c[105],u[5005],v[5005],d[5005];
int cnt1,cnt2;
vector<pii> in[105],out[105];
vector<pii> ans1,ans2;
map<int,int> dp1[105][10],dp2[105][10];
int main(){
	scanf("%d%d%d%d",&n,&m,&l,&k);
	for(int i=0;i<n;i++) scanf("%d",&c[i]);
	for(int i=0;i<m;i++) scanf("%d",&u[i]);
	for(int i=0;i<m;i++) scanf("%d",&v[i]);
	for(int i=0;i<m;i++) scanf("%d",&d[i]);
	for(int i=0;i<m;i++){
		in[u[i]].push_back(mp(v[i],d[i]));
		out[v[i]].push_back(mp(u[i],d[i]));
	}
	cnt1=l/2;
	dp1[0][0][1<<c[0]]=0;
	for(int i=0;i<cnt1;i++){
		for(int j=0;j<n;j++){
			for(auto k=dp1[j][i].begin();k!=dp1[j][i].end();++k){
				if(__builtin_popcount(k->fir)==i+1){
					for(auto l:in[j]){
						if(k->fir&(1<<c[l.fir])) continue;
						dp1[l.fir][i+1][k->fir|(1<<c[l.fir])]=max(dp1[l.fir][i+1][k->fir|(1<<c[l.fir])],k->sec+l.sec);
					}
				}
			}
		}
	}
	cnt2=l-cnt1-1;
	dp2[n-1][0][1<<c[n-1]]=0;
	for(int i=0;i<cnt2;i++){
		for(int j=0;j<n;j++){
			for(auto k=dp2[j][i].begin();k!=dp2[j][i].end();++k){
				if(__builtin_popcount(k->fir)==i+1){
					for(auto l:out[j]){
						if(k->fir&(1<<c[l.fir])) continue;
						dp2[l.fir][i+1][k->fir|(1<<c[l.fir])]=max(dp2[l.fir][i+1][k->fir|(1<<c[l.fir])],k->sec+l.sec);
					}
				}
			}
		}
	}
	int ans=0;
	for(int i=0;i<n;i++){
		ans1.clear(),ans2.clear();
		for(int j=0;j<=cnt1;j++){
			if(dp1[i][j].begin()==dp1[i][j].end()) continue;
			for(auto k=dp1[i][j].begin();k!=dp1[i][j].end();++k){
				ans1.push_back(mp(k->sec,k->fir));
			}
		}
		for(int j=0;j<=cnt2;j++){
			if(dp2[i][j].begin()==dp2[i][j].end()) continue;
			for(auto k=dp2[i][j].begin();k!=dp2[i][j].end();++k){
				ans2.push_back(mp(k->sec,k->fir));
			}
		}
		if(ans1.empty()||ans2.empty()) continue;
		sort(ans1.begin(),ans1.end(),greater<pii>());
		sort(ans2.begin(),ans2.end(),greater<pii>());
		for(int x=0;x<ans1.size();x++){
			if(ans1[x].fir+ans2[0].fir<=ans)break;
			for(int y=0;y<ans2.size();y++){
				if(ans1[x].fir+ans2[y].fir<=ans)break;
				if((ans1[x].sec&ans2[y].sec)==(1<<c[i])){
					if(ans1[x].fir+ans2[y].fir>ans){
						ans=ans1[x].fir+ans2[y].fir;
						break;
					}
				}
			}
		} 
	}
	printf("%d\n",ans);
	return 0;
}

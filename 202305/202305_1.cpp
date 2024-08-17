#include<bits/stdc++.h>
#define N 8
using namespace std;
char ch[105][10][10];
int n,ans[105];
bool cmp(int x,int y){
	for(int i=1;i<=N;i++){
		for(int j=1;j<=N;j++){
			if(ch[x][i][j]!=ch[y][i][j]){
				return false;
			}
		}
	}
	return true;
}
int main(){
	scanf("%d",&n);
	for(int i=1;i<=n;i++){
		for(int j=1;j<=N;j++){
			scanf("%s",ch[i][j]+1);
		}
	}
	for(int i=1;i<=n;i++){
		if(ans[i]){
			continue;
		}
		ans[i]=1; 
		int cnt=1;
		for(int j=i+1;j<=n;j++){
			if(cmp(i,j)){
				cnt++;
				ans[j]=cnt;
			}
		}
	}
	for(int i=1;i<=n;i++){
		printf("%d\n",ans[i]); 
	}
	return 0;
}

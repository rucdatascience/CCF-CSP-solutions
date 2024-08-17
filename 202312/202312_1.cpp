#include<bits/stdc++.h>
using namespace std;
int n,m;
int code[1005][15];
int ans[1005];
int main(){
	scanf("%d%d",&n,&m);
	for(int i=1;i<=n;i++){
		for(int j=1;j<=m;j++){
			scanf("%d",&code[i][j]);
		}
	}
	for(int i=1;i<=n;i++){
		for(int j=n;j>=1;j--){
			int tag=1;
			for(int k=1;k<=m;k++){
				if(code[i][k]>=code[j][k]){
					tag=0;
					break; 
				}
			}
			if(tag){
				ans[i]=j;
			}
		}
	}
	for(int i=1;i<=n;i++){
		printf("%d\n",ans[i]);
	}
	return 0;
}

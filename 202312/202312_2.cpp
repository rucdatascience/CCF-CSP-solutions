#include<bits/stdc++.h>
using namespace std;
int q;
long long n,k,res,cnt,ans;
int main(){
	scanf("%d",&q);
	while(q--){
		ans=1;
		scanf("%lld%lld",&n,&k);
		for(long long i=2;i*i<=n;i++){
			res=1,cnt=0;
			while(n%i==0){
				cnt++;
				res*=i;
				n/=i;
			}
			if(cnt>=k){
				ans=ans*res;
			}
		}
		if(k<=1)ans*=n;
		printf("%lld\n",ans); 
	}
	return 0;
}

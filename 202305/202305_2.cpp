#include<bits/stdc++.h>
using namespace std;
int n,d;
struct matrix{
	int x,y;
	vector<long long> mat[10005];
}q,k,v;
istream& operator >>(istream& in,matrix& x){
	for(int i=1;i<=x.x;i++){
		x.mat[i].resize(x.y+1,0);
	}
	for(int i=1;i<=x.x;i++){
		for(int j=1;j<=x.y;j++){
			in>>x.mat[i][j];
		}
	}
	return in;
}
ostream& operator <<(ostream& out,const matrix& x){
    for(int i=1;i<=x.x;i++){
        for(int j=1;j<=x.y;j++){
        	out<<x.mat[i][j]<<" ";
        }
        out<<endl;
    }
	return out;
}
matrix operator *(const matrix& a,const matrix& b){
    matrix ret;
    ret.x=a.x,ret.y=b.y;
    for(int i=1;i<=ret.x;i++){
    	ret.mat[i].resize(ret.y+1,0);
	}
    for(int i=1;i<=a.x;i++){
    	for(int j=1;j<=b.y;j++){
    		for(int k=1;k<=a.y;k++){
    			ret.mat[i][j]=ret.mat[i][j]+a.mat[i][k]*b.mat[k][j];
			}
		}
	}
    return ret;
}
int main(){
	scanf("%d%d",&n,&d);
	q.x=k.y=v.x=n;
	q.y=k.x=v.y=d;
	cin>>q;
	for(int i=1;i<=d;i++){
		k.mat[i].resize(n+1);
	}
	for(int i=1;i<=n;i++){
		for(int j=1;j<=d;j++){
			cin>>k.mat[j][i];
		}
	}
	cin>>v;
	matrix ans=q*(k*v);
	for(int i=1;i<=n;i++){
		long long x;
		scanf("%lld",&x);
		for(int j=1;j<=d;j++){
			ans.mat[i][j]*=x;
		}
	}
	cout<<ans;
	return 0;
}

#include<bits/stdc++.h>
#define rep(i,x,y) for(int i=x;i<=y;i++)
#define dwn(i,x,y) for(int i=x;i>=y;i--)
#define ll long long
using namespace std;
template<typename T>inline void qr(T &x){
    x=0;int f=0;char s=getchar();
    while(!isdigit(s))f|=s=='-',s=getchar();
    while(isdigit(s))x=x*10+s-48,s=getchar();
    x=f?-x:x;
}
int cc=0,buf[31];
template<typename T>inline void qw(T x){
    if(x<0)putchar('-'),x=-x;
    do{buf[++cc]=int(x%10);x/=10;}while(x);
    while(cc)putchar(buf[cc--]+'0');
}
const int N=110;
const double eps=1e-9;
int n,m;
map<string,int>f;
double a[N][N];
int getid(string now){
    if(f.find(now)==f.end()){
        f[now]=++n;
        return n;
    }
    return f[now];
}
void solve(){
    qr(m);
    n=0;f.clear();
    rep(i,1,50)rep(j,1,50)a[i][j]=0;
    rep(i,1,m){
        string t;cin>>t;
        string name="";int sum=0;
        for(int j=0;j<t.length();j++){
            char w=t[j];
            if(isdigit(w)){
                sum=sum*10+(w-'0');
            }
            else{
                if(!j||(j&&!isdigit(t[j-1]))){
                    name=name+w;
                }
                else{
                    int id=getid(name);
                    a[id][i]=1.0*sum;
                    name="";name=name+w;
                    sum=0;
                }
            }
        }
        int id=getid(name);
        a[id][i]=1.0*sum;
    }
    if(n<m){
        puts("Y");
        return;
    }
    rep(i,1,m){
        int k=0;
        rep(j,i,n)if(fabs(a[j][i])>eps){k=j;break;}
        if(!k){
            puts("Y");
            return;
        }
        rep(j,1,m)swap(a[i][j],a[k][j]);
        rep(j,i+1,n){
            double w=a[j][i]/a[i][i];
            rep(k,1,m)a[j][k]-=a[i][k]*w;
        }
    }
    puts("N");
}
int main(){
    int tt;qr(tt);
    while(tt--)solve();
    return 0;
}
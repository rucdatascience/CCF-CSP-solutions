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
int n,m;
int ans1[N],ans2[N];
bool v[N];
void solve(){
    qr(n),qr(m);
    rep(i,1,n){
        rep(j,1,m)v[j]=0;
        int t;qr(t);
        while(t--){
            int x;qr(x);
            if(!v[x]){
                v[x]=1;ans1[x]++;
            }
            ans2[x]++;
        }
    }
    rep(i,1,m)qw(ans1[i]),putchar(' '),qw(ans2[i]),puts("");
}
int main(){
    int tt;tt=1;
    while(tt--)solve();
    return 0;
}
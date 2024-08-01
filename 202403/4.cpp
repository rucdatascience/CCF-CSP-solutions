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
const int N=2e5+10;
int n,m,k;
set<int>s;
set<int>::iterator it;
map<int,int>f;
void work(int x){
    int pre=*(--s.lower_bound(x));
    int nxt=*(s.upper_bound(x));
    if(pre!=0)f[pre]++;
    if(nxt!=n+1)f[nxt]++;
    f[x]=0;s.erase(s.find(x));
    if(pre!=0&&f[pre]>=5)work(pre);
    if(nxt!=n+1&&f[nxt]>=5)work(nxt);
}
void solve(){
    qr(n),qr(m),qr(k);
    s.insert(0);
    s.insert(n+1);
    rep(i,1,m){
        int x,y;qr(x),qr(y);
        f[x]=y;s.insert(x);
    }
    while(k--){
        int x;qr(x);
        f[x]++;
        if(f[x]==1)s.insert(x);
        if(f[x]>=5)work(x);
        qw(s.size()-2);puts("");
    }
}
int main(){
    int tt;tt=1;
    while(tt--)solve();
    return 0;
}
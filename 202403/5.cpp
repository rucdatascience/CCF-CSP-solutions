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
const int N=5e5+10;
int n,m;ll a[N];int siz[N];
int fa[N];vector<int>e[N];
ll c1[N];int c[N];
int z,L[N],R[N],yss[N];
int dep[N];
void dfs(int x){
    L[x]=++z;
    yss[z]=x;
    for(int y:e[x]){
        dep[y]=dep[x]+1;
        dfs(y);
    }
    R[x]=z;
}
void add(int x,ll w){
    for(int i=x;i<=n;i+=i&-i){
        c1[i]+=w;
    }
}
ll calc(int x){
    ll ret=0;
    for(int i=x;i;i-=i&-i){
        ret+=c1[i];
    }
    return ret;
}
void add2(int x,int w){
    for(;x<=n;x+=x&-x){
        c[x]+=w;
    }
}
int calc2(int x){
    int ret=0;
    for(;x;x-=x&-x)ret+=c[x];
    return ret;
}
void solve(){
    qr(n),qr(m);
    rep(i,2,n){
        qr(fa[i]);
        e[fa[i]].push_back(i);
    }
    rep(i,1,n)qr(a[i]);
    rep(i,1,n)siz[i]=e[i].size();
    dep[1]=1;dfs(1);
    rep(i,1,n){
        add(L[i],(ll)dep[i]);
        add(L[i]+1,(ll)-dep[i]);
        add2(i,1);
    }
    while(m--){
        int op,x;qr(op),qr(x);
        if(op==1){
            int now=L[x]+1;
            while(now<=R[x]){
                int l=now,r=R[x],mid,pos=-1;
                while(l<=r){
                    mid=(l+r)/2;
                    if(calc2(mid)-calc2(l-1)>=1)pos=mid,r=mid-1;
                    else l=mid+1;
                }
                if(pos==-1)break;
                else{
                	int son=yss[pos];
                    siz[x]=siz[x]-1+siz[son];
                    a[x]+=a[son];
                    add2(pos,-1);
                    add(L[son],-1);
                    add(R[son]+1,1);
                    now=R[son]+1;
                }
            }
            qw(siz[x]),putchar(' ');
            qw(a[x]),puts("");
        }
        else{
            qw(calc(L[x]));puts("");
        }
    }
}
int main(){
    int tt;tt=1;
    while(tt--)solve();
    return 0;
}
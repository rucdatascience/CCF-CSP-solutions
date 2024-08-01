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
const int N=1e4+10;
int n,m;
set<string>s1,s2;
set<string>::iterator it;
void solve(){
    qr(n),qr(m);
    rep(i,1,n){
        string x;cin>>x;
        string now="";
        for(int j=0;j<x.length();j++){
            char w=x[j];
            if('a'<=w&&w<='z')w=w-'a'+'A';
            now=now+w;
        }
        s1.insert(now);
    }
    rep(i,1,m){
        string x;cin>>x;
        string now="";
        for(int j=0;j<x.length();j++){
            char w=x[j];
            if('a'<=w&&w<='z')w=w-'a'+'A';
            now=now+w;
        }
        s2.insert(now);
    }
    int ans1=0,ans2=s1.size();
    for(it=s2.begin();it!=s2.end();it++){
        string now=*it;
        if(s1.find(now)!=s1.end())ans1++;
        else ans2++;
    }
    cout<<ans1<<endl<<ans2<<endl;
}
int main(){
    int tt;tt=1;
    while(tt--)solve();
    return 0;
}
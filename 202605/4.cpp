#include<bits/stdc++.h>
using namespace std;
struct FastIO{
    static const int S=1<<16;
    char in[S],out[S];int at=0,len=0,used=0;
    char get(){if(at==len){len=fread(in,1,S,stdin);at=0;}return len?in[at++]:0;}
    int read(){int x=0;char c=get();while(c<'0'||c>'9')c=get();while(c>='0'&&c<='9'){x=x*10+c-'0';c=get();}return x;}
    void put(char c){if(used==S){fwrite(out,1,used,stdout);used=0;}out[used++]=c;}
    void write(int x){char s[12];int k=0;do{s[k++]=char('0'+x%10);x/=10;}while(x);while(k)put(s[--k]);put('\n');}
    ~FastIO(){fwrite(out,1,used,stdout);}
}io;
int main(){
    int n=io.read(),q=io.read();
    vector<pair<int,int>>v[2];
    v[0].push_back(make_pair(0,0));v[1].push_back(make_pair(0,-1));
    int pref[2]={0,0};
    for(int i=1;i<=n;i++){pref[i&1]^=io.read();v[i&1].push_back(make_pair(pref[i&1],i));}
    vector<vector<int>>jump(20,vector<int>(n+3,n+2));
    for(int p=0;p<2;p++){
        sort(v[p].begin(),v[p].end());
        // 相同奇偶前缀异或值的下一次出现，给出该起点的最短必胜区间。
        for(int i=1;i<(int)v[p].size();i++)if(v[p][i-1].first==v[p][i].first)
            jump[0][v[p][i-1].second+2]=v[p][i].second+1;
    }
    // 可以跳过零贡献区间，选择整个后缀中结束最早的必胜区间。
    for(int i=n;i>=1;i--)jump[0][i]=min(jump[0][i],jump[0][i+1]);
    for(int k=1;k<20;k++)for(int i=1;i<=n+2;i++)jump[k][i]=jump[k-1][jump[k-1][i]];
    while(q--){
        int l=io.read(),r=io.read(),ans=0;
        for(int k=19;k>=0;k--)if(jump[k][l]<=r+1){ans+=1<<k;l=jump[k][l];}
        io.write(ans);
    }
    return 0;
}

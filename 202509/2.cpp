#include<bits/stdc++.h>
using namespace std;
const int c[5][9]={
    {1,1,1,1,1,1,1,1,1},
    {1,0,0,1,0,0,1,0,1},
    {1,0,0,1,1,1,1,1,0},
    {1,0,0,0,0,1,1,0,0},
    {1,1,1,1,1,1,1,0,0}
};
const int N=222;
int a[N][N],n,L;
int main() {
    vector<pair<int,int>> ok;
    scanf("%d%d",&n,&L);
    for(int i=1;i<=n;i++)
        for(int j=1;j<=n;j++)
            scanf("%d",&a[i][j]);
    for(int x=1;x+4<=n;x++)
        for(int y=1;y+8<=n;y++) {
            int l=0,r=1e9; // l 是 c[][]=0 的最大值+1
            for(int i=0;i<5;i++)
                for(int j=0;j<9;j++)
                    if(c[i][j]==1) r=min(r,a[x+i][y+j]);
                    else l=max(l,a[x+i][y+j]+1);
            if(l<=r) ok.push_back({l,r});
        }

    //  所有区间取并集 
    sort(ok.begin(),ok.end());
    int R=-1;
    for(pair<int,int> o:ok) {
        int l=o.first,r=o.second;
        if(l>R) {
            for(int i=l;i<=r;i++) printf("%d\n",i);
            R=r;
        }
        else if(r>R) {
            for(int i=R+1;i<=r;i++) printf("%d\n",i);
            R=r;
        }
    }
    return 0;
}
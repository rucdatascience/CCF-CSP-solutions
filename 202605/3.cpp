#include<bits/stdc++.h>
using namespace std;
struct Process{
    char type;
    int start,w,k,pos,remain,failed,finish;
    bool released;
    long long gain;
    vector<int>a,t;
}p[11];
int n,m,owner[41];
void release(int i){
    for(int x=1;x<=m;x++)if(owner[x]==i)owner[x]=0;
}
int main(){
    cin>>n>>m;
    for(int i=1;i<=n;i++){
        cin>>p[i].type>>p[i].start;
        if(p[i].type!='X')cin>>p[i].w;
        cin>>p[i].k;
        p[i].a.resize(p[i].k);p[i].t.resize(p[i].k);
        for(int j=0;j<p[i].k;j++)cin>>p[i].a[j]>>p[i].t[j];
        p[i].finish=-1;
    }
    for(int day=1;;day++){
        vector<int>ask[41],steal[41];
        bool free_at_start[41],success[11]={},forced[11]={},drop[11]={};
        for(int x=1;x<=m;x++)free_at_start[x]=(owner[x]==0);
        for(int i=1;i<=n;i++)if(p[i].start<=day&&p[i].pos<p[i].k&&!p[i].remain){
            int x=p[i].a[p[i].pos];
            // C 在已经失败 w 次后的下一段首执行抢夺。
            if(p[i].type=='C'&&p[i].failed==p[i].w)steal[x].push_back(i);
            else ask[x].push_back(i);
        }
        // 先统一裁决所有申请，避免按进程顺序模拟破坏同时性。
        for(int x=1;x<=m;x++){
            if(!steal[x].empty()){
                int winner=steal[x].back();
                owner[x]=winner;
                for(int i:steal[x])forced[i]=true;
            }else if(free_at_start[x]&&!ask[x].empty()){
                int winner=ask[x][0];owner[x]=winner;success[winner]=true;
            }
        }
        for(int i=1;i<=n;i++)if(p[i].start<=day&&p[i].pos<p[i].k&&!p[i].remain){
            if(success[i]||forced[i])p[i].remain=p[i].t[p[i].pos];
            else{
                if(p[i].failed<p[i].w)p[i].failed++;
                if(p[i].type=='B'&&p[i].failed==p[i].w)p[i].remain=p[i].t[p[i].pos];
                if(p[i].type=='A'&&!p[i].released&&p[i].failed==p[i].w){
                    drop[i]=true;p[i].released=true;
                }
            }
        }
        // 段中：收益按抢夺裁决后实际占有的资源数计算。
        int held[11]={};
        for(int x=1;x<=m;x++)held[owner[x]]++;
        for(int i=1;i<=n;i++)if(p[i].remain){
            p[i].gain+=held[i];
            if(--p[i].remain==0){
                p[i].pos++;p[i].failed=0;p[i].released=false;
                if(p[i].pos==p[i].k){p[i].finish=day;drop[i]=true;}
            }
        }
        // 段末统一释放，当天申请阶段看不到这些释放。
        for(int i=1;i<=n;i++)if(drop[i])release(i);
        bool future=false;
        for(int i=1;i<=n;i++)if(p[i].pos<p[i].k){
            if(p[i].start>day||p[i].remain||p[i].type=='B'||p[i].type=='C'
                ||(p[i].type=='A'&&!p[i].released)||owner[p[i].a[p[i].pos]]==0)
                future=true;
        }
        // 没有未来启动/任务/超时事件，且等待资源均被占用，状态永久不变。
        if(!future)break;
    }
    for(int i=1;i<=n;i++)cout<<p[i].gain<<' '<<(p[i].finish<0?-1:p[i].finish-p[i].start+1)<<'\n';
    return 0;
}

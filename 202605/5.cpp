#include<bits/stdc++.h>
using namespace std;
// 活动串贪心 + 候选起点筛选。历史事件按根所在的欧拉区间维护。
// 算法参考讨论：https://pcs2.roj.ac.cn/problems/shumeng/CSP202605E/
// 此实现使用标准 set、链表拼接和迭代树遍历，兼容 C++14。
const int INF=1000000007;
int n;
vector<vector<int>>g;

struct Tree{
    vector<int>fa,dep,in,out,seq;
    vector<vector<int>>up;
    Tree():fa(n,-1),dep(n),in(n),out(n),up(18,vector<int>(n)){
        vector<pair<int,int>>s(1,make_pair(0,0));
        while(!s.empty()){
            int u=s.back().first,&i=s.back().second;
            if(i==0){in[u]=seq.size();seq.push_back(u);up[0][u]=fa[u]<0?u:fa[u];
                for(int k=1;k<18;k++)up[k][u]=up[k-1][up[k-1][u]];}
            if(i==(int)g[u].size()){out[u]=seq.size();s.pop_back();continue;}
            int v=g[u][i++];if(v==fa[u])continue;
            fa[v]=u;dep[v]=dep[u]+1;s.push_back(make_pair(v,0));
        }
    }
    bool anc(int u,int v){return in[u]<=in[v]&&in[v]<out[u];}
    int lift(int u,int d){for(int k=0;k<18;k++)if(d>>k&1)u=up[k][u];return u;}
    int lca(int u,int v){
        if(dep[u]<dep[v])swap(u,v);
        u=lift(u,dep[u]-dep[v]);
        if(u==v)return u;
        for(int k=17;k>=0;k--)if(up[k][u]!=up[k][v])u=up[k][u],v=up[k][v];
        return fa[u];
    }
    int dist(int u,int v){return dep[u]+dep[v]-2*dep[lca(u,v)];}
    int toward(int u,int v){return anc(u,v)?lift(v,dep[v]-dep[u]-1):fa[u];}
};
struct BIT{
    vector<int>a;
    BIT():a(n+2){}
    void add(int x,int v){for(++x;x<(int)a.size();x+=x&-x)a[x]+=v;}
    int get(int x){int s=0;for(++x;x;x-=x&-x)s+=a[x];return s;}
};
struct DSU{
    vector<int>p;
    DSU(int size):p(size){iota(p.begin(),p.end(),0);}
    int find(int x){int r=x;while(p[r]!=r)r=p[r];while(p[x]!=x){int y=p[x];p[x]=r;x=y;}return r;}
};
struct Alive:DSU{
    vector<char>on;int count;
    Alive():DSU(n+1),on(n,1),count(n){}
    void erase(int x){if(on[x]){on[x]=0;--count;p[x]=find(x+1);}}
    int first(){return find(0);}
};

// 固定首个聚拢点后，红点表示能在活动串前面添加更小字符的操作。
vector<int>fixed(int root){
    vector<int>fa(n,-1),order(1,root),mn(n,INF),rp(n,n),skip(n+1);
    vector<vector<int>>children(n),bucket(n);
    for(int i=0;i<n;i++){
        int u=order[i];
        for(int v:g[u])if(v!=fa[u])fa[v]=u,children[u].push_back(v),order.push_back(v);
    }
    deque<int>ans;vector<int>first=g[root];first.push_back(root);sort(first.begin(),first.end());
    for(int x:first)ans.push_back(x);
    int head=ans.front();
    vector<char>red(n),open(n),active(n);
    vector<list<int>>kids(n+1);
    vector<list<int>::iterator>where(n);
    set<pair<int,int>>front,top;
    iota(skip.begin(),skip.end(),0);
    for(int u:order)if(u!=root&&!children[u].empty()){
        active[u]=1;mn[u]=children[u][0];red[u]=mn[u]<head;
        if(red[u])bucket[mn[u]].push_back(u);
    }
    vector<int>last(n,n);
    for(int u:order){
        int prev=u==root?n:last[fa[u]];
        if(red[u]){rp[u]=prev;kids[prev].push_back(u);where[u]=--kids[prev].end();
            if(prev==n)top.insert(make_pair(mn[u],u));
            last[u]=u;}
        else last[u]=prev;
    }
    function<int(int)>find=[&](int x){
        int r=x;while(skip[r]!=r)r=skip[r];while(skip[x]!=x){int y=skip[x];skip[x]=r;x=y;}return r;
    };
    auto remove_red=[&](int x){
        if(!red[x])return;
        int p=find(rp[x]);red[x]=0;skip[x]=p;
        kids[p].erase(where[x]);
        if(p==n){top.erase(make_pair(mn[x],x));for(int y:kids[x])top.insert(make_pair(mn[y],y));}
        // splice 不逐个搬移红色后代，避免退化成平方复杂度。
        kids[p].splice(kids[p].end(),kids[x]);
    };
    auto expose=[&](int u){for(int v:children[u])if(active[v]){open[v]=1;front.insert(make_pair(mn[v],v));}};
    expose(root);
    while((int)ans.size()<n){
        int bad=top.empty()?-1:top.rbegin()->second,white=-1,tail=INF;
        auto it=front.upper_bound(make_pair(head,INF));if(it!=front.end())white=it->second;
        if(bad!=-1){auto j=lower_bound(children[bad].begin(),children[bad].end(),head);if(j!=children[bad].end())tail=*j;}
        int u=white!=-1&&(bad==-1||!open[bad]||(tail!=INF&&mn[white]<tail))?white:bad;
        assert(u!=-1&&open[u]);
        front.erase(make_pair(mn[u],u));open[u]=0;remove_red(u);active[u]=0;
        int cut=lower_bound(children[u].begin(),children[u].end(),head)-children[u].begin();
        for(int j=cut-1;j>=0;j--)ans.push_front(children[u][j]);
        for(int j=cut;j<(int)children[u].size();j++)ans.push_back(children[u][j]);
        expose(u);int next=ans.front();
        while(head>next){--head;for(int x:bucket[head])remove_red(x);}
    }
    return vector<int>(ans.begin(),ans.end());
}

struct History{
    Tree &tr;int size=1;
    vector<set<pair<int,int>>>s;
    vector<int>key;
    History(Tree &t):tr(t),key(n,-1){while(size<n)size*=2;s.resize(size*2);}
    void range(int l,int r,int u,bool add){
        pair<int,int>p=make_pair(key[u],u);
        for(l+=size,r+=size;l<r;l/=2,r/=2){
            if(l&1){if(add)s[l].insert(p);else s[l].erase(p);l++;}
            if(r&1){--r;if(add)s[r].insert(p);else s[r].erase(p);}
        }
    }
    void change(int u,int v,bool add){
        key[u]=v;int z=g[u][0];
        int l,r;
        if(tr.fa[z]==u)l=tr.in[z],r=tr.out[z];else l=tr.in[u],r=tr.out[u];
        bool inside=(v==z)!=(tr.fa[z]==u);
        if(inside)range(l,r,u,add);else{range(0,l,u,add);range(r,n,u,add);}
    }
    int query(int root,int head){
        pair<int,int>best=make_pair(-1,-1);
        for(int p=tr.in[root]+size;p;p/=2){
            auto it=s[p].lower_bound(make_pair(head,-1));
            if(it!=s[p].begin())best=max(best,*--it);
        }
        return best.second;
    }
};

// 已消费完的中心构成若干连通块；小并大维护其边界。
struct Components:DSU{
    vector<int>cnt,mn,second;
    vector<char>on;
    vector<set<int>>edge;
    vector<set<pair<int,int>>>choice;
    Components():DSU(n),cnt(n,1),mn(n),second(n,INF),on(n),edge(n),choice(n){iota(mn.begin(),mn.end(),0);}
    int value(int u,int parent){return g[u][0]!=parent?g[u][0]:g[u].size()>1?g[u][1]:INF;}
    void insert_min(int u,int x){if(x<mn[u])second[u]=mn[u],mn[u]=x;else if(x!=mn[u])second[u]=min(second[u],x);}
    void join(int x,int y){
        x=find(x);y=find(y);if(x==y)return;
        if(cnt[x]+edge[x].size()<cnt[y]+edge[y].size())swap(x,y);
        p[y]=x;cnt[x]+=cnt[y];insert_min(x,mn[y]);insert_min(x,second[y]);
        edge[x].insert(edge[y].begin(),edge[y].end());edge[y].clear();
        choice[x].insert(choice[y].begin(),choice[y].end());choice[y].clear();
    }
    void add(int u){
        on[u]=1;
        for(int v:g[u])if(!on[v]){
            edge[u].insert(v);int w=value(v,u);if(w!=INF)choice[u].insert(make_pair(w,v));
        }
        for(int v:g[u])if(on[v]){
            int r=find(v);edge[r].erase(u);choice[r].erase(make_pair(value(u,v),u));join(u,r);
        }
    }
    int low(int u,int root){u=find(u);int x=mn[u]==root?second[u]:mn[u];return edge[u].empty()?x:min(x,*edge[u].begin());}
    int next(int u,int h){u=find(u);auto it=choice[u].upper_bound(make_pair(h,INF));return it==choice[u].end()?-1:it->second;}
};

struct Search{
    Tree tr;BIT bit;DSU direction;Alive roots,todo;History history;Components done;
    vector<int>to,mark,stack,seen,label,event,tag,when;
    vector<char>used,red;
    vector<set<int>>left;
    vector<pair<int,int>>pending;
    vector<int>hidden;
    int clock=0,root=-1,last=-1,cut=0,head=0,block=-1,limit=0;
    bool running=false,root_done=false;
    Search():direction(n),history(tr),to(n,-1),mark(n),seen(n),label(n),event(n,-1),tag(n,-1),when(n,-1),used(n),red(n),left(n){
        iota(label.begin(),label.end(),0);
        for(int u=0;u<n;u++)left[u].insert(g[u].begin(),g[u].end());
    }
    bool eligible(int u,int v){return v==g[u][0]||(g[u].size()>1&&v==g[u][1]);}
    void activate(int u,int v){if(done.on[u]||red[u]||!eligible(u,v))return;history.change(u,v,true);red[u]=1;}
    void deactivate(int u){if(red[u])history.change(u,tag[u],false),red[u]=0;}
    void flush(){for(auto e:pending)activate(e.second,e.first);pending.clear();}
    void restore(){for(int u:hidden)if(!done.on[u])activate(u,tag[u]);hidden.clear();}
    void record(int u,int from){
        if(from<0)return;
        int id=clock++;
        label[u]=from;event[u]=id;
        if(!eligible(u,from))return;
        tag[u]=from;when[u]=id;
        if(running)pending.push_back(make_pair(from,u));else activate(u,from);
    }
    void mark_prefix(int u,int from=-1){
        if(u<0)return;
        if(!mark[u]){mark[u]=1;stack.push_back(u);bit.add(tr.in[u],1);bit.add(tr.out[u],-1);record(u,from);}
        else if(from!=-1&&label[u]<from&&from<u&&!(running&&from==root)&&!mark[from]&&event[u]!=-1){
            mark[from]=1;stack.push_back(from);bit.add(tr.in[from],1);bit.add(tr.out[from],-1);
            label[from]=label[u];label[u]=from;event[from]=event[u];
        }
    }
    void orient(int u,int parent){
        vector<pair<int,int>>s(1,make_pair(u,parent));
        while(!s.empty()){
            int x=s.back().first,p=s.back().second;s.pop_back();if(!roots.on[x])continue;
            to[x]=p;direction.p[direction.find(x)]=direction.find(p);roots.erase(x);
            for(int y:g[x])if(y!=p&&roots.on[y])s.push_back(make_pair(y,x));
        }
    }
    bool valid(int u,int target=-1){
        if(roots.on[u])return true;
        if(to[u]<0)return false;
        if(to[to[u]]<0)return true;
        int a=to[u],b=direction.find(u),c=tr.lca(a,b);
        int count=bit.get(tr.in[a])+bit.get(tr.in[b])-2*bit.get(tr.in[c])+mark[c];
        if(count==0)return true;
        return count==1&&mark[a]&&!stack.empty()&&stack.back()==a&&(target<0||tr.dist(target,u)<=2);
    }
    void consume(int u){if(used[u])return;used[u]=1;for(int v:g[u])left[v].erase(u);}
    int parent(int u){return u==root?-1:tr.toward(u,root);}
    int minimum(int u){return done.value(u,parent(u));}
    bool open(int u){return u!=root&&!done.on[u]&&done.on[parent(u)]&&done.find(parent(u))==done.find(root);}
    void finish(int u){if(!done.on[u]){deactivate(u);done.add(u);}head=min(head,done.low(u,root));}
    int peek(){
        for(;;){
            if(!root_done){
                int x=used[root]?INF:root;if(!left[root].empty())x=min(x,*left[root].begin());
                if(x!=INF)return x;
                finish(root);root_done=true;
            }
            if(block!=-1){auto it=left[block].lower_bound(limit);if(it!=left[block].end())return *it;finish(block);block=-1;}
            int bad=head>0?history.query(root,head):-1,tail=INF;
            if(bad!=-1){auto it=lower_bound(g[bad].begin(),g[bad].end(),head);if(it!=g[bad].end()&&*it==parent(bad))++it;if(it!=g[bad].end())tail=*it;}
            int white=done.next(root,head);
            int u=white!=-1&&(bad==-1||!open(bad)||(tail!=INF&&minimum(white)<tail))?white:bad;
            if(u<0)return INF;
            assert(open(u));block=u;limit=head;int v=minimum(u);
            if(v<head){if(when[u]!=-1&&when[u]<cut&&tag[u]==v&&red[u])deactivate(u),hidden.push_back(u);head=v;}
        }
    }
    void start(int u){
        if(running)return;
        restore();flush();root=u;running=true;cut=clock;
        root_done=false;block=-1;limit=0;head=min(u,g[u][0]);
    }
    int compare(int x){
        if(!running)return -1;
        int y=peek();if(y==INF)return -1;
        if(y<x)return root;
        if(y>x){running=false;restore();flush();}else consume(y);
        return -1;
    }
    int process(int x){
        for(int y:g[x])if(seen[y]&&to[y]<0)start(y);
        todo.erase(x);
        while(todo.count){
            int y=todo.first();
            if(!roots.on[y]&&!valid(y,x)){todo.erase(y);continue;}
            int win=compare(y);if(win!=-1)return win;
            int d=tr.dist(x,y);
            if(d>=3){
                int u=tr.toward(x,y),v=tr.toward(u,y);
                if(!roots.on[v]){todo.erase(y);continue;}
                if(last!=-1&&u!=last)mark_prefix(x,last),last=-1;
                orient(u,v);mark_prefix(to[x],x);
            }else if(d==2){
                int u=tr.toward(x,y);if(!roots.on[u]){todo.erase(y);continue;}
                if(last!=-1&&u!=last)mark_prefix(x,last);
                orient(x,u);mark_prefix(to[x],x);
            }else{
                if(last!=-1&&y!=last)mark_prefix(x,last);
                for(int v:g[x])if(v!=y&&to[v]!=x)orient(v,x);
                last=x;
            }
            break;
        }
        return -1;
    }
    vector<int>solve(){
        while(roots.count>2){
            int x=todo.first();assert(x<n);seen[x]=1;consume(x);
            if(roots.on[x]){int win=process(x);if(win!=-1)return fixed(win);}
            else{
                if(!valid(x)){todo.erase(x);continue;}
                mark_prefix(to[x],x);todo.erase(x);
                if(roots.on[to[x]])while(todo.count){
                    int y=todo.first();if(!roots.on[y]&&!valid(y,x)){todo.erase(y);continue;}
                    int win=compare(y);if(win!=-1)return fixed(win);
                    if(tr.dist(x,y)>=3){
                        int u=tr.toward(x,y),v=tr.toward(u,y);
                        if(u!=to[x]){todo.erase(y);continue;}orient(u,v);
                    }
                    break;
                }
            }
        }
        vector<int>ans(n,INF);
        for(int u=roots.first();u<n;u=roots.find(u+1))ans=min(ans,fixed(u));
        if(running)ans=min(ans,fixed(root));
        return ans;
    }
};
int main(){
    ios::sync_with_stdio(false);cin.tie(nullptr);
    cin>>n;g.resize(n);
    for(int i=1,u,v;i<n;i++){cin>>u>>v;--u;--v;g[u].push_back(v);g[v].push_back(u);}
    for(auto &v:g)sort(v.begin(),v.end());
    Search solver;vector<int>ans=solver.solve();
    for(int i=0;i<n;i++)cout<<ans[i]+1<<" \n"[i==n-1];
    return 0;
}

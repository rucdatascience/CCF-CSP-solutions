/*
CSP202209_4 吉祥物投票
得分：100
*/
#include<bits/stdc++.h>
using namespace std;

struct seg {
    int id;
    int l, r;
    bool operator < (const seg b)const {
        return l < b.l;
    }
};
struct Node {
    int id, cnt;
    bool operator > (const Node b)const {
        if (cnt == b.cnt) return id < b.id;
        return cnt > b.cnt;
    }
};
int n, m, q, idx, op;
int tot[100010];
int p[200010], h[200010], pre[200010];
set<seg> se;
set<Node, greater<Node> > heap;

void init() {//初始化
    tot[0] = n;
    se.insert({ 0,1,n });
    for (int i = 0; i <= m; i++) {
        p[i] = h[i] = pre[i] = i;
        heap.insert({ i,tot[i] });
    }
    idx = m + 1;
}

int find(int x) {
    if (x != p[x]) p[x] = find(p[x]);
    return p[x];
}

int mid(int x) {//通过编号找到任一个id
    return h[x];
}

int idm(int x) {//通过id找到唯一的编号
    return pre[find(x)];
}

void modify(int x, int l, int r) {//操作1
    auto ita = se.upper_bound({ 0,l,0 });
    auto itb = se.upper_bound({ 0,r,0 });
    ita--;
    // cout<<(*ita).l<<(*ita).r<<endl;
    vector<seg> win;//待新插入的节点
    vector<set<seg>::iterator> wer;//待删除的节点
    win.push_back({ mid(x),l,r });
    auto temp = heap.find({ x,tot[x] });
    heap.erase(temp);
    tot[x] += (r - l + 1);
    heap.insert({ x,tot[x] });
    for (auto it = ita; it != itb; it++) wer.push_back(it);
    for (int i = 0; i < wer.size(); i++) {
        seg t = *wer[i];
        se.erase(t);
        if (l >= t.l && r <= t.r) {//[t.l  [l  r]    t.r]的情况
            if (l - 1 >= t.l) win.push_back({ t.id,t.l,l - 1 });
            if (r + 1 <= t.r) win.push_back({ t.id,r + 1,t.r });
            Node tc = *heap.find({ idm(t.id),tot[idm(t.id)] });
            heap.erase(tc);
            tot[tc.id] -= (r - l + 1);
            heap.insert({ tc.id,tot[tc.id] });
        }
        else if (l <= t.r && t.l <= l) {//[t.l  [l   t.r]   r]的情况
            if (l - 1 >= t.l) win.push_back({ t.id,t.l,l - 1 });
            Node tc = *heap.find({ idm(t.id),tot[idm(t.id)] });
            heap.erase(tc);
            tot[tc.id] -= (t.r - l + 1);
            heap.insert({ tc.id,tot[tc.id] });
        }
        else if (r <= t.r && t.l <= r) {//[l    [t.l   r]  t.r]的情况
            if (r + 1 <= t.r) win.push_back({ t.id,r + 1,t.r });
            Node tc = *heap.find({ idm(t.id),tot[idm(t.id)] });
            heap.erase(tc);
            tot[tc.id] -= (r - t.l + 1);
            heap.insert({ tc.id,tot[tc.id] });
        }
        else if (l<t.l && r>t.r) {//[l  [t.l  t.r]    r]的情况
            Node tc = *heap.find({ idm(t.id),tot[idm(t.id)] });
            heap.erase(tc);
            tot[tc.id] -= (t.r - t.l + 1);
            heap.insert({ tc.id,tot[tc.id] });
        }
    }
    for (int i = 0; i < win.size(); i++) se.insert(win[i]);
}

void exchange(int x, int y) {//操作3
    heap.erase({ x,tot[x] }), heap.erase({ y,tot[y] });
    int a = h[x], b = h[y];
    pre[a] = y, pre[b] = x;
    swap(h[x], h[y]);
    swap(tot[x], tot[y]);
    heap.insert({ x,tot[x] });
    heap.insert({ y,tot[y] });
}

void xtoy(int x, int y) {//操作2
    heap.erase({ x,tot[x] }), heap.erase({ y,tot[y] });
    p[h[x]] = h[y];
    p[idx] = idx;
    h[x] = idx;
    pre[idx++] = x;
    tot[y] += tot[x];
    tot[x] = 0;
    heap.insert({ x,tot[x] });
    heap.insert({ y,tot[y] });
}

void debug() {//没用不用管
    cout << "--------------------------------------" << endl;
    for (auto it = se.begin(); it != se.end(); it++) {
        seg t = *it;
        cout << t.l << " " << t.r << " " << idm(t.id) << "          " << tot[idm(t.id)] << endl;
    }
    cout << "--------------------------------------" << endl;

    // cout<<"--------------------------------------"<<endl;
    // for(auto it=heap.begin();it!=heap.end();it++){
    //     Node t=*it;
    //     cout<<t.cnt<<" "<<tot[t.id]<<" "<<t.id<<endl;
    // }
    // cout<<"--------------------------------------"<<endl;
}

int main() {
    cin >> n >> m >> q;
    init();
    while (q--) {
        // debug();
        cin >> op;
        if (op == 1) {
            int l, r, x;
            cin >> l >> r >> x;
            modify(x, l, r);
        }
        else if (op == 2) {
            int x, y;
            cin >> x >> y;
            xtoy(x, y);
        }
        else if (op == 3) {
            int x, y;
            cin >> x >> y;
            exchange(x, y);
        }
        else if (op == 4) {
            int x;
            cin >> x;
            cout << tot[x] << endl;
        }
        // else if(op==9){
        //     int x;
        //     cin>>x;
        //     cout<<"--->"<<idm(x)<<endl;
        // }
        else {
            if (tot[0] == n) cout << 0 << endl;
            else {
                if ((*heap.begin()).id) cout << (*heap.begin()).id << endl;
                else cout << (*(++heap.begin())).id << endl;
            }
        }
    }
    system("pause");
}
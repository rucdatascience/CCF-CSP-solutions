#include<bits/stdc++.h>
using namespace std;
#define ll long long

const int N=500005;
int n;
ll m,a[N],b[N];

using P=pair<ll,ll>;

inline P add(P x,P y){
    return {x.first+y.first,x.second+y.second};
}

inline P sub(P x,P y){
    return {x.first-y.first,x.second-y.second};
}

// w = 2 * lambda
// 返回 {完成题数, 实际总花费}
inline P calc(ll w){
    priority_queue<P,vector<P>,greater<P>> q;

    // pair 的第一维是调整后的费用
    // 第二维记 -完成题数，方便平局时统一处理
    P f={0,0};

    for(int i=1;i<=n;i++){
        // 当天什么都不做 / 当天造完并立即验完
        P c=min(P{0,0},P{2*(a[i]+b[i])-w,-1});

        P low=sub(c,P{2*b[i]-w,-1});
        P high=sub(P{2*a[i],0},c);

        f=add(f,c);

        if(!q.empty()&&q.top()<low){
            P x=q.top();
            q.pop();

            f=add(f,sub(x,low));
            q.push(low);
        }

        q.push(high);
    }

    ll cnt=-f.second;
    ll cost=(f.first+w*cnt)/2;

    return {cnt,cost};
}

int main(){
    scanf("%d%lld",&n,&m);

    for(int i=1;i<=n;i++)
        scanf("%lld",&a[i]);

    for(int i=1;i<=n;i++)
        scanf("%lld",&b[i]);

    ll l=0,r=1;

    // 找到一个奖励足够大的位置
    while(calc(r).first<n)
        r<<=1;

    // 找到实际花费仍不超过 m 的最大整数奖励位置
    while(l<r){
        ll mid=(l+r+1)>>1;
        if(calc(mid).second<=m)
            l=mid;
        else
            r=mid-1;
    }

    P x=calc(l);

    if(x.first==n){
        printf("%d\n",n);
        return 0;
    }

    P y=calc(l+1);

    if(y.second<=m){
        printf("%lld\n",y.first);
        return 0;
    }

    if(y.first==x.first){
        printf("%lld\n",x.first);
        return 0;
    }

    // f(k) 是离散凸函数，同一段上的边际费用相同
    ll d=(y.second-x.second)/(y.first-x.first);
    ll k=(m-x.second)/d;

    k=min(k,y.first-x.first);

    printf("%lld\n",x.first+k);

    return 0;
}
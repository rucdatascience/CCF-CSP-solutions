#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
ll n;int k,m;
bool check(ll x){
    ll left=n;
    for(int i=0;i<m;i++){
        // 先变质、后食用，全程使用整数计算上取整。
        left-=(left*k+99)/100;
        if(left<x)return false;
        left-=x;
    }
    return true;
}
int main(){
    cin>>n>>k>>m;
    ll l=0,r=min(n,1000000000LL);
    while(l<r){
        ll mid=(l+r+1)/2;
        if(check(mid))l=mid;
        else r=mid-1;
    }
    cout<<l<<'\n';
    return 0;
}

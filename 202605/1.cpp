#include<bits/stdc++.h>
using namespace std;
int main(){
    int n;cin>>n;
    vector<int>x(n),y(n);
    for(int i=0;i<n;i++){
        string s;cin>>s;
        int a=stoi(s.substr(0,s.size()-2)),b=s.back()-'0';
        x[i]=a+(b>=5);
        // 恰好一半时，选择相邻的偶数。
        y[i]=a+(b>5||(b==5&&a%2==1));
    }
    for(int i=0;i<n;i++)cout<<x[i]<<" \n"[i==n-1];
    for(int i=0;i<n;i++)cout<<y[i]<<" \n"[i==n-1];
    return 0;
}

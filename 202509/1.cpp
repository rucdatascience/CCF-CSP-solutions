#include<bits/stdc++.h>
using namespace std;
int n,a,m;
int main() {
    cin>>n>>a;
    for(int i=1;i<=n;i++) {
        double x,y;
        cin>>x>>y; // 读入点坐标
        if(x*x+y*y<=1.0*a*a) m++; // 如果点在圆内部，m++
    }
    printf("%.10lf\n",4.0*m/n);
    return 0;
}
#pragma GCC optimize(3)
#include<bits/stdc++.h>
using namespace std;
int cnt=0;
int n;
long long len=0;
vector<pair<char,char> > ans;
int bit_to_int(pair<char,char> x){
	int ret=0;
	if(x.first>='a'&&x.first<='f') ret+=(((x.first-'a')+10)<<4);
	else ret+=((x.first-'0')<<4);
	if(x.second>='a'&&x.second<='f') ret+=((x.second-'a')+10);
	else ret+=(x.second-'0');
	return ret;
}
pair<char,char> read_bit(){
	char ch=0,ch1,ch2;
	while(!((ch>='0'&&ch<='9')||(ch>='a'&&ch<='f')))ch=getchar();
	if((ch>='0'&&ch<='9')||(ch>='a'&&ch<='f'))ch1=ch,ch=getchar(),ch2=ch;
	cnt++;
	return make_pair(ch1,ch2);
}
void recall(int l,int o){
	if(o>=l){
		int sz=ans.size()-1;
		for(int i=1;i<=l;i++){
			ans.push_back(ans[sz-o+i]);
		}
	}else{
		int sz=ans.size()-1,pos=ans.size()-1-o+1;
		for(int i=1;i<=l;i++){
			ans.push_back(ans[pos]);
			pos++;
			if(pos==sz+1) pos=sz-o+1;
		}
	}
}
int main(){
	scanf("%d",&n);
	while(1){
		int x;
		x=bit_to_int(read_bit());
		len+=(x&((1<<7)-1))*(1<<7);
		if(!(x&(1<<7)))break;
	}
	while(1){
		int x;
		x=bit_to_int(read_bit());
		if((x&((1<<2)-1))==0){
			x=(x>>2);
			long long len_x=x;
			if(x>=60){
				len_x=0;
				for(int i=0;i<x-59;i++){
					len_x+=bit_to_int(read_bit())*(1<<(i*8));
				}
			}
			for(int i=0;i<=len_x;i++){
				ans.push_back(read_bit());
			}
		}else if((x&((1<<1)))==(1<<1)){
			int l=(x>>2)+1;
			int o=bit_to_int(read_bit())+(bit_to_int(read_bit())<<8);
			recall(l,o);
		}else{ 
			int l=((x>>2)&((1<<3)-1))+(1<<2);
			int o=((x>>5)<<8)+bit_to_int(read_bit());
			recall(l,o);
		}
		if(cnt==n)break;
	}
	for(int i=0;i<ans.size();i++){
		printf("%c%c",ans[i].first,ans[i].second);
		if((i+1)%8==0){
			puts("");
		}
	}
	return 0;
}

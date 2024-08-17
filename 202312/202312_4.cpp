#pragma optimize(3)
#include<bits/stdc++.h>
using namespace std;
const long long mod=998244353;
inline int read(){
	int x=0,w=1;
	char ch=0;
	while(ch<'0'||ch>'9'){if(ch=='-') w=-1;ch=getchar();}
	while(ch>='0'&&ch<='9'){x=x*10+(ch-'0');ch=getchar();}
	return x*w;
}
struct matrix{
    int v[2][2];
    matrix(bool isE=false){
        v[0][0]=v[1][1]=isE;
        v[1][0]=v[0][1]=0;
    }
    matrix(int a11,int a12,int a21,int a22){
        v[0][0]=a11,v[0][1]=a12,v[1][0]=a21,v[1][1]=a22;
    }
    friend matrix operator *(const matrix& a,const matrix& b){
        matrix ans;
        ans.v[0][0]=(1ll*a.v[0][0]*b.v[0][0]+1ll*a.v[0][1]*b.v[1][0])%mod;
        ans.v[0][1]=(1ll*a.v[0][0]*b.v[0][1]+1ll*a.v[0][1]*b.v[1][1])%mod;
        ans.v[1][0]=(1ll*a.v[1][0]*b.v[0][0]+1ll*a.v[1][1]*b.v[1][0])%mod;
        ans.v[1][1]=(1ll*a.v[1][0]*b.v[0][1]+1ll*a.v[1][1]*b.v[1][1])%mod;
        return ans;
    }
    friend istream& operator >>(istream& in,matrix& x){
        return in>>x.v[0][0]>>x.v[0][1]>>x.v[1][0]>>x.v[1][1];
    }
    friend ostream& operator <<(ostream& out,const matrix& x){
        for(int i=0;i<2;i++){
            for(int j=0;j<2;j++){
                out<<x.v[i][j]<<" ";
            }
        }
		return out;
    }
};
matrix zero=matrix(1);
struct tree_node{
	matrix mat;
	int sum;
	int res;
};
tree_node uni(tree_node a,tree_node b,int tag){
	tree_node ret;
	if(tag==2) ret.mat=a.mat*b.mat;
	if(tag==1) ret.mat=b.mat*a.mat;
	ret.sum=a.sum+b.sum;
	ret.res=a.res;
	return ret;
}
struct segtree{
	matrix a[100005];
	int opt[100005];
	tree_node t[400005];
	tree_node t_ls[400005];
	tree_node query(int rt,int l,int r,int x,int y,int res,int tag){
		if(l==x&&r==y){
			if(res==0) return t[rt];
			else{
				int mid=(l+r)>>1;
				if(res>=t[rt].sum){
					return tree_node{zero,0,res-t[rt].sum+t[rt].res};
				}else if(res>=t[rt<<1|1].sum){
					return query(rt<<1,l,mid,x,mid,res-t[rt<<1|1].sum+t[rt<<1|1].res,tag);
				}else{
					tree_node ret_r=query(rt<<1|1,mid+1,r,mid+1,y,res,tag);
					tree_node ret_l=t_ls[rt];
					return uni(ret_l,ret_r,tag);
				}
			}
		}
		int mid=(l+r)>>1;
		if(y<=mid) return query(rt<<1,l,mid,x,y,res,tag);
		else if(x>mid) return query(rt<<1|1,mid+1,r,x,y,res,tag);
		else{
			tree_node ret_r=query(rt<<1|1,mid+1,r,mid+1,y,res,tag);
			tree_node ret_l=query(rt<<1,l,mid,x,mid,ret_r.res,tag);
			return uni(ret_l,ret_r,tag);
		}
	}
	void build(int rt,int l,int r,int tag){
		if(l==r){
			if(opt[l]==1) t[rt]=tree_node{a[l],1,0};
			else t[rt]=tree_node{a[l],0,1};
			return;
		}
		int mid=(l+r)>>1;
		build(rt<<1,l,mid,tag);
		build(rt<<1|1,mid+1,r,tag);
		tree_node ret_r=t[rt<<1|1];
		tree_node ret_l=query(rt<<1,l,mid,l,mid,ret_r.res,tag);
		t_ls[rt]=ret_l;
		t[rt]=uni(ret_l,ret_r,tag);
		return;
	}
	void modify(int rt,int l,int r,int x,int y,int pos,int tag){
		if(l==r){
			if(opt[l]==1) t[rt]=tree_node{a[l],1,0};
			else t[rt]=tree_node{a[l],0,1};
			return;
		}
		int mid=(l+r)>>1;
		if(pos<=mid){
			modify(rt<<1,l,mid,x,mid,pos,tag);
			tree_node ret_r=t[rt<<1|1];
			tree_node ret_l=query(rt<<1,l,mid,x,mid,ret_r.res,tag);
			t_ls[rt]=ret_l;
			t[rt]=uni(ret_l,ret_r,tag);
		}else if(pos>mid){
			modify(rt<<1|1,mid+1,r,mid+1,y,pos,tag);
			tree_node ret_r=t[rt<<1|1];
			tree_node ret_l=query(rt<<1,l,mid,x,mid,ret_r.res,tag);
			t_ls[rt]=ret_l;
			t[rt]=uni(ret_l,ret_r,tag);
		}
	}
}t1,t2;
int n,m;
int main(){
	n=read(),m=read();
	for(int i=1;i<=n;i++){
		int opt;
		opt=read();
		if(opt==1){
			int a11,a12,a21,a22;
			a11=read();a12=read();a21=read();a22=read();
			t1.opt[i]=t2.opt[i]=1;
			t1.a[i]=matrix(a11,a12,a21,a22);
			t2.a[i]=matrix(1);
		}else if(opt==2){
			int a11,a12,a21,a22;
			a11=read();a12=read();a21=read();a22=read();
			t1.opt[i]=t2.opt[i]=1;
			t1.a[i]=matrix(1);
			t2.a[i]=matrix(a11,a12,a21,a22);
		}else{
			t1.opt[i]=t2.opt[i]=-1;
			t1.a[i]=matrix(1);
			t2.a[i]=matrix(1);
		}
	}
	t1.build(1,1,n,1);
	t2.build(1,1,n,2);
	for(int i=1;i<=m;i++){
		int opt;
		opt=read();
		if(opt==2){
			int l,r;
			l=read();r=read();
			matrix ans=t1.query(1,1,n,l,r,0,1).mat*t2.query(1,1,n,l,r,0,2).mat;
			printf("%d %d %d %d\n",ans.v[0][0],ans.v[0][1],ans.v[1][0],ans.v[1][1]);
		}else{
			int iter,optt;
			iter=read(),optt=read();
			if(optt==1){
				int a11,a12,a21,a22;
				a11=read();a12=read();a21=read();a22=read();
				t1.opt[iter]=t2.opt[iter]=1;
				t1.a[iter]=matrix(a11,a12,a21,a22);
				t2.a[iter]=matrix(1);
				t1.modify(1,1,n,1,n,iter,1);
				t2.modify(1,1,n,1,n,iter,2);
			}else if(optt==2){
				int a11,a12,a21,a22;
				a11=read();a12=read();a21=read();a22=read();
				t1.opt[iter]=t2.opt[iter]=1;
				t1.a[iter]=matrix(1);
				t2.a[iter]=matrix(a11,a12,a21,a22);
				t1.modify(1,1,n,1,n,iter,1);
				t2.modify(1,1,n,1,n,iter,2);
			}else{
				t1.opt[iter]=t2.opt[iter]=-1;
				t1.a[iter]=matrix(1);
				t2.a[iter]=matrix(1);
				t1.modify(1,1,n,1,n,iter,1);
				t2.modify(1,1,n,1,n,iter,2);
			}
		}
	}
	return 0;
} 

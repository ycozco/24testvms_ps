#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
int c_max(const vector<int>& seq,const vector<vector<int>>& p){
    int n=(int)seq.size();
    int m=(int)p[0].size();
    vector<vector<int>> f(n+1,vector<int>(m+1,0));
    for(int i=1;i<=n;i++){
        for(int j=1;j<=m;j++){
            f[i][j]=max(f[i-1][j],f[i][j-1])+p[seq[i-1]][j-1];
        }
    }
    return f[n][m];
}
int best_insertion_position(const vector<int>& seq,int job,const vector<vector<int>>& p){
    int best=-1;
    int val=-1;
    for(int i=0;i<=(int)seq.size();i++){
        vector<int> test=seq;
        test.insert(test.begin()+i,job);
        int tmp=c_max(test,p);
        if(best==-1||tmp<val){
            val=tmp;
            best=i;
        }
    }
    return best;
}
vector<int> random_order(const vector<int>& seq){
    vector<int> s=seq;
    rotate(s.begin(),s.begin()+1,s.end());
    return s;
}
vector<int> LOCAL_SEARCH(const vector<int>& pi){
    vector<vector<int>> p={{3,2},{1,4},{2,3}};
    vector<int> pi_cur=pi;
    vector<int> pi_r=random_order(pi_cur);
    int k=0;
    int c=0;
    int best=c_max(pi_cur,p);
    int n=(int)pi_cur.size();
    while(c<n){
        int x=pi_r[k];
        vector<int> new_seq;
        for(auto j:pi_cur) if(j!=x) new_seq.push_back(j);
        int k_pos=best_insertion_position(new_seq,x,p);
        new_seq.insert(new_seq.begin()+k_pos,x);
        pi_cur=new_seq;
        int val=c_max(pi_cur,p);
        if(val<best){
            best=val;
            c=0;
        }else{
            c++;
        }
        k=(k+1)%n;
    }
    return pi_cur;
}
int main(){
    vector<int> seq={0,1,2};
    vector<int> s=LOCAL_SEARCH(seq);
    for(auto x:s) cout<<x<<" ";
    cout<<"\n";
}

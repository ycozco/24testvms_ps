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
vector<int> priority_order(const vector<vector<int>>& p){
    vector<pair<int,int>> s;
    for(int i=0;i<(int)p.size();i++){
        int sum_=0;
        for(auto x:p[i]) sum_+=x;
        s.push_back({sum_,i});
    }
    sort(s.begin(),s.end(),[](auto &a,auto &b){return a.first>b.first;});
    vector<int> o; 
    for(auto &x:s) o.push_back(x.second);
    return o;
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
vector<int> NEH_HEURISTIC(){
    vector<vector<int>> p={{3,2},{1,4},{2,3}};
    vector<int> po=priority_order(p);
    vector<int> seq;seq.push_back(po[0]);
    for(int k=1;k<(int)po.size();k++){
        int pos=best_insertion_position(seq,po[k],p);
        seq.insert(seq.begin()+pos,po[k]);
    }
    return seq;
}
int main(){
    vector<int> s=NEH_HEURISTIC();
    for(auto x:s) cout<<x<<" ";
    cout<<"\n";
}

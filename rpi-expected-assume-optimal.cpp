#include <bits/stdc++.h>
using namespace std;

vector<bool> alreadyIn;

//A function that returns the possibilities of switces to be made to get a deproved policies, given the IS
vector<int> findDepPossibilities(int a,int n){
    vector<int> positions;
    for(int i=0;i<n;i++){
        if(a%2==0){
            positions.push_back(i);
        }
        a>>=1;
    }
    vector<int> ret(1);
    for(int i=0;i<positions.size();i++){
        vector<int> tmp;
        for(int j=0;j<ret.size();j++){
            tmp.push_back(ret[j]);
            tmp.push_back(ret[j]+int(pow(2,positions[i])));
        }
        ret=tmp;
    }
    return ret;
}

//A function that returns the possibilities of switches to be made to get a Improved policies, given the IS(including itself)
vector<int> findImpPossibilities(int a,int n){
    vector<int> positions;
    for(int i=0;i<n;i++){
        if(a%2==1){
            positions.push_back(i);
        }
        a>>=1;
    }
    vector<int> ret(1);
    for(int i=0;i<positions.size();i++){
        vector<int> tmp;
        for(int j=0;j<ret.size();j++){
            tmp.push_back(ret[j]);
            tmp.push_back(ret[j]+int(pow(2,positions[i])));
        }
        ret=tmp;
    }
    return ret;
}


//given a policy and the places to swap xor will give next strategy
int operate(int a,int mask){
    return a^mask;
}

//given the current policy and policies already in deproved set this gives the average of all policy iterations one can take from here
double expectedIterations(int n,int currPolicy,int currPolicyLen,int opt){
    //if reached optimal policy then this is the length
    if(currPolicy==opt)return currPolicyLen;
    //find possible switches
    vector<int> switchPossible;
    for(int i=0;i<alreadyIn.size();i++){
        if(!alreadyIn[i])switchPossible.push_back(i);
    }
    double max_ret=1;
    for(int i=0;i<switchPossible.size();i++){
        //the improvement set for the switch
        int IS=switchPossible[i]^currPolicy;

        //if optimal policy falls in deproved policy set of current policy then IS is not valid.
        if(((opt^currPolicy)&(IS))==0){continue;}

        bool contin=false;
        vector<int> IPS=findImpPossibilities(IS,n);
        for(int j=0;j<IPS.size();j++){
            if(IPS[j]!=0 && alreadyIn[operate(currPolicy,IPS[j])]){contin=true;break;}
        }
        if(contin) continue;
        double ret=0;
        vector<int> deprovedSwitches=findDepPossibilities(IS,n);
        vector<int> changesMade;
        for(int j=0;j<deprovedSwitches.size();j++){
            if(!alreadyIn[operate(currPolicy,deprovedSwitches[j])]){
                alreadyIn[operate(currPolicy,deprovedSwitches[j])]=true;
                changesMade.push_back(operate(currPolicy,deprovedSwitches[j]));
            }
        }
        vector<double> ei;
        for(int j=0;j<IPS.size();j++){
            if(IPS[j]==0)continue;
            ei.push_back(expectedIterations(n,IPS[j]^currPolicy,currPolicyLen+1,opt));
            ret+=ei[ei.size()-1];
        }
        //cout<<currPolicyLen<<" "<<ret<<"/"<<ei.size()<<"\n";
        ret=ret/ei.size();
        for(int j=0;j<changesMade.size();j++){
            alreadyIn[changesMade[j]]=false;
        }
        if(ret>max_ret){
            max_ret=ret;
        }
    }
    //cout<<"Len "<<currPolicyLen<<endl;
    /*cout<<currPolicyLen<<"/"<<switchPossible.size()+1;
    for(int i=0;i<ei.size();i++){
        cout<<"+"<<ei[i]<<"/"<<ei.size()+1;
    }
    cout<<endl;*/
    return max_ret;
}

int main(){
    int n;
    cin>>n;
    //vector<int> check=findPossibilities(2,2);
    //for(int i=0;i<check.size();i++)cout<<check[i]<<" ";
    //cout<<endl;
    int max_pi=int(pow(2,n));
    double max=1;
    vector<int> opti(n+1);
    //Symmetry reduces initial testcases
    for(int i=1;i<=n;i++){
        opti[i]= opti[i]+(opti[i-1]<<1)+1;
    }
    for(int i=0;i<n+1;i++){
        vector<bool> tmp(max_pi);
        alreadyIn=tmp;
        alreadyIn[0]=true;
        double k=expectedIterations(n,0,1,opti[i]);
        cout<<k<<endl;
        if (k>max) max=k;
    }
    cout<<max<<endl;
}

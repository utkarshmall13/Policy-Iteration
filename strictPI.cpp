#include <bits/stdc++.h>
using namespace std;

vector<bool> alreadyIn;
int howard[8]={0,2,3,5,8,13,21,33};

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
int minimaxIterations(int n,int currPolicy,int currPolicyLen){
    //find possible switches
    vector<int> switchPossible;
    for(int i=0;i<alreadyIn.size();i++){
        if(!alreadyIn[i])switchPossible.push_back(i);
    }

    //prune if length is increasing more than howards upper bound as we are finding much less than it
    if(switchPossible.size()!=0 && (currPolicyLen+1)>=howard[n])return 2;

    int max_ret=0;
    for(int i=0;i<switchPossible.size();i++){
        int IS=switchPossible[i]^currPolicy;
        bool contin=false;
        vector<int> IPS=findImpPossibilities(IS,n);
        for(int j=0;j<IPS.size();j++){
            if(IPS[j]!=0 && alreadyIn[operate(currPolicy,IPS[j])]){contin=true;break;}
        }
        if(contin) continue;
        vector<int> deprovedSwitches=findDepPossibilities(IS,n);
        vector<int> changesMade;
        for(int j=0;j<deprovedSwitches.size();j++){
            if(!alreadyIn[operate(currPolicy,deprovedSwitches[j])]){
                alreadyIn[operate(currPolicy,deprovedSwitches[j])]=true;
                changesMade.push_back(operate(currPolicy,deprovedSwitches[j]));
            }
        }
        vector<int>improvementSwitches=findImpPossibilities(IS,n);
        int ret=10000000;
        for(int j=1;j<improvementSwitches.size();j++){
            int ret_new=minimaxIterations(n,improvementSwitches[j]^currPolicy,currPolicyLen+1);
            if(ret_new<ret)ret=ret_new;
        }
        if(ret==10000000)ret=0;
        for(int j=0;j<changesMade.size();j++){
            alreadyIn[changesMade[j]]=false;
        }
        if(ret>max_ret){
            max_ret=ret;
        }
    }
    return max_ret+1;
}

int main(){
    int n;
    cin>>n;
    int max_pi=int(pow(2,n));
    vector<bool> tmp(max_pi);
    alreadyIn=tmp;
    alreadyIn[0]=true;
    int k=minimaxIterations(n,0,1);
    cout<<k<<endl;
}

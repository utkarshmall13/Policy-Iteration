#include <bits/stdc++.h>
using namespace std;

vector<bool> alreadyIn;

//Formatting function to represent final policies in binary state form
string representBin(int x,int n){
    string ret="";
    while(x!=0){
        ret=char(x%2+'0')+ret;
        x>>=1;
    }
    string ret2=ret;
    for(int i=0;i<n-ret.length();i++){
        ret2=char('0')+ret2;
    }
    return ret2;
}

//A function that returns the possibilities of switces to be made to get a deproved policies, given the IS
vector<int> findPossibilities(int a,int n){
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

//given a policy and the places to swap xor will give next strategy
int operate(int a,int mask){
    return a^mask;
}

//given the current policy and policies already in deproved set this gives the average of all policy iterations one can take from here
vector<int> longestSeq(int n,int currPolicy){
    //find possible switches
    vector<int> switchPossible;
    for(int i=0;i<alreadyIn.size();i++){
        if(!alreadyIn[i])switchPossible.push_back(i);
    }

    vector<int> ret;
    for(int i=0;i<switchPossible.size();i++){
        int IS=switchPossible[i]^currPolicy;

        bool contin=false;
        int flippedPossibleOP=(int(pow(2,n))-1)^IS;
        vector<int> subsetsOfImp=findPossibilities(flippedPossibleOP,n);
        for(int j=0;j<subsetsOfImp.size();j++){
            if(subsetsOfImp[j]!=0 && alreadyIn[operate(currPolicy,subsetsOfImp[j])]){contin=true;break;}
        }
        if(contin) continue;

        vector<int> deprovedSwitches=findPossibilities(IS,n);
        vector<int> changesMade;
        for(int j=0;j<deprovedSwitches.size();j++){
            if(!alreadyIn[operate(currPolicy,deprovedSwitches[j])]){
                alreadyIn[operate(currPolicy,deprovedSwitches[j])]=true;
                changesMade.push_back(operate(currPolicy,deprovedSwitches[j]));
            }
        }
        alreadyIn[switchPossible[i]]=true;
        vector<int> tmp=longestSeq(n,switchPossible[i]);
        if(tmp.size()>ret.size())ret=tmp;
        alreadyIn[switchPossible[i]]=false;
        for(int j=0;j<changesMade.size();j++){
            alreadyIn[changesMade[j]]=false;
        }
    }
    ret.push_back(currPolicy);
    return ret;
}

int main(){
    int n;
    cin>>n;
    //vector<int> check=findPossibilities(2,2);
    //for(int i=0;i<check.size();i++)cout<<check[i]<<" ";
    //cout<<endl;
    int max_pi=int(pow(2,n));
    vector<bool> tmp(max_pi);
    alreadyIn=tmp;
    alreadyIn[0]=true;
    vector<int> policy=longestSeq(n,0);
    for(int i=0;i<policy.size();i++){
        cout<<representBin(policy[policy.size()-i-1],n)<<endl;
    }

}

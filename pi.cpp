#include <bits/stdc++.h>
using namespace std;

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

//A function that returns the possibilities of changes to be made to get a deproved policies
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


vector<int> findSeq(int last,vector<bool> &alreadyIn,vector<bool> &npossibleOp,int n){
//to find the operations with that can still be used
    //cout<<"last "<<representBin(last,n)<<endl;
    vector<int> possibleOp;
    for(int i=1;i<npossibleOp.size();i++){
        if(!npossibleOp[i]) {
            possibleOp.push_back(i);
            //cout<<"possibleOp"<<i<<endl;
        }
    }
    //cout<<endl;
    vector<int> ret;
    for(int i=0; i<possibleOp.size();i++){
//To check whether the given improvable set is actually improvable or not
//first get the subsets of improvable sets
        bool contin=false;
        int flippedPossibleOP=(int(pow(2,n))-1)^possibleOp[i];
        vector<int> subsetsOfImp=findPossibilities(flippedPossibleOP,n);
        /*cout<<"PossibleOP "<<possibleOp[i]<<endl;
        cout<<"flippedPossibleOP "<<flippedPossibleOP<<endl;
        for(int j=0;j<subsetsOfImp.size();j++){
            cout<<"\tsubsets "<<subsetsOfImp[j]<<endl;
        }*/
        for(int j=0;j<subsetsOfImp.size();j++){

            if(subsetsOfImp[j]!=0 && alreadyIn[operate(last,subsetsOfImp[j])]){contin=true;break;}
        }
        if(contin) continue;
        int x=operate(last,possibleOp[i]);
        //cout<<last<<" "<<possibleOp[i]<<" "<<i<<" "<<x<<endl;
//if found strategy not gives a deproved value move inside 'if' else check next strategy
        if(!alreadyIn[x]){
//now find and insert all new deproved policies found from this change in alreadyIn
//this act as a stack every time we move deeper into tree
//when moving back to parent node remove last count values from stack
            vector<int> nums = findPossibilities(possibleOp[i],n);
            vector<int> changesInAlreadyIn;
            vector<int> changesInNPossibleOp;
            for(int j=0;j<nums.size();j++){
                //cout<<"NUM" <<representBin(x,n)<<" "<<representBin(possibleOp[i],n)<<" "<<representBin(nums[j],n)<<endl;
                int newnum=(last&possibleOp[i])|nums[j];
                if(!alreadyIn[newnum]){
                    alreadyIn[newnum]=true;
                    changesInAlreadyIn.push_back(newnum);
                }
            }
            /*cout<<"nums ";
            for(int k=0;k<alreadyIn.size();k++){
                cout<<alreadyIn[k]<<" ";
            }
            cout<<endl;*/
//similar to alreadyIn insert superset strategies into npossibleOp
            for(int j=0;j<nums.size();j++){
                int newop=possibleOp[i]|nums[j];
                if(!npossibleOp[newop]){
                    changesInNPossibleOp.push_back(newop);
                    npossibleOp[newop]=true;
                }
            }
            /*cout<<"ops ";
            for(int k=0;k<npossibleOp.size();k++){
                cout<<npossibleOp[k]<<" ";
            }
            cout<<endl;*/

            vector<int> tmp=findSeq(x,alreadyIn,npossibleOp,n);

            //removing values belonging to siblings from stack.
            for(int j=0;j<changesInAlreadyIn.size();j++){
                alreadyIn[changesInAlreadyIn[j]]=false;
            }
            for(int j=0;j<changesInNPossibleOp.size();j++){
                npossibleOp[changesInNPossibleOp[j]]=false;;
            }
            /*if(tmp.size()==3){
                cout<<representBin(0,n)<<endl;
                cout<<representBin(last,n)<<endl;
                for(int i=tmp.size()-1;i>=0;i--){
                    cout<<representBin(tmp[i],n)<<endl;
                }
                cout<<endl;
            }*/
            if(tmp.size()>ret.size()){
                ret=tmp;
                /*cout<<"whenGreater ";
                for(int k=0;k<tmp.size();k++){
                    cout<<tmp[k]<<" ";
                }
                cout<<endl;*/
            }
        }
    }
    //if(ret.size()==0)cout<<"leaf\n";
    ret.push_back(last);
    /*cout<<"retArray ";
    for(int i=0;i<ret.size();i++){
        cout<<ret[i]<<" ";
    }
    cout<<endl;*/
    return ret;
}

int main(){
    int n;
    cin>>n;
    //vector<int> check=findPossibilities(2,2);
    //for(int i=0;i<check.size();i++)cout<<check[i]<<" ";
    //cout<<endl;
    vector <int> max;
    for(int i=1;i<=n;i++){
        vector<bool> alreadyIn(int(pow(2,n)));
        vector<bool>npossibleOp(int(pow(2,n)));
        int incby=pow(2,i);
        for(int j=0;j<int(pow(2,n-i));j++){
            alreadyIn[j*incby]=true;
            //cout<<"alreadyIn"<<j*incby<<endl;
        }
        alreadyIn[operate(0,incby-1)]=true;
        //alreadyInCount.push_back(count);
        for(int j=0;j<int(pow(2,n-i));j++){
            npossibleOp[j*incby+incby-1]=true;
            //cout<<"npossibleOp"<<j*incby+incby-1<<endl;
        }
        //npossibleOpCount.push_back(count);
        //cout<<"operations "<<incby-1<<" "<<operate(0,incby-1)<<endl;
        vector <int> tmp = findSeq(operate(0,incby-1),alreadyIn,npossibleOp,n);
        if(tmp.size()>max.size()) max=tmp;
    }
    max.push_back(0);
    for(int i=max.size()-1;i>=0;i--){
        cout<<representBin(max[i],n)<<endl;
    }
}

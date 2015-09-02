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


vector<int> findSeq(int last,vector<int> &alreadyIn,vector<int> &alreadyInCount,vector<int> &npossibleOp,vector<int> &npossibleOpCount,int n){
//to find the operations with that can still be used
    vector<bool> arr(int(pow(2,n)));
    //cout<<"last "<<representBin(last,n)<<endl;
    for(int i=0;i<npossibleOp.size();i++){
        arr[npossibleOp[i]]=true;
    }
    vector<int> possibleOp;
    for(int i=1;i<arr.size();i++){
        if(!arr[i]) {
            possibleOp.push_back(i);
            //cout<<"possibleOp"<<i<<endl;
        }
    }
    //cout<<endl;
    vector<int> ret;
    for(int i=0; i<possibleOp.size();i++){
        int x=operate(last,possibleOp[i]);
        //cout<<last<<" "<<possibleOp[i]<<" "<<i<<" "<<x<<endl;
//if found strategy not gives a deproved value move inside 'if' else check next strategy
        if(find(alreadyIn.begin(),alreadyIn.end(),x)==alreadyIn.end()){
//now find and insert all new deproved policies found from this change in alreadyIn
//this act as a stack every time we move depper into tree
//when moving back to parent node remove last count values from stack
            vector<int> nums = findPossibilities(possibleOp[i],n);
            int count=0;
            for(int j=0;j<nums.size();j++){
                //cout<<"NUM" <<representBin(x,n)<<" "<<representBin(possibleOp[i],n)<<" "<<representBin(nums[j],n)<<endl;
                int newnum=(last&possibleOp[i])|nums[j];
                if(find(alreadyIn.begin(),alreadyIn.end(),newnum)==alreadyIn.end()){
                    count++;
                    alreadyIn.push_back(newnum);
                }
            }
            /*cout<<"nums ";
            for(int k=0;k<alreadyIn.size();k++){
                cout<<alreadyIn[k]<<" ";
            }
            cout<<endl;*/
//similar to alreadyIn insert superset strategies into npossibleOp
            alreadyInCount.push_back(count);
            count=0;
            for(int j=0;j<nums.size();j++){
                int newop=possibleOp[i]|nums[j];
                if(find(npossibleOp.begin(),npossibleOp.end(),newop)==npossibleOp.end()){
                    count++;
                    npossibleOp.push_back(newop);
                }
            }
            /*cout<<"ops ";
            for(int k=0;k<npossibleOp.size();k++){
                cout<<npossibleOp[k]<<" ";
            }
            cout<<endl;*/


//removing values belonging to siblings from stack.
            npossibleOpCount.push_back(count);
            vector<int> tmp=findSeq(x,alreadyIn,alreadyInCount,npossibleOp,npossibleOpCount,n);
            for(int j=0;j<npossibleOpCount[npossibleOpCount.size()-1];j++){
                npossibleOp.pop_back();
            }
            npossibleOpCount.pop_back();
            for(int j=0;j<alreadyInCount[alreadyInCount.size()-1];j++){
                alreadyIn.pop_back();
            }
            alreadyInCount.pop_back();
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
        vector<int> alreadyIn;
        vector<int> alreadyInCount;
        vector<int>npossibleOp;
        vector<int>npossibleOpCount;
        int count=0;
        int incby=pow(2,i);
        for(int j=0;j<int(pow(2,n-i));j++){
            count++;
            alreadyIn.push_back(j*incby);
            //cout<<"alreadyIn"<<j*incby<<endl;
        }
        count++;
        alreadyIn.push_back(operate(0,incby-1));
        alreadyInCount.push_back(count);
        count=0;
        for(int j=0;j<int(pow(2,n-i));j++){
            count++;
            npossibleOp.push_back(j*incby+incby-1);
            //cout<<"npossibleOp"<<j*incby+incby-1<<endl;
        }
        npossibleOpCount.push_back(count);
        //cout<<"operations "<<incby-1<<" "<<operate(0,incby-1)<<endl;
        vector <int> tmp = findSeq(operate(0,incby-1),alreadyIn,alreadyInCount,npossibleOp,npossibleOpCount,n);
        if(tmp.size()>max.size())max=tmp;
    }
    max.push_back(0);
    for(int i=max.size()-1;i>=0;i--){
        cout<<representBin(max[i],n)<<endl;
    }
}

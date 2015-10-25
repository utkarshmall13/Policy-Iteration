#include <bits/stdc++.h>
using namespace std;

vector< vector <int> > PIposs(16);
vector<int> PI(16);

vector<bool> alreadyIn;

//function inserts all the possible switches that can happen an a possible improvement set.
void makePIposs(){
    PIposs[0].push_back(0);
    PIposs[1].push_back(1);
    PIposs[2].push_back(2);
    PIposs[4].push_back(4);
    PIposs[8].push_back(8);

    PIposs[3].push_back(1); PIposs[3].push_back(2); PIposs[3].push_back(3);
    PIposs[5].push_back(1); PIposs[5].push_back(4); PIposs[5].push_back(5);
    PIposs[6].push_back(2); PIposs[6].push_back(4); PIposs[6].push_back(6);
    PIposs[9].push_back(1); PIposs[9].push_back(8); PIposs[9].push_back(9);
    PIposs[10].push_back(2); PIposs[10].push_back(8); PIposs[10].push_back(10);
    PIposs[12].push_back(4); PIposs[12].push_back(8); PIposs[12].push_back(12);

    for(int i=1;i<=7;i++)PIposs[7].push_back(i);
    for(int i=2;i<=14;i+=2)PIposs[14].push_back(i);
    for(int i=1;i<=15;i++)PIposs[15].push_back(i);

    PIposs[11].push_back(1);PIposs[11].push_back(2);PIposs[11].push_back(3);PIposs[11].push_back(8);PIposs[11].push_back(9);PIposs[11].push_back(10);PIposs[11].push_back(11);
    PIposs[13].push_back(1);PIposs[13].push_back(4);PIposs[13].push_back(5);PIposs[13].push_back(8);PIposs[13].push_back(9);PIposs[13].push_back(12);PIposs[13].push_back(13);
}

//A function that returns the possibilities of swithces to be made to get a deproved policies, given the IS
vector<int> finddepPossibilities(int a,int n){
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

//given the current policy and policies already in deproved set, the set gives maximum number of policy iterations required from here.
int longestSeq(int n,int len,int currPolicy){
    //if it takes more than 8 steps it is anyways worse than howard's policy iteration, so prune here.
    if(len>8)return -1;
    //find possible switches
    vector<int> switchPossible;
    for(int i=0;i<alreadyIn.size();i++){
        if(!alreadyIn[i])switchPossible.push_back(i);
    }
    int ret=len;
    for(int i=0;i<switchPossible.size();i++){
        int IS=switchPossible[i]^currPolicy;

        bool contin=false;
        int flippedPossibleOP=(int(pow(2,n))-1)^IS;
        vector<int> subsetsOfImp=finddepPossibilities(flippedPossibleOP,n);
        for(int j=0;j<subsetsOfImp.size();j++){
            if(subsetsOfImp[j]!=0 && alreadyIn[operate(currPolicy,subsetsOfImp[j])]){contin=true;break;}
        }
        if(contin) continue;

        vector<int> deprovedSwitches=finddepPossibilities(IS,n);
        vector<int> changesMade;
        for(int j=0;j<deprovedSwitches.size();j++){
            if(!alreadyIn[operate(currPolicy,deprovedSwitches[j])]){
                alreadyIn[operate(currPolicy,deprovedSwitches[j])]=true;
                changesMade.push_back(operate(currPolicy,deprovedSwitches[j]));
            }
        }
        alreadyIn[operate(currPolicy,PI[IS])]=true;
        int tmp=longestSeq(n,len+1,operate(currPolicy,PI[IS]));
        //pruning if steps>8 in atleast 1 branch
        if(tmp==-1)return -1;
        if(tmp>ret)ret=tmp;
        alreadyIn[operate(currPolicy,PI[IS])]=false;
        for(int j=0;j<changesMade.size();j++){
            alreadyIn[changesMade[j]]=false;
        }
    }
    return ret;
}

int main(){
    int n=4;
    makePIposs();
    int max_pi=int(pow(2,n));
    vector<bool> tmp(max_pi);
    //for(int i=0;i<16;i++){PI[i]=i;}
    //setting all case of possible switching strategies
    PI[0]=0;
    PI[1]=1;
    PI[2]=2;
    PI[4]=4;
    PI[8]=8;
    int pcounter=0;
    for(int i3=0;i3<3;i3++){
        PI[3]=PIposs[3][i3];
        for(int i5=0;i5<3;i5++){
            PI[5]=PIposs[5][i5];
            for(int i6=0;i6<3;i6++){
                PI[6]=PIposs[6][i6];
                for(int i9=0;i9<3;i9++){
                    PI[9]=PIposs[9][i9];
                    for(int i10=0;i10<3;i10++){
                        PI[10]=PIposs[10][i10];
                        for(int i12=0;i12<3;i12++){
                            PI[12]=PIposs[12][i12];
                            for(int i7=0;i7<7;i7++){
                                PI[7]=PIposs[7][i7];
                                for(int i14=0;i14<7;i14++){
                                    PI[14]=PIposs[14][i14];
                                    for(int i11=0;i11<7;i11++){
                                        PI[11]=PIposs[11][i11];
                                        for(int i13=0;i13<7;i13++){
                                            PI[13]=PIposs[13][i13];
                                            for(int i15=0;i15<15;i15++){
                                                pcounter++;
                                                PI[15]=PIposs[15][i15];
                                                alreadyIn=tmp;
                                                alreadyIn[0]=true;
                                                int policylen=longestSeq(n,1,0);
                                                if(policylen!=-1){
                                                    //out : policy counter : policylength if<=8;
                                                    //out : strategy
                                                    cout<<pcounter<<" "<<policylen<<endl;
                                                    for(int i=0;i<16;i++){
                                                        cout<<PI[i]<<" ";
                                                    }
                                                    cout<<endl;
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

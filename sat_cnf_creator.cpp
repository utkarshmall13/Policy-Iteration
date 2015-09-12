#include <bits/stdc++.h>
using namespace std;
class Pair{
public:
    int a,b;
    Pair(int a,int b){
        this->a=a;
        this->b=b;
    }
};


void create_cnf_clause(int i,int j,int n){
    vector < vector <Pair> > clauses_in_dnf;
    for(int k=0;k<n;k++){
        vector<Pair> tmp;
        tmp.push_back(Pair(i*n+k+1,(i+1)*n+k+1));
        tmp.push_back(Pair(-(i*n+k+1),-((i+1)*n+k+1)));
        tmp.push_back(Pair(-((i+1)*n+k+1),(j)*n+k+1));
        tmp.push_back(Pair((i+1)*n+k+1,-((j)*n+k+1)));
        tmp.push_back(Pair(-((j)*n+k+1),(j+1)*n+k+1));
        tmp.push_back(Pair((j)*n+k+1,-((j+1)*n+k+1)));
        clauses_in_dnf.push_back(tmp);
    }

    vector <vector <int> > cnf;
    for(int i=0;i<6;i++){
        vector<int> tmp;
        tmp.push_back(clauses_in_dnf[0][i].a);
        tmp.push_back(clauses_in_dnf[0][i].b);
        cnf.push_back(tmp);
    }

    for(int l=1;l<n;l++){
        int siz = cnf.size();
        for(int j=0;j<5;j++){
            for(int i=0;i<siz;i++){
                cnf.push_back(cnf[i]);
            }
        }
        for(int j=0;j<6;j++){
            for(int i=0;i<siz;i++){
                cnf[j*siz+i].push_back(clauses_in_dnf[l][j].a);
                cnf[j*siz+i].push_back(clauses_in_dnf[l][j].b);
            }
        }
    }
    for(int i=0;i<cnf.size();i++){
        for(int j=0;j<cnf[i].size();j++){
            cout<<cnf[i][j]<<" ";
        }
        cout<<"0"<<endl;
    }
}

void create_inequality_check(int i,int j,int n){
    vector < vector <Pair> > clauses_in_dnf;
    for(int k=0;k<n;k++){
        vector<Pair> tmp;
        tmp.push_back(Pair(i*n+k+1,(j)*n+k+1));
        tmp.push_back(Pair(-(i*n+k+1),-((j)*n+k+1)));
        clauses_in_dnf.push_back(tmp);
    }

    vector <vector <int> > cnf;
    for(int i=0;i<2;i++){
        vector<int> tmp;
        tmp.push_back(clauses_in_dnf[0][i].a);
        tmp.push_back(clauses_in_dnf[0][i].b);
        cnf.push_back(tmp);
    }

    for(int l=1;l<n;l++){
        int siz = cnf.size();
        for(int j=0;j<1;j++){
            for(int i=0;i<siz;i++){
                cnf.push_back(cnf[i]);
            }
        }
        for(int j=0;j<2;j++){
            for(int i=0;i<siz;i++){
                cnf[j*siz+i].push_back(clauses_in_dnf[l][j].a);
                cnf[j*siz+i].push_back(clauses_in_dnf[l][j].b);
            }
        }
    }
    for(int i=0;i<cnf.size();i++){
        for(int j=0;j<cnf[i].size();j++){
            cout<<cnf[i][j]<<" ";
        }
        cout<<"0"<<endl;
    }
}


int main(){
    int n;
    cin>>n;
    int length;
    cin>>length;
    cout<<"p cnf "<<n*length<<" "<<(((length-1)*(length-2))/2)*(int(pow(6,n)))+((length*(length-1))/2)*(int(pow(2,n)))<<endl;
    for(int i=0;i<length-2;i++){
        for(int j=i+1;j<length-1;j++){
            create_cnf_clause(i,j,n);
        }
    }
    for(int i=0;i<length-1;i++){
        for(int j=i+1;j<length;j++){
            create_inequality_check(i,j,n);
        }
    }
}

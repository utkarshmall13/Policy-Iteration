#include <bits/stdc++.h>
using namespace std;

//compile and run the file
/*terminal will wait for inputs 1)number of states on MDP i.e. columns in OR and
    2) number of policy iterations i.e. rows in or matrices.

Remember to send the output genrated to a file say "sat.in"

Use command "minisat sat.in sat.out" to get the resulting satisfying OR matrix in "sat.out"
    or sat.out will give unsat as solution.

to check try on smaller examples s.t. number of rows <= 4
*/

int offsetForDepVar=0;

//class pair of two integers
class Pair{
public:
    int a,b;
    Pair(int a,int b){
        this->a=a;
        this->b=b;
    }
};


class Quadruple{
public:
    int a,b,c,d;
    Quadruple(int a,int b,int c,int d){
        this->a=a;
        this->b=b;
        this->c=c;
        this->d=d;
    }
};


//for all i<j exists k ((B[i,k] ^ ~B[j,k]) ^ (A[i,k]!=A[j,k]))
//so for all OR over k of ((B[i,k] ^ ~B[j,k]) ^ (A[i,k] v A[j,k]) ^ (~A[i,k] v ~A[j,k]))
void create_Dep_clauses(int i,int j,int n){
    vector < vector <vector <int> > > clauses_in_df;
    for(int k=0;k<n;k++){
        vector<vector <int> > tmp;

        vector<int> subtmp;
        subtmp.push_back(offsetForDepVar+i*n+k+1);
        tmp.push_back(subtmp);
        subtmp.clear();
        subtmp.push_back(-(offsetForDepVar+j*n+k+1));
        tmp.push_back(subtmp);
        subtmp.clear();
        subtmp.push_back(i*n+k+1);
        subtmp.push_back(j*n+k+1);
        tmp.push_back(subtmp);
        subtmp.clear();
        subtmp.push_back(-(i*n+k+1));
        subtmp.push_back(-(j*n+k+1));
        tmp.push_back(subtmp);
        subtmp.clear();
        clauses_in_df.push_back(tmp);
    }

    vector <vector <int> > cnf;
    for(int i=0;i<4;i++){
        vector<int> tmp;
        for(int j=0;j<clauses_in_df[0][i].size();j++){
            tmp.push_back(clauses_in_df[0][i][j]);
        }
        cnf.push_back(tmp);
    }

    for(int l=1;l<n;l++){
        int siz = cnf.size();
        for(int j=0;j<3;j++){
            for(int i=0;i<siz;i++){
                cnf.push_back(cnf[i]);
            }
        }
        for(int j=0;j<4;j++){
            for(int i=0;i<siz;i++){
                for(int k=0;k<clauses_in_df[l][j].size();k++){
                    cnf[j*siz+i].push_back(clauses_in_df[l][j][k]);
                }
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

//B[i,k] = (A[i,k] != A[i,k])
void create_Relate_clauses(int i,int n){
    for(int k=0;k<n;k++){
        cout<<(offsetForDepVar+i*n+k+1)<<" "<<-(i*n+k+1)<<" "<<((i+1)*n+k+1)<<" "<<0<<endl;
        cout<<(offsetForDepVar+i*n+k+1)<<" "<<(i*n+k+1)<<" "<<-((i+1)*n+k+1)<<" "<<0<<endl;
        cout<<-(offsetForDepVar+i*n+k+1)<<" "<<(i*n+k+1)<<" "<<((i+1)*n+k+1)<<" "<<0<<endl;
        cout<<-(offsetForDepVar+i*n+k+1)<<" "<<-(i*n+k+1)<<" "<<-((i+1)*n+k+1)<<" "<<0<<endl;
    }
}

//Policy j is not superset of i, theremust be a k s.t. policy[i,k]=1 and policy[j,k]=1
//OR over k for(B[i][k] ^ ~ B[j][k])
void create_notSuperSet_clauses(int i,int j,int n){
    vector < vector <int> > clauses_in_df;
    for(int k=0;k<n;k++){
        vector<int> tmp;
        tmp.push_back(offsetForDepVar+i*n+k+1);
        tmp.push_back(-(offsetForDepVar+j*n+k+1));
        clauses_in_df.push_back(tmp);
    }

    vector <vector <int> > cnf;
    for(int i=0;i<2;i++){
        vector<int> tmp;
        tmp.push_back(clauses_in_df[0][i]);
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
                cnf[j*siz+i].push_back(clauses_in_df[l][j]);
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

    //number of ORmatrix A type variables
    int numOR=n*length;

    offsetForDepVar=numOR;
    //number of improvable set variables B type
    int numB=n*(length);

    // number of clauses to get from A type variables(matrix values) to B type variables(improvement possible) B[i][j]=(A[i][j]!=A[i+1][j]);
    int numRelateC=(length-1)*n*4;
    //number of clauses to check if B satisfies the deprovement set conditions i.e. (B[i][k] && !B[j][k]) => A[i][k]!=A[j][k]
    int numDepC=((length)*(length-1)/2)*int(pow(4,n));
    // deprovable condition,given clause for each i,j,k are
    //B[i][k] AND ~B[j][k] AND (A[i][k] OR A[j][k]) AND (~A[i][k] OR ~A[j][k])
    // we have to take OR of this for all k. done by create_Dep_clauses()

    //to show that a later strategy is not a superset of any previous strategy
    //so forall i<j exists k s.t. (B[i][k]=1 and B[j][k]=0)
    int numSupC=((length)*(length-1)/2)*int(pow(2,n));

    cout<<"p cnf "<<numOR+numB<<" "<<numSupC+numDepC+numRelateC<<endl;

    //for creating OR matrix clauses
    for(int i=0;i<length-1;i++){
        for(int j=i+1;j<length;j++){
            create_Dep_clauses(i,j,n);
            create_notSuperSet_clauses(i,j,n);
        }
    }

    for(int i=0;i<length-1;i++){
        create_Relate_clauses(i,n);
    }
}

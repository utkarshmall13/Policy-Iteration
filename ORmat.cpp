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

//class pair of two integers
class Pair{
public:
    int a,b;
    Pair(int a,int b){
        this->a=a;
        this->b=b;
    }
};


void create_cnf_clause(int i,int j,int n){
    vector < vector <Pair> > clauses_in_df;
    //for any two rows i,j and any column k following tmp vector should hold
    //values inside pair are connected by disjunction
    //values of tmp are in conjunction

    //e.g. if tmp is [(1 3) (2 4)]
    // then tmp represents formula (1 v 3) ^ (2 v 4)

    //also atleast one of these must be true this means clauses_in_df is disjunction of tmps
    for(int k=0;k<n;k++){
        vector<Pair> tmp;
        tmp.push_back(Pair(i*n+k+1,(i+1)*n+k+1));
        tmp.push_back(Pair(-(i*n+k+1),-((i+1)*n+k+1)));
        tmp.push_back(Pair(-((i+1)*n+k+1),(j)*n+k+1));
        tmp.push_back(Pair((i+1)*n+k+1,-((j)*n+k+1)));
        tmp.push_back(Pair(-((j)*n+k+1),(j+1)*n+k+1));
        tmp.push_back(Pair((j)*n+k+1,-((j+1)*n+k+1)));
        clauses_in_df.push_back(tmp);
    }

    //below code expand clauses_in_df to a cnf form by recursively(here iteratively) taking
    //cartesian product of clauses in clauses_in_df
    vector <vector <int> > cnf;
    for(int i=0;i<6;i++){
        vector<int> tmp;
        tmp.push_back(clauses_in_df[0][i].a);
        tmp.push_back(clauses_in_df[0][i].b);
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
                cnf[j*siz+i].push_back(clauses_in_df[l][j].a);
                cnf[j*siz+i].push_back(clauses_in_df[l][j].b);
            }
        }
    }

    //outputting clauses
    for(int i=0;i<cnf.size();i++){
        for(int j=0;j<cnf[i].size();j++){
            cout<<cnf[i][j]<<" ";
        }
        cout<<"0"<<endl;
    }
}


//Explanation exactly similar to create_cnf_clause() funtion except now tmp takes 2 clauses instead of 6
void create_inequality_check(int i,int j,int n){
    vector < vector <Pair> > clauses_in_df;
    for(int k=0;k<n;k++){
        vector<Pair> tmp;
        tmp.push_back(Pair(i*n+k+1,(j)*n+k+1));
        tmp.push_back(Pair(-(i*n+k+1),-((j)*n+k+1)));
        clauses_in_df.push_back(tmp);
    }

    vector <vector <int> > cnf;
    for(int i=0;i<2;i++){
        vector<int> tmp;
        tmp.push_back(clauses_in_df[0][i].a);
        tmp.push_back(clauses_in_df[0][i].b);
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
                cnf[j*siz+i].push_back(clauses_in_df[l][j].a);
                cnf[j*siz+i].push_back(clauses_in_df[l][j].b);
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

    //n*length is size of matrix and hence the number of propositional variables in SAT problem
    //(((length-1)*(length-2))/2)*(int(pow(6,n))) is number of clauses required for OR matrix condition
    //forall i,j>i exists k s.t. A[i][k] ne A[i+1][k] = A[j][k] = A[j+1][k]
    //i and j together will take (((length-1)*(length-2))/2) values
    //for each i,j a formula with (pow(6,n)) clauses is formed in CNF

    //similarly for checking distinct rows ((length*(length-1))/2)*(int(pow(2,n))) clauses are required
    //explanation similar to before

    cout<<"p cnf "<<n*length<<" "<<(((length-1)*(length-2))/2)*(int(pow(6,n)))+int(pow(2,n))/*+((length*(length-1))/2)*(int(pow(2,n)))*/<<endl;

    //for creating OR matrix clauses
    for(int i=0;i<length-2;i++){
        for(int j=i+1;j<length-1;j++){
    //for(int j=2;j<length-1;j++){
    //    for(int i=j-1;i>=0;i--){
            create_cnf_clause(i,j,n);
        }
    }
    create_inequality_check(length-2,length-1,n);
    //for creating distinc row clauses
    /*for(int i=0;i<length-1;i++){
        for(int j=i+1;j<length;j++){
            create_inequality_check(i,j,n);
        }
    }*/
}

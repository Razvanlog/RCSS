#include "CDCL.h"
#include <map>
#include<cstring>
using namespace std;
void CDCL::ini(){
    int counter=0;
    cout<<"literal count:";
    cin>>lit_count;
    cout<<"clause count:";
    cin>>clause_count;
    assigned_lit_count=0;
    c_antecedent=-1;
    al_unsat=false;
    literals.clear();
    lit_freq.clear();
    lit_polarity.clear();
    clauses.clear();
    clauses.resize(clause_count);
    lit_antecedent.clear();
    lit_dec_level.clear();
    literals.resize(lit_count,-1);
    lit_freq.resize(lit_count,-1);
    lit_polarity.resize(lit_count,-1);
    lit_antecedent.resize(lit_count,-1);
    lit_dec_level.resize(lit_count,-1);
    map<string,int>vars;
    map<int,string>rev_vars;
    for (int i=-1; i<clause_count; i++){
        vector<int>inp;
        char s[500];
        if (i>=0){
            cout<<"clause"<<i<<":";
        }
        cin.getline(s,sizeof(s));
        char* next_token=NULL;
        char* token=NULL;
        token=strtok_s(s,"{,}",&next_token);
        while (token!=NULL){
            int sign=1;
            if (token[0]=='!'){
                token=token+1;
                sign=-1;
            }
            if (vars.find(token)==vars.end()){
                //lit_count++;
                counter++;
                vars[token]=counter;
                rev_vars[counter]=token;
            }
            clauses[i].push_back(vars[token]*sign);
            if (sign>0){
                lit_freq[vars[token]-1]++;
                lit_polarity[vars[token]-1]++;
            }
            else if (sign<0) {
                lit_freq[-1+vars[token]]++;
                lit_polarity[-1+vars[token]]--;
            }
            //inp.push_back(vars[token]*sign);
            token=strtok_s(NULL,"{,}",&next_token);
        }
    }
    og_lit_freq=lit_freq;
}
void CDCL::ini_pigeon(){
    int pigeons,holes;
    cout<<"pigeons:";
    cin>>pigeons;
    cout<<"holes:";
    cin>>holes;
    int counter=0;
    clause_count=pigeons+pigeons*(holes-1)*holes/2+pigeons*(pigeons-1)*holes/2;
    lit_count=pigeons*holes;
    //cout<<"literal count:";
    //cin>>lit_count;
    //cout<<"clause count:";
    //cin>>clause_count;
    assigned_lit_count=0;
    c_antecedent=-1;
    al_unsat=false;
    literals.clear();
    lit_freq.clear();
    lit_polarity.clear();
    clauses.clear();
    clauses.resize(clause_count);
    lit_antecedent.clear();
    lit_dec_level.clear();
    literals.resize(lit_count,-1);
    lit_freq.resize(lit_count,-1);
    lit_polarity.resize(lit_count,-1);
    lit_antecedent.resize(lit_count,-1);
    lit_dec_level.resize(lit_count,-1);
    map<string,int>vars;
    map<int,string>rev_vars;
    int var_counter=0;
    int cur_clause=0;
    for (int i = 1; i <= pigeons; i++)
    {
        //string input="";
        //vector<int>inp;
        for (int j = 1; j <= holes; j++)
        {
            string name;
            var_counter++;
            //inp.push_back(var_counter);
            name = "p";
            name.push_back('0' + i);
            name.push_back('0' + j);
            rev_vars[var_counter] = name;
            vars[name] = var_counter;
            clauses[cur_clause].push_back(var_counter);
            lit_freq[var_counter-1]++;
            lit_polarity[var_counter-1]++;
        }
        cur_clause++;
    }
    for (int i = 1; i <= pigeons; i++)
    {
        for (int j = 1; j <= holes; j++)
        {
            for (int k = j + 1; k <= holes; k++)
            {
                vector <int>inp;
                string name;
                name = "p";
                name.push_back(i + '0');
                name.push_back(j + '0');
                clauses[cur_clause].push_back(-vars[name]);
                //inp.push_back(-vars[name]);
                lit_freq[-1+vars[name]]++;
                lit_polarity[-1+vars[name]]--;
                name = "p";
                name.push_back(i + '0');
                name.push_back(k + '0');
                clauses[cur_clause].push_back(-vars[name]);
                cur_clause++;
                lit_freq[-1+vars[name]]++;
                lit_polarity[-1+vars[name]]--;
                //inp.push_back(-vars[name]);
                //f.read_clause(inp, vars);
            }
        }
    }

    for (int i = 1; i <= pigeons; i++)
    {
        for (int j = 1; j <= holes; j++)
        {
            for (int k = i + 1; k <= pigeons; k++)
            {
                //vector <int>inp;
                string name;
                name = "p";
                name.push_back(i + '0');
                name.push_back(j + '0');
                clauses[cur_clause].push_back(-vars[name]);
                lit_freq[-1+vars[name]]++;
                lit_polarity[-1+vars[name]]--;
                //inp.push_back(-names[name]);
                name = "p";
                name.push_back(k + '0');
                name.push_back(j + '0');
                clauses[cur_clause].push_back(-vars[name]);
                lit_freq[-1+vars[name]]++;
                lit_polarity[-1+vars[name]]--;
                cur_clause++;
                //inp.push_back(-names[name]);
                //f.read_clause(inp, vars);
            }
        }
    }

    /*for (int i=0; i<clause_count; i++)
        {
            for (int j=0; j<clauses[i].size(); j++)
                cout<<rev_vars[abs(clauses[i][j])]<<' ';
            cout<<'\n';
        }*/
    /*for (int i=-1; i<clause_count; i++){
        //vector<int>inp;
        char s[500];
        if (i>=0){
            cout<<"clause"<<i<<":";
        }
        //cin.getline(s,sizeof(s));
        char* next_token=NULL;
        char* token=NULL;
        token=strtok_s(s,"{,}",&next_token);
        while (token!=NULL){
            int sign=1;
            if (token[0]=='!'){
                token=token+1;
                sign=-1;
            }
            if (vars.find(token)==vars.end()){
                //lit_count++;
                counter++;
                vars[token]=counter;
                rev_vars[counter]=token;
            }
            clauses[i].push_back(vars[token]*sign);
            if (sign>0){
                lit_freq[vars[token]-1]++;
                lit_polarity[vars[token]-1]++;
            }
            else if (sign<0) {
                lit_freq[-1+vars[token]]++;
                lit_polarity[-1+vars[token]]--;
            }
            //inp.push_back(vars[token]*sign);
            token=strtok_s(NULL,"{,}",&next_token);
        }
    }*/
    og_lit_freq=lit_freq;
}
int CDCL::start(){
    int dec_level=0;
    if (al_unsat){
        return ret_val::r_unsat;
    }
    int u_p_r=unit_propagation(dec_level);
    if (u_p_r==ret_val::r_unsat){
        return u_p_r;
    }
    while (!all_assigned()){
        int pick_var=pick_branch();
        dec_level++;
        assign_lit(pick_var,dec_level,-1);
        while(true){
            u_p_r=unit_propagation(dec_level);
            if (u_p_r==ret_val::r_unsat){
                if(dec_level==0){
                    return u_p_r;
                }
                dec_level=conflict_check_backt(dec_level);
            }
            else break;
        }
    }
    return ret_val::r_sat;
}
int CDCL::unit_propagation(int dec_level){
    bool u_c_f=false;
    int false_count=0;
    int unset_count=0;
    int lit_index;
    bool sat_flag=false;
    int last_unset_lit=-1;
    do{
        u_c_f=false;
        for (int i=0; i<clauses.size() && !u_c_f; i++){
            false_count=0;
            unset_count=0;
            sat_flag=false;
            for (int j=0; j<clauses[i].size(); j++){
                lit_index=lit2var_index(clauses[i][j]);
                if(literals[lit_index]==-1){
                    unset_count++;
                    last_unset_lit=j;
                }
                else if((literals[lit_index]==0 && clauses[i][j]>0)|| (literals[lit_index]==1 && clauses[i][j]<0)){
                    false_count++;
                }
                else{
                    sat_flag=true;
                    break;
                }
            }
            if (sat_flag)
                continue;
            if (unset_count==1){
                assign_lit(clauses[i][last_unset_lit],dec_level,i);
                u_c_f=true;
                break;
            }
            else if (false_count==clauses[i].size()){
                c_antecedent=i;
                return ret_val::r_unsat;
            }
        }
    }while(u_c_f);
    c_antecedent=-1;
    return ret_val::r_unknown;
}
void CDCL::assign_lit(int var,int dec_level,int antecedent){
    int lit=lit2var_index(var);
    int value=(var>0)? 1:0;
    literals[lit]=value;
    lit_dec_level[lit]=dec_level;
    lit_antecedent[lit]=antecedent;
    lit_freq[lit]=-1;
    assigned_lit_count++;
}
void CDCL::unassign_lit(int lit_index){
    literals[lit_index]=-1;
    lit_dec_level[lit_index]=-1;
    lit_antecedent[lit_index]=-1;
    lit_freq[lit_index]=og_lit_freq[lit_index];
    assigned_lit_count--;
}
int CDCL::lit2var_index(int var){
    return (var>0)? var-1:-var-1;
}
int CDCL::conflict_check_backt(int dec_level){
    vector<int>learnt_clause=clauses[c_antecedent];
    int c_dec_level=dec_level;
    int cur_level=0;
    int r_lit;
    int lit;
    do{
        cur_level=0;
        for (int i=0; i<learnt_clause.size(); i++){
            lit=lit2var_index(learnt_clause[i]);
            if (lit_dec_level[lit]==c_dec_level){
                cur_level++;
            }
            if (lit_dec_level[lit]==c_dec_level && lit_antecedent[lit]!=-1){
                r_lit=lit;
            }
        }
        if (cur_level==1)
        break;
        learnt_clause=res(learnt_clause,r_lit);
    }while(true);
    clauses.push_back(learnt_clause);
    for (int i=0;i<learnt_clause.size(); i++){
        int lit_index=lit2var_index(learnt_clause[i]);
        int update=(learnt_clause[i]>0)?1:-1;
        lit_polarity[lit_index]+=update;
        if (lit_freq[lit_index]!=-1){
            lit_freq[lit_index]++;
        }
        og_lit_freq[lit_index]++;
    }
    clause_count++;
    int backt_dec_level=0;
    for (int i=0; i<learnt_clause.size(); i++){
        int lit_index=lit2var_index(learnt_clause[i]);
        int dec_level_here=lit_dec_level[lit_index];
        if (dec_level_here!=c_dec_level && dec_level_here>backt_dec_level)
        backt_dec_level=dec_level_here;
    }
    for (int i=0; i<literals.size(); i++){
        if (lit_dec_level[i]>backt_dec_level){
            unassign_lit(i);
        }
    }
    return backt_dec_level;
}
vector<int> &CDCL::res(vector<int>&input_clause,int lit){
    vector <int> second_inp=clauses[lit_antecedent[lit]];
    input_clause.insert(input_clause.end(),second_inp.begin(),second_inp.end());
    for (int i=0; i<input_clause.size(); i++){
        if (input_clause[i]==lit+1 || input_clause[i]==-lit-1){
            input_clause.erase(input_clause.begin()+i);
            i--;
        }
    }
    sort(input_clause.begin(),input_clause.end());
    input_clause.erase(unique(input_clause.begin(),input_clause.end()),input_clause.end());
    return input_clause;
}
int CDCL::pick_branch(){
    uniform_int_distribution<int>choose_branch(1,10);
    uniform_int_distribution<int>choose_literal(0,lit_count-1);
    int random_val=choose_branch(generator);
    bool too_many_tries=false;
    int nr_try=0;
    do{
        if (random_val>4 || assigned_lit_count<lit_count/2 || too_many_tries){
            hits++;
            if (hits==20*lit_count){
                for (int i=0; i<literals.size(); i++){
                    og_lit_freq[i]/=2;
                    if (lit_freq[i]!=-1){
                        lit_freq[i]/=2;
                    }
                }
                hits=0;
            }
            int var=distance(lit_freq.begin(),max_element(lit_freq.begin(),lit_freq.end()));
            if (lit_polarity[var]>=0)
            return var+1;
            return -var-1;
        }
        else {
            while (hits<10*lit_count){
                int var=choose_literal(generator);
                if (lit_freq[var]!=-1){
                    if (lit_polarity[var]>=0) return var+1;
                    return -var-1;
                }
                hits++;
            }
            too_many_tries=true;
        }
    }while(too_many_tries);
    return 10;
}
bool CDCL::all_assigned(){
    return lit_count==assigned_lit_count;
}
void CDCL::show(int status){
    if (status==ret_val::r_sat){
        cout<<"sat"<<'\n';
        /*for (int i=0; i<literals.size(); i++){
            if (i!=0){
                cout<<" ";
            }
            if (literals[i]!=-1){
                cout<<pow(-1,(literals[i]+1))*(i+1);
            }
            else cout<<(i+1);
        }*/
    }
    else cout<<"unsat"<<'\n';
}
void CDCL::solve(){
    int answer=start();
    show(answer);
}
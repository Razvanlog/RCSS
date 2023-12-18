#pragma once
#ifndef CDCL_H_INCLUDED
#define CDCL_H_INCLUDED
#include <algorithm>
#include <cmath>
#include <vector>
#include <random>
#include <iostream>
using namespace std;
enum ret_val{
    r_sat,
    r_unsat,
    r_unknown
};
class CDCL{
private:
    vector <int> literals;
    vector<vector<int>> clauses;
    vector<int> lit_freq;
    vector<int> lit_polarity;
    vector<int> og_lit_freq;
    int lit_count,clause_count,c_antecedent;
    vector<int>lit_dec_level;
    vector<int>lit_antecedent;
    int assigned_lit_count;
    bool al_unsat;
    int unit_propagation(int);
    void assign_lit(int,int,int);
    void unassign_lit(int);
    int lit2var_index(int);
    mt19937 generator;
    int hits=0;
    int conflict_check_backt(int);
    vector<int>& res(vector<int>&,int);
    int pick_branch();
    bool all_assigned();
    void show(int);
public:
    CDCL(){}
    void ini();
    void ini_pigeon();
    int start();
    void solve();
};
#endif
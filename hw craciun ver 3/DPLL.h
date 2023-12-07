#pragma once
#ifndef DPLL_H_INCLUDED
#define DPLL_H_INCLUDED
#include <fstream>
#include <vector>
#include <map>
#include <string>
using namespace std;
class Clause
{
public:
	static int each_step;
	static void set_print(int f) {
		each_step = f;
	}
	Clause(){}
	void read_clause(vector <int> in);
	void read_clause(vector<int> in, map< int, string> mp);
	void dump_clause()const;
	virtual void add_literal(int lit)= 0;
	virtual void add_var(int lit, string var) = 0;
	virtual int max_var()const = 0;
	virtual int len() const = 0;
	virtual int get_lit(int i)const=0;
	virtual string get_var(int i)const = 0;
};
class Clause_impl : public Clause 
{
public:
	vector <int> lits;
	vector< pair<int, string>> literal_names;
	Clause_impl() {}
	Clause_impl(const Clause_impl & c) :lits(c.lits),literal_names(c.literal_names) {}
	void add_literal(int lit);
	void add_var(int lit, string var);
	int max_var() const;
	int len() const;
	int get_lit(int i)const;
	string get_var(int i)const;
	vector<int> get_lits()const;
	void add_lits(int lit);
	void add_vars(int lit, string s);
	bool has_lit(int i)const;
};
class Formula {
public:
	int maxvar;
	Formula() :maxvar(0) {}
	void read_clause(vector<int> v);
	void read_clause(vector<int>v, map<int, string> mp);
	void dump_clause();
	virtual Clause& add_clause() = 0;
	virtual int len() const = 0;
	virtual Clause& get_clause(int i) = 0;
};
class Formula_impl:public Formula {
public:
	
	vector<Clause_impl> clauses;
	Formula_impl() {}
	Formula_impl(const Formula_impl&f):clauses(f.clauses){}
	Clause& add_clause();
	int len() const;
	Clause& get_clause(int i);
	bool has_lit(int i);
	vector<Clause_impl>& get_clauses() { return clauses; };
	void set_clauses(vector<Clause_impl>new_clauses) { clauses = new_clauses; };
};
class solver{
public:
	map<int, bool>assign;
	bool solve(Formula_impl& f);
	bool splitting(Formula_impl& c);
	Formula_impl& unit_prop(Formula_impl& f, int uni,string var);
	Formula_impl& pure_lit(Formula_impl& f, vector <pair<int,string>> p);
	pair<int,string> get_uni_prop(vector<Clause_impl>& c)const;
	vector<pair<int,string>> get_pure(Formula_impl& f);
};

#endif
#include "DPLL.h"
#include <vector>
#include <iostream>
#include<cmath>
using namespace std;
//Clause methods
void Clause::read_clause(vector <int> in)
{
	for (auto i : in)
	{
		add_literal(i);
		/*if (cat.literal_names.find(i) == cat.literal_names.end())
		{
			cat.literal_names
		}*/
	}
}
void Clause::read_clause(vector <int> in,map< int,string> mp)
{
	//literal_names = mp;
	//cout << "DA";
	for (auto i : in)
	{
		//cout << "DA";
		add_literal(i);
		add_var(abs(i), mp[abs(i)]);
	}
}
void Clause::dump_clause()const
{
	int i;
	//cout << literal_names.size() << '\n';
	cout << "(";
	for (i = 0; i < len() - 1; i++)
	{
		int ans = get_lit(i);
		//cout << ans << ", ";
		if (ans < 0)
			cout << "!";
		cout << get_var(abs(ans)) << ", ";
		//cout << literal_names[ans] << ", ";
	}
	if (len()!=0)
	{
		int ans = get_lit(i);
		//cout << ans;
		if (ans < 0)
			cout << "!";
		cout << get_var(abs(ans));
	}
	cout << ")\n";
}
//Clause_impl methods
void Clause_impl::add_literal(int lit)
{
	add_lits(lit);
}
void Clause_impl::add_var(int lit,string var)
{
	/*if (literal_names.find(lit) == literal_names.end())
	{
		cout << var << "DA";
		add_vars(lit, var);
		//literal_names[lit] = var;
	}*/
	for (auto i : literal_names)
	{
		if (i.first == lit)
		{
			return;
		}
	}
	literal_names.push_back(make_pair(lit, var));
}
int Clause_impl::max_var() const 
{
	int max = -1;
	for (int i = 0; i < lits.size(); i++)
	{
		if (max < abs(lits[i]))
			max = abs(lits[i]);
	}
	return max;
}
int Clause_impl::len()const
{
	return lits.size();
}
int Clause_impl::get_lit(int i)const
{
	return lits.at(i);
}
string Clause_impl::get_var(int i)const
{
	for (auto j : literal_names)
	{
		if (j.first == i)
			return j.second;
	}
	return "null";
}
vector <int> Clause_impl::get_lits()const
{
	return lits;
}
void Clause_impl::add_lits(int lit)
{
	lits.push_back(lit);
}
void Clause_impl::add_vars(int lit, string s)
{
	literal_names.push_back(make_pair(lit, s));
	//literal_names[lit] = s;
}
bool Clause_impl::has_lit(int a)const
{
	for (int i = 0; i < lits.size(); i++)
	{
		if (lits[i] == a)
			return true;
	}
	return false;
}
//Formula methods
void Formula::read_clause(vector <int> v)
{
	Clause& c = add_clause();
	c.read_clause(v);
	int tmp = c.max_var();
	if (tmp > maxvar)
		maxvar = tmp;
}
void Formula::read_clause(vector <int> v,map<int,string> mp)
{
	Clause& c = add_clause();
	//cout << "DA";
	c.read_clause(v,mp);
	int tmp = c.max_var();
	if (tmp > maxvar)
		maxvar = tmp;
}
void Formula::dump_clause()
{
	for (int i = 0; i < len(); i++)
	{
		get_clause(i).dump_clause();
	}
}
//Formula_impl methods
Clause& Formula_impl::add_clause()
{
	Clause_impl c;
	clauses.push_back(c);
	return clauses.back();
}
int Formula_impl::len() const {
	return clauses.size();
}
Clause& Formula_impl::get_clause(int i)
{
	return clauses.at(i);
}
//solver methods
bool solver::solve(Formula_impl& f)
{
	return splitting(f);
}
bool solver::splitting(Formula_impl& f)
{
	Formula_impl c(f);
	c.dump_clause();
	cout << "\n\n";
	pair<int, bool>temp;
	if (c.get_clauses().size() == 0)
		return true;
	for (int i = 0; i < c.get_clauses().size(); i++)
	{
		if (c.get_clause(i).len() == 0)
			return false;
	}
	pair<int, string>tempp= get_uni_prop(c.get_clauses());
	int uni = tempp.first;
	string name = tempp.second;
	if (uni != 0)
	{
		temp = make_pair(uni, true);
		assign.insert(temp);
		return splitting(unit_prop(c, uni,name));
	}
	else
	{
		//cout << "DA";
		vector<pair<int,string>>p = get_pure(c);
		if (p.size() != 0)
		{
			for (int i = 0; i < p.size(); i++)
			{
				temp = make_pair(p[i].first, true);
				assign.insert(temp);
			}
			//cout << "DA";
			return splitting(pure_lit(c, p));
		}
		else
		{
			//cout << "DA";
			int firstlit = c.get_clause(0).get_lit(0);
			string name = c.get_clause(0).get_var(abs(firstlit));
			temp = make_pair(firstlit, true);
			assign.insert(temp);
			//cout << "splitting: trying literal " << c.get_clause(0).get_var(firstlit) << '\n';
			cout << "splitting: trying literal " << name << '\n';
			if (splitting(unit_prop(c, firstlit,name)) == true)
			{
				//cout << ;
				return true;
			}
			else
			{
				//cout << "splitting: trying literal !" << c.get_clause(0).get_var(abs(firstlit)) << '\n';
				cout << "splitting: trying literal !" << name << '\n';
				//cout << "DA";
				assign.erase(firstlit);
				temp = make_pair(firstlit, false);
				assign.insert(temp);
				return splitting(unit_prop(f, -firstlit,name));
			}
		}
	}
}
Formula_impl& solver::unit_prop(Formula_impl& f, int uni,string var)
{
	vector <int>lits;
	while (uni != 0)
	{
		pair<int, bool>temp;
		temp = make_pair(uni, true);
		assign.insert(temp);
		vector <Clause_impl>::iterator get_var=f.clauses.begin();
		//cout << "removing literal " << get_var->get_var(abs(uni)) << '\n';
		cout << "removing literal ";
		if (uni < 0)
			cout << "!";
		cout<< var << '\n';
		vector<Clause_impl>::iterator it;
		for (it = f.clauses.begin(); it != f.clauses.end();)
		{
			f.dump_clause();
			cout << '\n';
			bool stop_flag = false;
			vector <int>::iterator jt;
			for (jt = it->lits.begin(); jt != it->lits.end();)
			{
				if ((*jt) == -uni)
				{
					//cout << "removing literal "<<*jt<<'\n';
					jt = it->lits.erase(jt);
					break;
				}
				else if ((*jt) == uni)
				{
					stop_flag = true;
					break;
				}
				else ++jt;
			}
			if (stop_flag == false)
				++it;
			else it = f.clauses.erase(it);
		}
		
		//f.dump_clause();
		//cout << '\n';
		uni = get_uni_prop(f.get_clauses()).first;
	}
	return f;
}
pair<int,string> solver::get_uni_prop(vector<Clause_impl>& c)const {
	for (int i = 0; i < c.size(); i++)
	{
		if (c[i].get_lits().size() == 1)
		{
			int uni = c[i].get_lit(0);
			string name = c[i].get_var(abs(uni));
			return make_pair(uni,name);
		}
	}
	return make_pair(0,"null");
}
Formula_impl& solver::pure_lit(Formula_impl& f, vector<pair<int,string>>p)
{
	vector<Clause_impl>::iterator it;
	for (int i = 0; i < p.size(); i++)
	{
		cout << "removing pure literal ";
		if (p[i].first < 0)
			cout << "!";
		cout<< p[i].second << '\n';
		for (it = f.clauses.begin(); it != f.clauses.end();)
		{
			if (it->has_lit(p[i].first))
			{
				pair<int, bool>temp;
				temp = make_pair(p[i].first, true);
				assign.insert(temp);
				it = f.clauses.erase(it);
			}
			else it++;
		}
		f.dump_clause();
		cout << '\n';
	}
	return f;
}
vector <pair<int,string>>solver::get_pure(Formula_impl& f)
{
	map<int, int>lit_table;
	map<int, string> name_reg;
	vector<pair<int,string>>p;
	vector<Clause_impl>::iterator it;
	for (it = f.clauses.begin(); it != f.clauses.end(); it++)
	{
		vector<int>::iterator jt;
		for (jt = it->lits.begin(); jt != it->lits.end(); jt++)
		{
			name_reg.insert(pair<int, string>(*jt,it->get_var(abs(*jt))));
			if (lit_table.find(abs(*jt)) == lit_table.end())
			{
				if (*jt < 0)lit_table.insert(pair<int, int>(abs(*jt), -1));
				else lit_table.insert(pair<int, int>(abs(*jt), 1));
			}
			else
			{
				if (lit_table.find(abs(*jt))->second == -1 && *jt > 0) {
					lit_table.erase(*jt);
					lit_table.insert(pair<int, int>(abs(*jt), 0));
				}
				else if (lit_table.find(abs(*jt))->second == 1 && *jt < 0)
				{
					lit_table.erase(abs(*jt));
					lit_table.insert(pair<int, int>(abs(*jt), 0));
				}
			}
		}
	}
	map<int, int>::iterator kt;
	int lit;
	for (kt = lit_table.begin(); kt != lit_table.end(); kt++)
	{
		if (kt->second != 0)
		{
			lit = (kt->first) * (kt->second);
			p.push_back(make_pair(lit,name_reg[lit]));
		}
	}
	return p;
}
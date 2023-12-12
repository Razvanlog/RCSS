#pragma once
#ifndef Quine_McClusky_H_INCLUDED
#define Quine_McClusky_H_INCLUDED
#include <string>
#include <vector>
#include<iostream>
#include <algorithm>
using namespace std;
class qm
{
public:
	int var;
	std::string dontcares;
	qm(int a)
	{
		var = a;
		dontcares.append(var, '-');
	}
	vector <string> get_vars(void);
	vector <string>get_vars(vector<string> var);
	string dec_to_bin(int n);
	string pad(string n);
	bool is_grey_code(string a, string b);
	string replace_comp(string a, string b);
	bool in_vector(vector<string> a, string b);
	vector <string> reduce(vector<string> minterms);
	string get_value(string a);
	string get_value_custom_negation(string a, vector<string>var);
	string get_value_custom(string a, vector<string> var);
	bool vec_equal(vector<string> a, vector<string> b);
};
#endif

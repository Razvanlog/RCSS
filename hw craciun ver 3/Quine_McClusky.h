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
	vector <string> get_vars(void)
	{
		vector <string> answer;
		string letters[] = { "a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p" };
		for (int i = 0; i < this->var; i++)
		{
			answer.push_back(letters[i]);
		}
		return answer;
	}
	vector <string>get_vars(vector<string> var)
	{
		vector <string> answer=var;
		return answer;
	}
	string dec_to_bin(int n)
	{
		if (n == 0)
			return "";
		if (n % 2 == 0)
			return dec_to_bin(n / 2) + "0";
		else return dec_to_bin(n / 2) + "1";
	}
	string pad(string n)
	{
		int max = var - n.length();
		for (int i = 0; i < max; i++)
		{
			n = "0" + n;
		}
		return n;
	}
	bool is_grey_code(string a, string b)
	{
		int flags = 0;
		for (int i = 0; i < a.length(); i++)
		{
			if (a[i] != b[i])
				flags++;
		}
		return (flags == 1);
	}
	string replace_comp(string a, string b)
	{
		string answer = "";
		for (int i = 0; i < a.length(); i++)
		{
			if (a[i] != b[i])
				answer = answer + "-";
			else answer = answer + a[i];
		}
		return answer;
	}
	bool in_vector(vector<string> a, string b)
	{
		for (int i = 0; i < a.size(); i++)
		{
			if (a[i].compare(b) == 0)
			{
				return true;
			}
		}
		return false;
	}
	vector <string> reduce(vector<string> minterms)
	{
		vector<string>new_minterms;
		int max = minterms.size();
		int* checked = new int[max];
		for (int i = 0; i < max; i++)
		{
			for (int j = i; j < max; j++)
			{
				if (is_grey_code(minterms[i], minterms[j]))
				{
					checked[i] = 1;
					checked[j] = 1;
					if (!in_vector(new_minterms, replace_comp(minterms[i], minterms[j])))
					{
						new_minterms.push_back(replace_comp(minterms[i], minterms[j]));
					}
				}
			}
		}
		for (int i = 0; i < max; i++)
		{
			if (checked[i] != 1 && !in_vector(new_minterms, minterms[i]))
				new_minterms.push_back(minterms[i]);
		}
		delete[] checked;
		return new_minterms;
	}
	string get_value(string a)
	{
		string answer = "";
		vector <string> vars = this->get_vars();
		if (a == dontcares)
		{
			return "1";
		}
		//cout << a << '\n';
		int it = 0;
		for (it = 0; it < a.length(); it++)
		{
			if (a[it] != '-')
			{
				if (a[it] == '0')
				{
					answer = answer+"!"+ vars[it];
				}
				else answer = answer + vars[it];
				answer = answer + '&';
			}
		}
		//cout << answer << '\n';
		answer.erase(answer.begin()+answer.size()-1);
		//cout << answer;
		return answer;
	}
	string get_value_custom_negation(string a, vector<string>var)
	{
		string answer = "";
		vector <string> vars = this->get_vars(var);
		if (a == dontcares)
		{
			return "0";
		}
		int it = 0;
		for (it = 0; it < a.length(); it++)
		{
			if (a[it] != '-')
			{
				if (a[it] == '0')
				{
					answer = answer + vars[it];
				}
				else answer = answer+"!" + vars[it];
				answer = answer + '|';
			}
		}
		answer.erase(answer.begin() + answer.size() - 1);
		return answer;
	}
	string get_value_custom(string a, vector<string> var)
	{
		string answer = "";
		vector <string> vars = this->get_vars(var);
		if (a == dontcares)
		{
			return "1";
		}
		int it = 0;
		for (it = 0; it < a.length(); it++)
		{
			if (a[it] != '-')
			{
				if (a[it] == '0')
				{
					answer = answer + "!" + vars[it];
				}
				else answer = answer + vars[it];
				answer = answer + '&';
			}
		}
		answer.erase(answer.begin() + answer.size() - 1);
		return answer;
	}
	bool vec_equal(vector<string> a, vector<string> b)
	{
		if (a.size() != b.size())
			return false;
		sort(a.begin(), a.end());
		sort(b.begin(), b.end());
		for (int i = 0; i < a.size(); i++)
		{
			if (a[i] != b[i])
				return false;
		}
		return true;
	}
};
#endif

#pragma once
#ifndef Trie_DP_H_INCLUDED
#define Trie_DP_H_INCLUDED
#include <vector>
#include <iostream>
#include <algorithm>
#include <unordered_set>
#include <memory>
using namespace std;
//nodes class
class trie_node {
public:
	int var;
	shared_ptr<trie_node> pos,  neg,  rest;
	static shared_ptr<trie_node> nullmarker()
	{
		static shared_ptr<trie_node> marker = make_shared<trie_node>(0);
		marker->pos = nullptr;
		marker->neg = nullptr;
		marker->rest = nullptr;
		return marker;
	}
	trie_node(int);
	/*~trie_node()
	{
		delete pos;
		delete neg;
		delete rest;
	}*/
};
//clause class
class clause {
public:
	vector<int> literals;
	clause(const vector<int>&inp):literals(inp){}
	clause()
	{
	}
};
//CNF class
class CNF
{
public:
	vector<clause> clauses;
	CNF(const vector<clause>&inp):clauses(inp){}
	CNF(){}
};
//DP class
class DP
{
public:
	shared_ptr<trie_node> trie;
	DP(CNF& cnf):trie(nullptr)
	{
		for (clause& cl: cnf.clauses)
		{
			trie = trie_insert(trie, cl.literals);
			//trie_display(trie);
			//cout << '\n';
		}
	}
	shared_ptr<trie_node> trie_merge(shared_ptr<trie_node>, shared_ptr<trie_node> );
	shared_ptr<trie_node> trie_insert(shared_ptr<trie_node> ,  vector<int>&);
	void trie_display(shared_ptr<trie_node>);
	shared_ptr<trie_node> unit_propagation(shared_ptr<trie_node>, int);
	bool exec(shared_ptr<trie_node>,CNF& , vector<int>& );
	bool is_satisfied(CNF& , shared_ptr<trie_node> );
};
//DP class
#endif
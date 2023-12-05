#include "Trie_DP.h"
#include <iostream>
#include <vector>
#include <functional>
#include <memory>
#include <algorithm>
#include <unordered_set>
using namespace std;

//node methods
trie_node::trie_node(int v)
{
	var = v;
	this->pos = nullptr;
	this->neg = nullptr;
	this->rest = nullptr;
}
shared_ptr<trie_node> DP::trie_merge(shared_ptr<trie_node>a,shared_ptr<trie_node>b)
{
	//if (a == trie_node::nullmarker() || b == trie_node::nullmarker())
	//	return trie_node::nullmarker();
	if (a ==  NULL || b == NULL)
		return NULL;
	if (a == nullptr || a == trie_node::nullmarker())
		return b;
	if (b == nullptr || b==trie_node::nullmarker())
		return a;
	/*if (a == trie_node::nullmarker())
		return NULL;
	if (b == trie_node::nullmarker())
		return NULL;*/
	if (a->var < b->var)
	{
		shared_ptr<trie_node> merged = make_shared<trie_node>(a->var);
		merged->rest = trie_merge(a->rest, b);
		merged->neg = a->neg;
		merged->pos = a->pos;
		//default_delete(a);
		return merged;
	}
	else if (a->var > b->var)
	{
		shared_ptr<trie_node> merged = make_shared<trie_node>(b->var);
		merged->rest = trie_merge(a, b->rest);
		merged->neg = b->neg;
		merged->pos = b->pos;
		//delete b;
		//b->rest = trie_merge(a, b->rest);
		return merged;
	}
	else
	{
		shared_ptr<trie_node> n = trie_merge(a->neg,b->neg);
		shared_ptr<trie_node> r = trie_merge(a->rest, b->rest);
		shared_ptr<trie_node> inp=make_shared<trie_node>(a->var);
		inp->var = a->var;
		inp->pos = trie_merge(a->pos, b->pos);
		inp->neg = n;
		inp->rest = r;
		//delete a;
		//delete b;
		return inp;
		/*trie_node* merged = new trie_node(a->var);
		if (a->neg != nullptr && b->neg != nullptr)
			merged->neg = trie_merge(a->neg, b->neg);
		else merged->neg = nullptr;
		if (a->pos != nullptr && b->pos != nullptr)
			merged->pos = trie_merge(a->pos, b->pos);
		else merged->pos = nullptr;
		merged->rest = trie_merge(a->rest, b->rest);
		//delete b;
		//delete a;
		return merged;*/
	}
}
shared_ptr<trie_node> DP::trie_insert(shared_ptr<trie_node> root, vector<int>&cl)
{
	if (cl.empty())
	{
		return trie_node::nullmarker();
	}
	int var = cl.back();
	vector <int> clwithoutvar(cl.begin(), cl.end() - 1);
	shared_ptr<trie_node> new_node = make_shared <trie_node>(var);
	shared_ptr<trie_node> tclause = trie_insert(nullptr, clwithoutvar);
	trie_display(new_node);
	cout<<"new node" << '\n';
	trie_display(tclause);
	cout <<"tclause" << '\n';
	new_node->rest = trie_merge(tclause, root);
	trie_display(new_node);
	cout << "\n\n";
	return new_node;
}
void DP::trie_display(shared_ptr<trie_node> root)
{
	//cout << "DA";
	//cout << root << '\n';
	if (root == nullptr) {
		//cout << "nullptr, ";
		return;
	}
	if (root == trie_node::nullmarker())
	{
		//cout << "DA";
		cout << "nil, ";
		return;
	}
	//if (root->neg != nullptr)
	//{
		trie_display(root->neg);
		cout << "$" << root->var << ", ";
	//}
	//if (root->pos != nullptr)
	//{
		trie_display(root->pos);
		cout << "nil, nil, ";
	//}
	//if (root->rest != nullptr)
	//{
		trie_display(root->rest);
		cout << "%, ";
	//}
}
shared_ptr<trie_node> DP::unit_propagation(shared_ptr<trie_node> root, int literal)
{
	if (root == nullptr) return nullptr;
	if (root == trie_node::nullmarker())
		return trie_node::nullmarker();
	shared_ptr<trie_node> unitcl = make_shared<trie_node>(literal);
	if (root->var == literal)
	{
		shared_ptr<trie_node> rest = trie_merge(unitcl, root->rest);
		//delete root;
		return rest;
	}
	else if (root->var == -literal)
	{
		shared_ptr<trie_node> rest = trie_merge(unitcl, root->rest);
		if (rest != nullptr) 
		{
			rest->neg = trie_merge(unitcl, root->neg);
			rest->pos = trie_merge(unitcl, root->pos);
		}
	//	delete root;
		return rest;
	}
	else
	{
		root->neg = unit_propagation(root->neg, literal);
		root->pos = unit_propagation(root->pos, literal);
		root->rest = unit_propagation(root->rest, literal);
		shared_ptr<trie_node> new_node = make_shared <trie_node>(root->var);
		new_node->rest = trie_merge(unitcl, root->rest);
		new_node->neg = root->neg;
		new_node->pos = root->pos;
		return new_node;
	}
	//trie_node* unitcl = new trie_node(literal);
	//return trie_merge(unitcl, root);
}
bool DP::exec(shared_ptr<trie_node> root, CNF& cnf, vector<int>& assignment)
{
	//cout << "DA";
	if (root == nullptr)
		return true;
	if (root->var != 0)
	{
		assignment.push_back(root->var);
		return exec(unit_propagation(root, -root->var), cnf, assignment);
	}
	int var = 0;
	for (int i = 1; i <= cnf.clauses.size(); i++)
	{
		if (find(assignment.begin(), assignment.end(), i) == assignment.end() && find(assignment.begin(), assignment.end(), -i) == assignment.end())
		{
			var = i;
			break;
		}
	}
	if (var == 0)
	{
		return is_satisfied(cnf, root);
	}
	assignment.push_back(var);
	shared_ptr<trie_node> true_branch = unit_propagation(root, var);
	if (exec(true_branch, cnf, assignment))
	{
		return true;
	}
	assignment.pop_back();
	assignment.push_back(-var);
	shared_ptr<trie_node> false_branch = unit_propagation(root, -var);
	if (exec(false_branch, cnf, assignment))
	{
		return true;
	}
	assignment.pop_back();
	return false;
}
bool DP::is_satisfied(CNF& cnf, shared_ptr<trie_node> root)
{
	//cout << "DA";
	unordered_set<int>assigned_literals;
	function<void(shared_ptr<trie_node>)> traverse_trie = [&](shared_ptr<trie_node> node)
		{
			if (node == nullptr) return;
			if (node->var != 0) assigned_literals.insert(node->var);
			traverse_trie(node->neg);
			traverse_trie(node->pos);
			traverse_trie(node->rest);
		};
	traverse_trie(root);
	for (auto& clause : cnf.clauses)
	{
		bool clause_satified = any_of(clause.literals.begin(), clause.literals.end(), [&](int literal) {return assigned_literals.count(literal) > 0; });
		if (!clause_satified) return false;
	}
	return true;
}
//tree methods
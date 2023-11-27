#ifndef PROPTREE_H_INCLUDED
#define PROPTREE_H_INCLUDED
#include<ostream>
#include <iostream>
#include <vector>
#include <map>
#include<memory>
#include <string>
class prop_tree;
class tree_node
{
    friend class prop_tree;
    
    friend std::ostream& operator<<(std::ostream&, const prop_tree&);
public:
 //   virtual int counter() const = 0;
 //   virtual int iam() const = 0;
protected:
    virtual tree_node* position_mem() { return this; };
    virtual int eval() const = 0;
    virtual void print(std::ostream&) const = 0;
};
class prop_tree {
public:
    //friend class tree_node;
    prop_tree() :_root(nullptr) {}
    
    prop_tree(std::string);//build variable
    prop_tree(int);
    prop_tree(char, prop_tree, prop_tree);//build logic operator'
    int eval() const { return _root->eval(); }
    tree_node* position_mem() { return _root.get(); }
    //std::vector<shared_ptr<tree_node>> inordered_arr() { return };
    //int iam() const { return _root->iam(); }
    //int counter() const { return _root->counter(); }
    void whatami()
    {
        std::cout << "i am an empty node\n";
    }
    static int format,print_each_step;
    int type=0;
 //   std::vector <char> v;
 //   std::map<char, int> mp;
    static void set_print(int f) { print_each_step = f; }
    static void set_format(int f) { format = f; }
    std::shared_ptr<tree_node> _root;
private:
    

    friend std::ostream& operator<<(std::ostream&, const prop_tree&);
};
inline std::ostream& operator<<(std::ostream& out, const prop_tree& T)
{
    T._root->print(out);
    return out;
}
class Operand : public tree_node {
public:
    Operand(std::string c) :name(c) {}
    Operand(int n):value(n) {
        if (n == 0)
            name = "0";
        else name = "1";
    }
    int type = 1;
    
    void whatami()
    {
        std::cout << "i am an operand\n";
    }
private:
    std::string name;
    int value = 0;
    int eval() const{
        return value; 
    }

   // vector<prop_tree> inordered_arr();
    /*int iam()const {
        return name;
    }*/
    int counter() const
    {
        return 1;
    }
    void print(std::ostream& out) const { out << name; }
};
class Operator :public tree_node {
public:
    Operator(char binop, prop_tree left, prop_tree right)
        : binop(binop), left(left), right(right) {}
    int type = 2;
    void whatami()
    {
        std::cout << "i am an operator\n";
    }
    char binop;
    prop_tree left, right;
    int eval() const;
    void modif_value();
    int counter() const;
    void print(std::ostream&) const;
};
#endif // PROPTREE_H_INCLUDED
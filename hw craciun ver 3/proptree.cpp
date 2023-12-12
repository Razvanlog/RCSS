#include <iostream>
#include <string>
#include <memory>
#include "proptree.h"
using namespace std;

prop_tree::prop_tree(string c)
    :_root(make_shared<Operand>(c)) {}
prop_tree::prop_tree(int nr)
    :_root(make_shared<Operand>(nr)) {}
prop_tree::prop_tree(char binop, prop_tree left, prop_tree right)
    :_root(make_shared<Operator>(binop, left, right)) {}
prop_tree::prop_tree(string f,vector<prop_tree> p)
    :_root(make_shared<Function>(f,p)){}
prop_tree::prop_tree(char quan,prop_tree var,prop_tree cond,int type)
    :_root(make_shared<Quantifier>(quan,var,cond,type)){}
int prop_tree::format = 0;
int prop_tree::print_each_step = 0;
void Operator::print(std::ostream& out)const {
    switch (prop_tree::format)
    {
    case 1:
        if (binop == '!')
            out << binop <<'(' << left<<')';
        else
        out << '(' << left << binop << right << ')'; break;
    case 2:
    {
        out << binop<<',' << '[' << left << ']';
        if (binop != '!')
            out <<',' << '[' << right << ']';
        break;
    }
        //out << left << ' ' << right << ' ' << binop; break;
    }
}
//vector<prop_tree>
int Operator::eval()const {
    int val_1 = left.eval();
    int val_2 = right.eval();
    switch (binop) {
    case '&': val_1 = (val_1 & val_2); break;
    case '|': val_1 = (val_1 | val_2); break;
    case '!': if (val_1)val_1 = 0; else val_1 = 1; break;
    case '$': {if (val_1 == 0) val_1 = 1;
            else val_1 = val_2;
        break;
    }
    case '#':val_1 = (val_1 == val_2); break;
    }
        return val_1;
}
void Function::print(std::ostream& out)const{
    int i=0;    
    switch (prop_tree::format)
    {
    case 1:
        out<<function_name;
        if (parameters.empty()){
            out<<"[]";
        }
        else{
        out<<"[";
        for (i=0; i<parameters.size()-1; i++)
            out<<parameters[i]<<',';
        out<<parameters[i]<<']';
        }
        break;
    case 2:
    {
        out << function_name;
        if (parameters.empty()){
            out<<"[]";
        }
        else {
            out<<',' << '[';
        for (i=0;i<parameters.size()-1; i++)
            out<<'{'<<parameters[i]<<'}'<<',';
        out<<'{'<<parameters[i]<<'}'<<']';
        }
        break;
    }
        //out << left << ' ' << right << ' ' << binop; break;
    }
}
int Function::eval() const{//THIS FUNCTION IS WIP
    return 0;
}
void Quantifier::print(std::ostream& out) const{
    switch (prop_tree::format)
    {
    case 1:
        out<<'('<<quan_type<<variable<<'('<<condition<<')'<<')';
        break;
    case 2:
    {
        out<<'['<<quan_type<<']'<<','<<'['<<variable<<']'<<','<<'['<<condition<<']';
        break;
    }
        //out << left << ' ' << right << ' ' << binop; break;
    }
}
int Quantifier::eval() const{//THIS IS WIP
    return 0;
}
/*int Operator::counter() const {
    int val_1 = left.counter();
    cout << left.iam()<<'\n';
    int val_2 = right.counter();
    return val_1 + val_2;
}*/

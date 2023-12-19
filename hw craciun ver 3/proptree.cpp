#include <iostream>
#include <string>
#include <memory>
#include "proptree.h"
#include <sstream>
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
    
    case 2:
    {
        out << binop<<',' << '[' << left << ']';
        if (binop != '!')
            out <<',' << '[' << right << ']';
        break;
    }
    default:
        if (binop == '!')
            out << binop <<'(' << left<<')';
        else
        out << '(' << left << binop << right << ')'; break;
        //out << left << ' ' << right << ' ' << binop; break;
    }
}
/*void Operator::sub(map<string,prop_tree> m,int type) {
    switch (prop_tree::format)
    {
    default:
        if (binop == '!')
        {cout<<binop<<'(';
        left.sub(m,type);
        cout<<')';
        }
            //cout << binop <<'(' << left<<')';
        else{
            cout<<'(';
            left.sub(m,type);
            cout<<binop;
            right.sub(m,type);
            cout<<')';
        //cout << '(' << left << binop << right << ')';
        } break;
    //case 2:
    //{
    //    out << binop<<',' << '[' << left << ']';
    //    if (binop != '!')
     //       out <<',' << '[' << right << ']';
     //   break;
   // }
    //    //out << left << ' ' << right << ' ' << binop; break;
    }
}
void Operand::sub(map<string,prop_tree> m,int type){
    if (m.find(name)==m.end())
    {
        cout<<name;
    }
    else cout<<m[name];
}
int Operand::check(string var){
    if (var==name)
    return 1;
    return 0;
}*/
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
/*void Function::sub(map<string,prop_tree>m,int type){
    int i=0;    
    switch (prop_tree::format)
    {
    default:
        cout<<function_name;
        if (parameters.empty()){
            cout<<"[]";
        }
        else{
        cout<<"[";
        for (i=0; i<parameters.size()-1; i++)
            {
                parameters[i].sub(m,type);
                cout<<',';
                //cout<<parameters[i]<<',';
            }
        parameters[i].sub(m,type);
        cout<<']';
        //out<<parameters[i]<<']';
        }
        break;
        //out << left << ' ' << right << ' ' << binop; break;
    }
}*/
void Function::print(std::ostream& out)const{
    int i=0;    
    switch (prop_tree::format)
    {
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
    default:
        {out<<function_name;
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
        }
        //out << left << ' ' << right << ' ' << binop; break;
    }
}
int Function::eval() const{//THIS FUNCTION IS WIP
    return 0;
}
/*void Quantifier::sub(map<string,prop_tree> m,int type){
    switch (prop_tree::format)
    {
    case 1:
        cout<<'('<<quan_type;
        //stringstream tester;
        //tester<<variable;
        variable.sub(m,type);
        cout<<'(';
        condition.sub(m,type);
        cout<<')'<<')';
        if (m.find(variable)!=m.end())
        m.erase(m[variable]);
        cout<<variable;
        cout<<'(';
        cout<<condition.sub(m);
        cout<<')'<<')';
        break;
        //out << left << ' ' << right << ' ' << binop; break;
    }
}*/
void Quantifier::print(std::ostream& out) const{
    switch (prop_tree::format)
    {
    
    case 2:
    {
        out<<'['<<quan_type<<']'<<','<<'['<<variable<<']'<<','<<'['<<condition<<']';
        break;
    }
    default:{
        out<<'('<<quan_type<<variable<<'('<<condition<<')'<<')';
        break;
    }
        //out << left << ' ' << right << ' ' << binop; break;
    }
}
int Quantifier::eval() const{//THIS IS WIP
    return 0;
}
/*void replacement::execute(tree_node *cur){

}*/
/*int Operator::counter() const {
    int val_1 = left.counter();
    cout << left.iam()<<'\n';
    int val_2 = right.counter();
    return val_1 + val_2;
}*/

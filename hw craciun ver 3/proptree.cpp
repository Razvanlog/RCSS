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
int prop_tree::format = 0;
void Operator::print(std::ostream& out)const {
    switch (prop_tree::format)
    {
    case 1:
        if (binop == '!')
            out << binop <<'(' << left<<')';
        else
        out << '(' << left << ' ' << binop << ' ' << right << ')'; break;
    case 2:
    {
        out <<'[' << binop<<']' << '[' << left << ']';
        if (binop != '!')
            out << '[' << right << ']';
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
/*int Operator::counter() const {
    int val_1 = left.counter();
    cout << left.iam()<<'\n';
    int val_2 = right.counter();
    return val_1 + val_2;
}*/

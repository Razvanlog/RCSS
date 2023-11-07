#include "infix.h"
#include <string>
#include <iostream>
prop_tree Climb :: parse_climb(int level) {
    prop_tree tree = parse_primary();
    while (priority(look_ahead()) >= level) {
        char op;
        infix >> op;
        int next_level = right_ass(op) ? priority(op) : priority(op) + 1;
        tree = prop_tree(op, tree, parse_climb(next_level));
    }
    return tree;
}
prop_tree Climb::parse_primary() {
    char c; infix >> c;
    switch (c) {
    case '(':
    {
        prop_tree tree = parse_climb(1);
        char p; infix >> p;
        if (p != ')')
            throw "Error: please close your parantheses :)";
        return tree;
    }
    default:
    {
        //std::cout << c << '\n';
        if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '!' || (c >= '0' && c <= '1')))
        {
            //std::cout << << '\n';
            //std::string s = "Error: logic error "+c;
           // s += " is placed incorrectly";
            throw "Error: logic error";
        }
        infix.putback(c);
        char variabila; infix >> variabila;
        if (variabila == '0' || variabila == '1')
        {
            infix.putback(c);
            int nr;
            infix >> nr;
            return prop_tree(nr);
        }
        else if (variabila == '!')
        {
            infix.putback(variabila);
            char c;
            infix >> c;
            prop_tree tree = parse_primary();
            //int next_level = right_ass(c) ? priority(c) : priority(c) + 1;
            return prop_tree(c, tree, NULL);
        }
        else
        {
            return prop_tree(variabila);
        }
    }
    }
}

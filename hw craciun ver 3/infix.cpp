#include "infix.h"
#include <string>
#include <iostream>
#include <fstream>
using namespace std;
ofstream output("output_terminal_log.txt");
prop_tree Climb :: parse_climb(int level) {
    prop_tree tree = parse_primary();
    //cout << tree << '\n';
    while (priority(look_ahead()) >= level) {
        //cout<<"YES";
        char op;
        infix >> op;
        //std::cout << op;
        int next_level = right_ass(op) ? priority(op) : priority(op) + 1;
        tree = prop_tree(op, tree, parse_climb(next_level));
        //cout << tree << '\n';
        if (prop_tree::print_each_step)
        {
            stringstream inp;
            if (prop_tree::format == 1)
                inp << tree << '\n', output << tree << '\n';
            else inp << '[' << tree << ']' << '\n', output << '[' << tree << ']' << '\n'<<'\n';
            string sinp;
            inp >> sinp;
            if (subs.wff_marker.find(sinp)==subs.wff_marker.end())
            subs.wffs.push_back(sinp),subs.wff_marker.insert(sinp);
        }
    }
    if (prop_tree::print_each_step){
        stringstream inp;
            if (prop_tree::format == 1)
                inp << tree << '\n', output << tree << '\n';
            else inp << '[' << tree << ']' << '\n', output << '[' << tree << ']' << '\n'<<'\n';
            string sinp;
            inp >> sinp;
            if (subs.wff_marker.find(sinp)==subs.wff_marker.end())
            subs.wffs.push_back(sinp),subs.wff_marker.insert(sinp);
    }
    //cout<<"DA";
    //cout << tree << '\n';
    return tree;
}
prop_tree Climb::parse_primary() {
    char c; infix >> c;
    switch (c) {
    case '(':
    {
        prop_tree tree = parse_climb(1);
        //cout << tree << '\n';
        char p; infix >> p;
        //cout << tree << ' ' << p << '\n';
        if (p != ')')
             throw "Error: please close your parantheses :)";
        return tree;
    }
    default:
    {
        //std::cout << c << '\n';
        if (c=='#' || c=='$' || c=='&' || c=='|')//(!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '!' || (c >= '0' && c <= '1')))
        {
            //std::cout << << '\n';
            //std::string s = "Error: logic error "+c;
           // s += " is placed incorrectly";
            throw "Error: logic error";
        }
        infix.putback(c);
        char variabila; infix >> variabila;
        //std::cout << "succes";
        if (variabila == '0' || variabila == '1')
        {
            infix.putback(variabila);
            int nr;
            infix >> nr;
            return prop_tree(nr);
        }
        else if (variabila == '!')
        {
            //cout << "exclamation check";
            //cout << c << '\n';
            prop_tree tree = parse_primary();
            prop_tree empty_node;
            //cout << c;
            //int next_level = right_ass(c) ? priority(c) : priority(c) + 1;
            return prop_tree(variabila, tree, empty_node);
        }
        else if (variabila=='@'||variabila=='~'){
            prop_tree var=parse_primary();
            prop_tree cond=parse_primary();
            //return prop_tree(variabila,var,cond,4);
        }
        else
        {
            //c = 'a';
            std::string name = "";
            do
            {
                if (!(variabila=='\\' || variabila == '#' || variabila == '$' || variabila == '&' || variabila == '|' || variabila == ')' || variabila=='!' || variabila=='[' || variabila==']' || variabila==','))
                    name.push_back(variabila);
                //char c;
                infix >> variabila;
                //std::cout << infix.str()<<' '<<variabila << '\n';
                
            } while (variabila!='\\' && !(variabila == '#' || variabila == '$' || variabila == '&' || variabila == '|' || variabila == ')' || variabila=='!' || variabila=='[' || variabila==']' || variabila==','));
            //std::cout << name<< '\n';
            if (name=="")
            throw "Error: NULL when expected name";
            if (variabila=='!')
                throw "Error: logic error";
            else if (variabila!='['){
            infix.putback(variabila);
            /*if (subs.wff_marker.find(name) == subs.wff_marker.end())
                subs.wff_marker.insert(name), subs.wffs.push_back(name);*/
            return prop_tree(name);
            }
            else {
                vector<prop_tree> par;
                char a;
                infix>>a;
               if (a==']')
               {
                return prop_tree(name,par);
               }
               infix.putback(a);
                do{
                    prop_tree inp=parse_primary();
                    par.push_back(inp);
                    infix>>variabila;
                }while(variabila!=']');
                return prop_tree(name,par);
                /*std::string vars_formatted="";
                vector<prop_tree> par;
                do{
                    
                    infix>>variabila;
                    if (variabila=='['){
                        par.push_back(prop_tree(vars_formatted));
                    }
                    if (variabila==','){
                        if (vars_formatted=="")
                        throw "Error: empty variable";
                        else {
                            par.push_back(prop_tree(vars_formatted));
                            vars_formatted="";
                        }
                    }
                    else if (variabila!=']'){
                        vars_formatted.push_back(variabila);
                    }
                }while (variabila!=']' && !infix.eof());
                if (vars_formatted=="")
                throw "Error:empty variable";
                else par.push_back(prop_tree(vars_formatted));
                if (variabila!=']' && infix.eof())
                throw "Error: please close your parantheses :)";
                return prop_tree(name,par);*/
            }
        }

    }
    }
}

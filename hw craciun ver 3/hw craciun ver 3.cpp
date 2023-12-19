//#pragma warning(suppress : 4996 455)
#include "infix.h"
#include "Quine_McClusky.h"
#include "DPLL.h"
#include "CDCL.h"
//#include "Trie_DP.h"
#include <iostream>
#include <string>
#include <map>
#include <algorithm>
#include <vector>
#include <chrono>
#include <cstring>
#include <fstream>
using namespace std;
ofstream input_log("input_terminal_log.txt",std::ios::app);
ofstream output_log("output_terminal_log.txt",std::ios::app);
string input;
vector <string> var;
vector <string> minterms;
vector <shared_ptr<tree_node>>inorder;
map <string, int> mp;
int l,mode,l_c_1,l_c_0,n,line_i;
int create_input(string wff);
void create_table_vals(int counter, vector <string> sub_wffs);
void obtain_minterms(int counter, int& r, qm QM, string wff);
string get_dnf(string wff, int each_step);
string get_cnf(string wff, int each_step);
void empty_all_containers();
void mode_1();
void mode_2();
void mode_3();
void mode_4();
void mode_5();
void mode_6();
void mode_7();
void mode_8();
void mode_9();
void mode_help();
int recog_com(string);
int main(int argc, char** argv)
{
    mode = 1;
    time_t time = chrono::system_clock::to_time_t(chrono::system_clock::now());
    char current_time[26];
    ctime_s(current_time, sizeof(current_time), &time);
    input_log << "program started at " << current_time;
    output_log << "program started at " << current_time;
    //line_i++;
    while (mode)
    {
        //cout << "mode:";
        //cin >> mode;
        string command="\n";
        cout<<"RLC:";
        cin>>command;
        mode=recog_com(command);
        //input_log<<"line "<<line_i << ": " << mode << '\n';
        //if (mode > 4 && mode<0)
        //    cout<<"Error: invalid mode",mode=0;
        //output_log << "yes" << '\n';
        if (mode>=0)
        {
            //auto tt = chrono::system_clock::now();
            //g<<"program started at " << asctime_s(tt) << '\n';
            //empty_all_containers();
            if (mode == 2)
            {
                try{
                mode_2();
                }
                catch(...){
                    cout<<"caught an error, verify input\n";
                }
               // output_log << "Yes";
            }
            else if (mode==1)
            {
                try{
                mode_1();
                }
                catch(...){
                    cout<<"caught an error, verify input\n";
                }
            }
            else if (mode == 3)
            {
                try{
                mode_3();
                }
                catch(...){
                    cout<<"caught an error, verify input\n";
                }
            }
            else if (mode == 4)
            {
                try{
                mode_4();
                }
                catch(...){
                    cout<<"caught an error, verify input\n";
                }
            }
            else if (mode == 5)
            {
                try{
                mode_5();
                }
                catch(...){
                    cout<<"caught an error, verify input\n";
                }
            }
            else if (mode == 6)
            {
                try{
                Clause::set_print(0);
                mode_6();
                }
                catch(...){
                    cout<<"caught an error, verify input\n";
                }
            }
            else if (mode == 7)
            {
                try{
                Clause::set_print(0);
                mode_7();
                }
                catch(...){
                    cout<<"caught an error, verify input\n";
                }
            }
            else if (mode==8){
                try{
                mode_8();
                }
                catch(...){
                    cout<<"caught an error, verify input\n";
                }
            }
            else if (mode==9){
                try{
                mode_9();
                }
                catch(...){
                    cout<<"caught an error, verify input\n";
                }
            }
            else if (mode==10){
                try{
                mode_help();
                }
                catch(...){
                    cout<<"caught an error, verify input\n";
                }
            }
        }
        else if (mode<0) {
            cout<<"command unrecognized\n";
        }
        line_i++;
        cin.clear();
    }
    return 0;
}
//functions

void mode_help(){
    cout<<"is_wff / wff : to check if a formula is valid\n";
    cout<<"create_tt / tt : to create a truth table\n";
    cout<<"truth_function_wff / tfwff : to create a wff from a truth function\n";
    cout<<"simplify / s : to simplify a wff\n";
    cout<<"nf : to obtain cnf and dnf of a wff\n";
    cout<<"dpll : to verify satisfiability of a given clause set using dpll\n";
    cout<<"pigeon : to solve the pigeon problem with dpll\n";
    cout<<"cdcl : to verify satisfiability of a given clause set using cdcl\n";
    cout<<"op_pigeon: to solve the pigeon problem with cdcl\n";
    cout<<"enter / exit : exit and close program\n";
}
int recog_com(string s){
    if (s=="is_wff" || s=="wff")
        return 1;
    if (s=="create_tt" || s=="tt")
        return 2;
    if (s=="truth_function_wff" || s=="tfwff")
        return 3;
    if (s=="simplify" || s=="s")
        return 4;
    if (s=="nf")
        return 5;
    if (s=="dpll")
        return 6;
    if (s=="pigeon")
        return 7;
    if (s=="op_pigeon")
        return 9;
    if (s=="cdcl")
        return 8;
    if (s=="\n" || s=="exit")
        return 0;
    if (s=="help")
        return 10;
    return -1;
}
int create_input(string wff)
{
    string s = "";
    int i = 0;
    wff += "\\";
    // cout << wff << ' ';
    while (i < wff.size())
    {
        string name = "";
        if (wff[i] != '\\' && wff[i] != '0' && wff[i] != '1' && wff[i] != '!' && wff[i] != '|' && wff[i] != '&' && wff[i] != '(' && wff[i] != ')' && wff[i] != '#' && wff[i] != '$')
        {
            while (i < wff.size() && wff[i] != '\\' && wff[i] != '!' && wff[i] != '|' && wff[i] != '&' && wff[i] != '(' && wff[i] != ')' && wff[i] != '#' && wff[i] != '$')//((input[i] >= 'a' && input[i] <= 'z') || (input[i] >= 'A' && input[i] <= 'Z'))
            {
                name.push_back(wff[i]);
                i++;
            }
            s.push_back(mp[name] + '0');
        }
        else {
            s.push_back(wff[i]);
            i++;
        }
    }
    Climb infix;
    //cout << s<<' '<<input << '\n';
    //s = s + '\\';
    //cout << s << '\n';
    prop_tree::set_print(0);
    prop_tree tree = infix.get_tree(s);
    prop_tree::set_print(1);
    //cout << tree << '\n';
    return tree.eval();
}
void create_table_vals(int counter, vector <string> sub_wffs)
{
    if (counter < l)
    {
        mp[var[counter]] = 0;
        create_table_vals(counter + 1, sub_wffs);
        mp[var[counter]] = 1;
        create_table_vals(counter + 1, sub_wffs);
    }
    else
    {
        int last_val = 0;
        for (int i = 0; i < sub_wffs.size(); i++)
        {
            last_val = create_input(sub_wffs[i]);
            cout << last_val << ' ';
            output_log << last_val << ' ';
            //for (int j = 0; j < var[i].size(); j++)
            //    cout << ' ';
        }
        //for (int i=0; i)
        //int result = create_input(input);
        //cout << result;
        if (last_val)
            l_c_1++;
        else l_c_0++;
        cout << '\n';
        output_log << '\n';
    }
}

void obtain_minterms(int counter, int& r, qm QM, string wff)
{
    if (counter < l)
    {
        mp[var[counter]] = 0;
        obtain_minterms(counter + 1, r, QM, wff);
        mp[var[counter]] = 1;
        obtain_minterms(counter + 1, r, QM, wff);
    }
    else
    {
        int result = create_input(wff);
        /*int result = 0;
        for (int i = 0; i < var.size(); i++)
        {
            result = result + mp[var[i]] * (1 << (var.size()-i-1));
        }*/
        if (result)
            minterms.push_back(QM.pad(QM.dec_to_bin(r)));
        r++;
    }
}
string get_dnf(string wff,int each_step) {
    wff += "\\";
    Climb infix;
    try {
        //cout << wff << '\n';
        prop_tree tree = infix.get_tree(wff);
        //prop_tree::set_format(1);
        //cout <<"DA" << tree <<"DA" << '\n';
        int i = 0;
        while (i < wff.size())
        {
            if (wff[i] != '\\' && wff[i] != '!' && wff[i] != '|' && wff[i] != '&' && wff[i] != '(' && wff[i] != ')' && wff[i] != '#' && wff[i] != '$')//((input[i] >= 'a' && input[i] <= 'z') || (input[i] >= 'A' && input[i] <= 'Z'))
            {
                string name = "";
                while (i < wff.size() && wff[i] != '\\' && wff[i] != '!' && wff[i] != '|' && wff[i] != '&' && wff[i] != '(' && wff[i] != ')' && wff[i] != '#' && wff[i] != '$')//((input[i] >= 'a' && input[i] <= 'z') || (input[i] >= 'A' && input[i] <= 'Z'))
                {
                    name.push_back(wff[i]);
                    i++;
                }
                if (mp.find(name) == mp.end())
                {
                    var.push_back(name);
                    //cout << name << '\n';
                    mp[name] = 0;
                }
            }
            i++;
        }
        l = var.size();
        qm q(var.size());
        int it = 0;
        obtain_minterms(0, it, q, wff);
        sort(minterms.begin(), minterms.end());
        /*for (int t = 0; t < minterms.size(); t++)
            cout << minterms[t] << ' ';
        cout << '\n';
        cout << '\n';*/
        //cout << "DA";
        if (minterms.empty())
        {
            if (each_step)
            {
                cout << "step:1 (0)'\n'";
                output_log << "step:1 (0)\n";
            }
            return "(0)";
        }
        else
        {
            int steps = 1;
            do
            {

                if (each_step)
                {
                    it = 0;
                    string before_wff = "";
                    for (it = 0; it < minterms.size() - 1; it++)
                        before_wff = before_wff + "(" + q.get_value_custom(minterms[it], var) + ")" + '|';
                    before_wff = before_wff + "(" + q.get_value_custom(minterms[it], var) + ")";
                    //cout << q.get_value_custom(minterms[t], var) << ' ';
                    cout << "step" << steps << ": " + before_wff << '\n';
                    output_log << "step" << steps << ": " + before_wff + ", ";
                    steps++;
                }
                minterms = q.reduce(minterms);
                sort(minterms.begin(), minterms.end());
            } while (!q.vec_equal(minterms, q.reduce(minterms)));
        }
        it = 0;
        string before_wff = "";
        //cout << "dnf:";
        for (it = 0; it < minterms.size() - 1; it++)
        {
            before_wff = before_wff + "(" + q.get_value_custom(minterms[it], var) + ")" + '|';
        }
        before_wff = before_wff + "(" + q.get_value_custom(minterms[it], var) + ")";
        //before_wff = before_wff + '\\';
        //cout << "dnf: " << before_wff << '\n';
        return before_wff;
    }
    catch (const char* error)
    {
        cout << error << '\n';
        output_log << error << '\n';
        return "";
    }
}
string get_cnf(string wff,int each_step) {
    wff.insert(wff.begin(), '(');
    wff.insert(wff.begin(), '!');
    wff.push_back(')');
    //wff = "!(" + wff + ")";
    wff += "\\";
    Climb infix;
    try {
        //cout << "CNF" << '\n';
        //prop_tree::set_format(1);
        prop_tree tree = infix.get_tree(wff);
        //prop_tree::set_format(1);
        //cout<<"DA" << tree <<"DA" << '\n';
        int i = 0;
        while (i < wff.size())
        {
            if (wff[i] != '\\' && wff[i] != '!' && wff[i] != '|' && wff[i] != '&' && wff[i] != '(' && wff[i] != ')' && wff[i] != '#' && wff[i] != '$')//((input[i] >= 'a' && input[i] <= 'z') || (input[i] >= 'A' && input[i] <= 'Z'))
            {
                string name = "";
                while (i < wff.size() && wff[i] != '\\' && wff[i] != '!' && wff[i] != '|' && wff[i] != '&' && wff[i] != '(' && wff[i] != ')' && wff[i] != '#' && wff[i] != '$')//((input[i] >= 'a' && input[i] <= 'z') || (input[i] >= 'A' && input[i] <= 'Z'))
                {
                    name.push_back(wff[i]);
                    i++;
                }
                if (mp.find(name) == mp.end())
                {
                    var.push_back(name);
                    //cout << name << '\n';
                    mp[name] = 0;
                }
            }
            i++;
        }
        //cout << "DA";
        l = var.size();
        qm q(var.size());
        int it = 0;
        obtain_minterms(0, it, q, wff);
        sort(minterms.begin(), minterms.end());
        /*for (int t = 0; t < minterms.size(); t++)
            cout << minterms[t] << ' ';
        cout << '\n';
        cout << '\n';*/
        //cout << "DA";
        if (minterms.empty())
        {
            return "(1)";
            //minterms.push_back("0");
        }
        else
        {
            do
            {
                minterms = q.reduce(minterms);
                /*for (int t = 0; t < minterms.size(); t++)
                    cout << q.get_value_custom(minterms[t],var) << ' ';
                cout << '\n'<<'\n';*/
                sort(minterms.begin(), minterms.end());
            } while (!q.vec_equal(minterms, q.reduce(minterms)));
        }
        //cout << minterms[0];
        it = 0;
        string before_wff = "";
        //cout << "dnf:";
        for (it = 0; it < minterms.size() - 1; it++)
        {
            before_wff = before_wff + "(" + q.get_value_custom_negation(minterms[it], var) + ")" + '&';
        }
        before_wff = before_wff + "(" + q.get_value_custom_negation(minterms[it], var) + ")";
        //before_wff = before_wff + '\\';
        //cout << "dnf: " << before_wff << '\n';
        return before_wff;
    }
    catch (const char* error)
    {
        cout << error << '\n';
        output_log << error << '\n';
        return "";
    }
}
void empty_all_containers() {
    minterms.clear();
    mp.clear();
    var.clear();
    inorder.clear();
    prop_tree::set_format(0);
    prop_tree::set_print(0);
}
void mode_1() {
    cout << "input wff:";
    cin >> input;
    input_log << "command:" << line_i << "," << " mode:1," << " wff:" << input << ",";
    //line_i++;
    input += "\\";
    Climb infix;
    try {
        cout << "format?\n";
        int syntax = -1;
        int each_step=-1;
        cin >> syntax;
        cout<<"each step?\n";
        cin>>each_step;
        input_log << "format:" << syntax<<"each_step:"<<each_step << '\n';
        prop_tree::set_format(syntax);
        prop_tree::set_print(each_step);
        if (syntax > 2 && syntax<0)
        {
            throw("Error: invalid syntax");
        }
        else if (each_step>0){
            throw("Error: invalid input");
        }
        else
        {
            prop_tree::set_print(1);
            prop_tree tree = infix.get_tree(input);
            prop_tree::set_print(0);
            //cout << '\n';
            /*if (syntax == 2)
                cout << "[" << tree << "]" << '\n';
            else cout << tree << '\n';*/
            output_log << "command:" << line_i << ", show wff" << '\n';
            //cout<<tree<<'\n';
            for (auto i : infix.subs.wffs)
            {
                cout << i << '\n';
                //if (i!=infix.subs.wffs.rbegin())
                output_log << i << ", ";
            }
            output_log << '\n';
            //tree = infix.get_tree(input);
            //cout << tree.mode << '\n';
            /*if (syntax == 2)
                cout << '[' << tree << ']' << '\n';
            else
                cout << tree << '\n';*/
        }
        //prop_tree::set_mode(0);
    }
    catch (const char* error)
    {
        output_log << error << '\n';
        cout << error << '\n';
    }
    empty_all_containers();
    //line_i++;
}
void mode_2() {
    cout << "input wff:";
    cin >> input;
    input_log << "command:" << line_i << ", mode:2, wff:" << input << '\n';
    input += "\\";
    prop_tree::set_print(1);
    prop_tree::set_format(1);
    Climb infix;
    try {
        prop_tree tree = infix.get_tree(input);
        int i = 0;
        while (i < input.size())
        {
            if (input[i] != '\\' && input[i] != '!' && input[i] != '|' && input[i] != '&' && input[i] != '(' && input[i] != ')' && input[i] != '#' && input[i] != '$')//((input[i] >= 'a' && input[i] <= 'z') || (input[i] >= 'A' && input[i] <= 'Z'))
            {
                string name = "";
                while (i < input.size() && input[i] != '\\' && input[i] != '!' && input[i] != '|' && input[i] != '&' && input[i] != '(' && input[i] != ')' && input[i] != '#' && input[i] != '$')//((input[i] >= 'a' && input[i] <= 'z') || (input[i] >= 'A' && input[i] <= 'Z'))
                {
                    name.push_back(input[i]);
                    i++;
                }
                if (mp.find(name) == mp.end())
                {
                    var.push_back(name);
                    //cout << name << ' ';
                    mp[name] = 0;
                }
            }
            i++;
        }
        l = var.size();
        //int syntax = 1;
        //prop_tree::set_format(syntax);
        output_log << "command:" << line_i << ", truth table:\n";
        for (auto i : infix.subs.wffs)
        {
            cout << i << ' ';
            output_log << i << " ";
        }
        cout << '\n';
        output_log << '\n';
        //cout << tree << '\n';
        vector<string> vars(infix.subs.wffs);
        create_table_vals(0, vars);
        if (l_c_1 == (1 << l))
            cout << "wff is valid\n", output_log << "wff is valid\n";
        else if (l_c_1)
            cout << "wff is satisfiable\n", output_log << "wff is satisfiable\n";
        else cout << "wff is unsatisfiable\n", output_log << "wff is unsatisfiable\n";
    }
    catch (const char* error)
    {
        cout << error << '\n';
        output_log << "command:" << line_i << ", " << error << '\n';
    }
    prop_tree::set_format(0);
    prop_tree::set_print(0);
    empty_all_containers();
}
void mode_3(){
    cout << "number of variables:";
    cin >> n;
    cout << "number of truths in truth function:";
    int rows = 0;
    cin >> rows;
    qm q(n);
    int a1 = 0;
    string input = "";
    cout << "truth function:\n";
    try
    {
        for (int t = 0; t < rows; t++)
        {
            int inp;
            int i = 0;
            //cout << i<<":";
            for (int j = 0; j < n; j++)
            {
                cin >> inp;
                if (inp < 2 && inp >= 0)
                    i = i + inp * (1 << (n - j - 1));
                else
                {
                    throw("Error: invalid truth value");
                    output_log << "command:" << line_i << ", Error: invalid truth value\n";
                }
            }
            //cout<<t<<' ' << i << '\n';
            minterms.push_back(q.pad(q.dec_to_bin(i)));
        }
        sort(minterms.begin(), minterms.end());
        do
        {
            minterms = q.reduce(minterms);
            sort(minterms.begin(), minterms.end());
        } while (!q.vec_equal(minterms, q.reduce(minterms)));
        if (minterms.size())
        {
            //cout << "succes";
            int it = 0;
            string before_wff = "";
            for (it = 0; it < minterms.size() - 1; it++)
            {
                //cout << "DA";
                before_wff = before_wff + q.get_value(minterms[it]) + '|';
            }
            //cout << "DA";
            //cout << it << ' ' << minterms.size() << '\n';
            //cout << minterms[it] << '\n';
            before_wff = before_wff + q.get_value(minterms[it]);
            //out << "DA";
            before_wff = before_wff + '\\';
            Climb infix;
            try {
                prop_tree tree = infix.get_tree(before_wff);
                cout << "format?\n";
                int syntax = 1;
                cin >> syntax;
                if (syntax > 2)
                {
                    throw("Error: invalid syntax");
                    output_log << "command:" << line_i << ", Error: invalid syntax\n";
                }
                prop_tree::set_format(syntax);
                if (syntax == 2)
                    cout << '[' << tree << ']' << '\n';
                else
                    cout << tree << '\n';
            }
            catch (const char* error)
            {
                cout << error << '\n';
            }
        }
    }
    catch (const char* error)
    {
        cout << error << '\n';
        output_log << "command:" << line_i << ", " << error << '\n';
    }
    empty_all_containers();
}
void mode_4() {
    cout << "input wff:";
    cin >> input;
    cout << "show steps:";
    int show_steps;
    cin >> show_steps;
    input_log << "command:" << line_i << ", mode:4, wff:" << input;
    output_log << "command:" << line_i << " ";
    string answer = get_dnf(input, show_steps);
    if (!show_steps)
    {
        cout << "simplified:" << answer << '\n';
        output_log << ",simplified:" << answer << '\n';
    }
    else {
        cout << "final step: " << answer << '\n';
        output_log << "final step: " << answer << '\n';
    }
    empty_all_containers();
}
void mode_5() {
    cout << "input wff:";
    cin >> input;
    cout << "show steps:";
    int show_steps;
    cin >> show_steps;
    input_log << "command:" << line_i << ", mode:5, wff:" << input;
    //string second_input = "!(" + input+ ")";
    //cout << second_input << '\n';
    //second_input.push_back(')');
    //second_input.insert(second_input.begin(), '(');
    //empty_all_containers();
    string ans_dnf = get_dnf(input, show_steps);
    cout << "dnf: " << ans_dnf << '\n';
    output_log << "command:" << line_i << ", dnf:" << ans_dnf;
    //cout << "succes" << '\n';
   // name.clear();
    empty_all_containers();
    string ans_cnf = get_cnf(input, show_steps);
    cout << "cnf: " << ans_cnf << '\n';
    output_log << ", cnf:" << ans_cnf << "\n";
    empty_all_containers();
}
void mode_6() {
    cout << "input clause number:";
    int nr;
    cin >> nr;
    cout << "print each step:";
    int each_step = 0;
    cin >> each_step;
    Clause::set_print(each_step);
    int var_counter = 0;
    map<string, int> vars;
    map<int, string> rev_vars;
    //CNF cnf;
    Formula_impl f;
    for (int i = -1; i < nr; i++)
    {
        //clause inp;
        vector <int> inp;
        char s[500];
        if (i >= 0)
        {
            cout << "clause " << i << ":";
        }
        cin.getline(s, sizeof(s));
        //cout << "DA";
        char* next_token = NULL;
        char* token = NULL;
        token = strtok_s(s, "{,}", &next_token);
        while (token != NULL)
        {
            int sign = 1;
            if (token[0] == '!')
            {
                token = token + 1;
                sign = -1;
            }
            if (vars.find(token) == vars.end())
            {
                //cout << token << '\n';
                var_counter++;
                vars[token] = var_counter;
                rev_vars[var_counter] = token;
                //cout << "DA";
            }
            inp.push_back(vars[token] * sign);

            //cout << token<<'\n';
            token = strtok_s(NULL, "{,}", &next_token);
        }
        //Clause_impl c;
        //c.read_clause(inp);
        //c.dump_clause();
        if (i >= 0)
            f.read_clause(inp, rev_vars);
        //f.get_clause(i);
        //f.add_clause
        //cout << '\n';
        //cnf.clauses.push_back(inp);

        //cout << '\n';
    }
    //f.dump_clause();
    solver sat;

    // cout << "DA";
     //f.get_var_names(rev_vars);
    if (sat.solve(f))
    {
        cout << "satisfiable\n";
    }
    else cout << "unsatisfiable\n";
    //f.dump_clause();
    //DP cur_dp(cnf);
    //cout << "DA";
    //vector <int> assignment;
    //if (cur_dp.trie == trie_node::nullmarker())
    //    cout << "DA";
    //cur_dp.trie_display(cur_dp.trie);
    /*if (cur_dp.exec(cur_dp.trie, cnf, assignment))
    {
        cout << "satisfiable" << '\n';
        cout << "cnf: ";
        /*for (int literal : assignment)
        {
            if (literal < 0)
            {
                cout << "!" <<literal << ',';
            }
            else cout << literal << ",";
        }
        cur_dp.trie_display(cur_dp.trie);
        cout << '\n';
    }
    else cout << "unsatisfiable\n";*/

    //delete &cur_dp;
}
void mode_7() {
    map<int, string>vars;
    map<string, int>names;
    Formula_impl f;
    int var_counter = 0;
    int n,m;
    cin>>n>>m;
    for (int i = 1; i <= n; i++)
    {
        vector<int>inp;
        for (int j = 1; j <= m; j++)
        {
            string name;
            var_counter++;
            inp.push_back(var_counter);
            name = "p";
            name.push_back('0' + i);
            name.push_back('0' + j);
            vars[var_counter] = name;
            names[name] = var_counter;
        }
        f.read_clause(inp, vars);
    }
    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= m; j++)
        {
            for (int k = j + 1; k <= m; k++)
            {
                vector <int>inp;
                string name;
                name = "p";
                name.push_back(i + '0');
                name.push_back(j + '0');
                inp.push_back(-names[name]);
                name = "p";
                name.push_back(i + '0');
                name.push_back(k + '0');
                inp.push_back(-names[name]);
                f.read_clause(inp, vars);
            }
        }
    }
    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= m; j++)
        {
            for (int k = i + 1; k <= n; k++)
            {
                vector <int>inp;
                string name;
                name = "p";
                name.push_back(i + '0');
                name.push_back(j + '0');
                inp.push_back(-names[name]);
                name = "p";
                name.push_back(k + '0');
                name.push_back(j + '0');
                inp.push_back(-names[name]);
                f.read_clause(inp, vars);
            }
        }
    }
    //cout<<f.clauses.size();
    solver  sat;
    time_t start,end;
    time(&start);
    if (sat.solve(f))
    {
        cout << "sat";
    }
    else cout << "unsat";
    cout << '\n';
    time(&end);
    cout<<end-start<<'\n';
}
void mode_8(){
    CDCL a;
    a.ini();
    a.solve();
}
void mode_9(){
    CDCL a;
    time_t start,end;
    a.ini_pigeon();
    time(&start);
    a.solve();
    time(&end);
    cout<<end-start<<'\n';
}
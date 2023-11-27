#include "infix.h"
#include "Quine_McClusky.h"
#include <iostream>
#include <string>
#include <map>
#include <algorithm>
#include <vector>
#include <chrono>
#include <fstream>
using namespace std;
ofstream g("input_terminal_log.txt");
string input;
vector <string> var;
vector <string> minterms;
vector <shared_ptr<tree_node>>inorder;
map <string, int> mp;
int l,mode,l_c_1,l_c_0,n,line_i;
int create_input(string wff)
{
    string s = "";
    int i = 0;
   // cout << wff << ' ';
    while (i < wff.size())
    {
        string name = "";
        if (wff[i]!='\\' && wff[i] != '0' && wff[i] != '1' && wff[i] != '!' && wff[i] != '|' && wff[i] != '&' && wff[i] != '(' && wff[i] != ')' && wff[i] != '#' && wff[i] != '$')
        {       while (i<wff.size()&&wff[i]!='\\' && wff[i] != '!' && wff[i] != '|' && wff[i] != '&' && wff[i] != '(' && wff[i] != ')' && wff[i] != '#' && wff[i] != '$')//((input[i] >= 'a' && input[i] <= 'z') || (input[i] >= 'A' && input[i] <= 'Z'))
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
    prop_tree tree = infix.get_tree(s);
    //cout << tree << '\n';
    return tree.eval();
}
void create_table_vals(int counter)
{
    if (counter < l)
    {
        mp[var[counter]] = 0;
        create_table_vals(counter + 1);
        mp[var[counter]] = 1;
        create_table_vals(counter + 1);
    }
    else
    {
        for (int i = 0; i < var.size(); i++)
        {
            cout << mp[var[i]];
            for (int j = 0; j < var[i].size(); j++)
                cout << ' ';
        }
        int result = create_input(input);
        cout << result;
        if (result)
            l_c_1++;
        else l_c_0++;
        cout << '\n';
    }
}
void obtain_minterms(int counter, int& r,qm QM,string wff)
{
    if (counter < l)
    {
        mp[var[counter]] = 0;
        obtain_minterms(counter + 1,r,QM,wff);
        mp[var[counter]] = 1;
        obtain_minterms(counter + 1,r,QM,wff);
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
string get_dnf(string wff)
{
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
        obtain_minterms(0, it, q,wff);
        sort(minterms.begin(), minterms.end());
        /*for (int t = 0; t < minterms.size(); t++)
            cout << minterms[t] << ' ';
        cout << '\n';
        cout << '\n';*/
        //cout << "DA";
        if (minterms.empty())
        {
            return "(0)";
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
        return "";
    }
}string get_cnf(string wff)
{
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
        return "";
    }
}
void empty_all_containers()
{
    minterms.clear();
    mp.clear();
    var.clear();
    inorder.clear();
}
void mode_1()
{
    cout << "input wff:";
    cin >> input;
    input += "\\";
    Climb infix;
    try { 
        cout << "format?\n";
        int syntax = 1;
        cin >> syntax;
        prop_tree::set_format(syntax);
        if (syntax > 2)
        {
            throw("Error: invalid syntax");
        }
        else
        {
            prop_tree::set_print(1);
            prop_tree tree = infix.get_tree(input);
            prop_tree::set_print(0);
            //cout << '\n';
            if (syntax == 2)
                cout << "[" << tree << "]" << '\n';
            else cout << tree << '\n';
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
        cout << error << '\n';
    }
    empty_all_containers();
}
void mode_2()
{
    cout << "input wff:";
    cin >> input;
    input += "\\";
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
                    cout << name << ' ';
                    mp[name] = 0;
                }
            }
            i++;
        }
        l = var.size();
        int syntax = 1;
        prop_tree::set_format(syntax);
        cout << tree << '\n';
        create_table_vals(0);
        if (l_c_1 == (1 << l))
            cout << "wff is valid\n";
        else if (l_c_1)
            cout << "wff is satisfiable\n";
        else cout << "wff is unsatisfiable\n";
    }
    catch (const char* error)
    {
        cout << error << '\n';
    }
    empty_all_containers();
}
void mode_3()
{
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
    }
    empty_all_containers();
}
void mode_4()
{
    cout << "input wff:";
    cin >> input;
    cout << get_dnf(input)<<'\n';
    empty_all_containers();
}
void mode_5()
{
    cout << "input wff:";
    cin >> input;
    //string second_input = "!(" + input+ ")";
    //cout << second_input << '\n';
    //second_input.push_back(')');
    //second_input.insert(second_input.begin(), '(');
    //empty_all_containers();
    cout<<"dnf: " << get_dnf(input) << '\n';
    //cout << "succes" << '\n';
   // name.clear();
    empty_all_containers();
    cout <<"cnf: " << get_cnf(input)<<'\n';
    empty_all_containers();
}
int main(int argc, char** argv)
{
    mode = 1;
    time_t time = chrono::system_clock::to_time_t(chrono::system_clock::now());
    char current_time[26];
    ctime_s(current_time, sizeof(current_time), &time);
    g <<line_i<<' ' << "program started at " << current_time << '\n';
    line_i++;
    while (mode)
    {
        cout << "mode:";
        cin >> mode;
        g<<"line "<<line_i << ": " << mode << '\n';
        //if (mode > 4 && mode<0)
        //    cout<<"Error: invalid mode",mode=0;
        if (mode<=5 && mode>=1)
        {
            //auto tt = chrono::system_clock::now();
            //g<<"program started at " << asctime_s(tt) << '\n';
            //empty_all_containers();
            if (mode == 2)
            {
                mode_2();
            }
            else if (mode==1)
            {
                mode_1();
            }
            else if (mode == 3)
            {
                mode_3();
            }
            else if (mode == 4)
            {
                mode_4();
            }
            else if (mode == 5)
            {
                mode_5();
            }
        }
        line_i++;
    }
    return 0;
}

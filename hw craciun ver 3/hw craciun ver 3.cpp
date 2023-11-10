#include "infix.h"
#include "Quine_McClusky.h"
#include <iostream>
#include <string>
#include <map>
#include <algorithm>
#include <vector>
using namespace std;
string input;
vector <string> var;
vector <int> p2;
vector <string> minterms;
map <string, int> mp;
int l,mode,l_c_1,l_c_0,n;
int create_input()
{
    string s = "";
    int i = 0;
    while (i < input.size())
    {
        string name = "";
        if (input[i]!='\\' && input[i] != '0' && input[i] != '1' && input[i] != '!' && input[i] != '|' && input[i] != '&' && input[i] != '(' && input[i] != ')' && input[i] != '#' && input[i] != '$')
        {       while (i<input.size()&&input[i]!='\\' && input[i] != '0' && input[i] != '1' && input[i] != '!' && input[i] != '|' && input[i] != '&' && input[i] != '(' && input[i] != ')' && input[i] != '#' && input[i] != '$')//((input[i] >= 'a' && input[i] <= 'z') || (input[i] >= 'A' && input[i] <= 'Z'))
            {
                name.push_back(input[i]);
                i++;
            }
            s.push_back(mp[name] + '0');
        }
        else {
            s.push_back(input[i]);
            i++;
        }
    }
    Climb infix;
    prop_tree tree = infix.get_tree(s);
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
        int result = create_input();
        cout << result;
        if (result)
            l_c_1++;
        else l_c_0++;
        cout << '\n';
    }
}
void obtain_minterms(int counter, int& r,qm QM)
{
    if (counter < l)
    {
        mp[var[counter]] = 0;
        obtain_minterms(counter + 1,r,QM);
        mp[var[counter]] = 1;
        obtain_minterms(counter + 1,r,QM);
    }
    else
    {
        int result = create_input();
        if (result)
            minterms.push_back(QM.pad(QM.dec_to_bin(r)));
        r++;
    }
}
int main(int argc, char** argv)
{
    mode = 1;
    for (int i = 1; i < 31; i++)
    {
        p2.push_back((1 << i));
    }
    while (mode)
    {
        cout << "mode:";
        cin >> mode;
        if (mode > 3 && mode<0)
            cout<<"Error: invalid mode",mode=0;
        if (mode<=4 && mode>=1)
        {
            minterms.clear();
            mp.clear();
            var.clear();
            if (mode==4 && mode <= 4 && mode >= 1)
            {
                cout << "input wff:";
                cin >> input;
                input += "\\";
            }

            if (mode == 2)
            {
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
            }
            else if (mode==1)
            {
                Climb infix;
                try {
                    prop_tree tree = infix.get_tree(input);
                    cout << "format?\ninput 1 for wff standard\ninput 2 for set representation\n";
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
            else if (mode == 3)
            {
                cout << "number of variables:";
                cin >> n;
                qm q(n);
                int a1=0;
                string input="";
                cout << "values truth function:\n";
                
                for (int i = 0; i < (1 << n); i++)
                {
                    int inp;
                    cout << i<<":";
                    cin >> inp;
                    
                    if (inp)
                    {
                        minterms.push_back(q.pad(q.dec_to_bin(i)));
                    }
                }
                sort(minterms.begin(), minterms.end());
                do
                {
                    minterms = q.reduce(minterms);
                    sort(minterms.begin(), minterms.end());
                } while (!q.vec_equal(minterms,q.reduce(minterms)));
                int it = 0;
                string before_wff = "";
                for (it = 0; it < minterms.size() - 1; it++)
                {
                    before_wff = before_wff + q.get_value(minterms[it]) + '|';
                }
                before_wff = before_wff + q.get_value(minterms[it]);
                before_wff = before_wff + '\\';
                Climb infix;
                try {
                    prop_tree tree = infix.get_tree(before_wff);
                    cout << "format?\ninput 1 for wff standard\ninput 2 for set representation\n";
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
            else if (mode == 4)
            {
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
                                mp[name] = 0;
                            }
                        }
                        i++;
                    }
                    l = var.size();
                    qm q(var.size());
                    int it = 0;
                    obtain_minterms(0,it,q);
                    sort(minterms.begin(), minterms.end());
                    //cout << "DA";
                    do
                    {
                        minterms = q.reduce(minterms);
                        sort(minterms.begin(), minterms.end());
                    } while (!q.vec_equal(minterms, q.reduce(minterms)));
                    it = 0;
                    string before_wff = "";
                    for (it = 0; it < minterms.size() - 1; it++)
                    {
                        before_wff = before_wff + q.get_value_custom(minterms[it], var)+'|';
                    }
                    before_wff = before_wff + q.get_value_custom(minterms[it], var);
                    before_wff = before_wff + '\\';
                    Climb infixx;
                    try {
                        prop_tree treee = infixx.get_tree(before_wff);
                        cout << "format?\ninput 1 for wff standard\ninput 2 for set representation\n";
                        int syntax = 1;
                        cin >> syntax;
                        if (syntax > 2)
                        {
                            throw("Error: invalid syntax");
                        }
                        prop_tree::set_format(syntax);
                        if (syntax == 2)
                            cout << '[' << treee << ']' << '\n';
                        else
                            cout << treee << '\n';
                    }
                    catch (const char* error)
                    {
                        cout << error << '\n';
                    }
                }
                catch (const char* error)
                {
                    cout << error << '\n';
                }
            }
        }
    }
    return 0;
}

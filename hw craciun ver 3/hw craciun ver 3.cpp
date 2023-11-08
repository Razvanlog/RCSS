#include "infix.h"
#include <iostream>
#include <string>
#include <map>
#include <vector>
using namespace std;
string input;
vector <string> var;
map <string, int> mp;
int l,mode,l_c_1,l_c_0;
int create_input()
{
    string s = "";
    int i = 0;
    while (i < input.size())//for (int i = 0; i < input.size(); i++)
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
    //cout << s << '\n';
    prop_tree tree = infix.get_tree(s);
    //cout << tree<<'\n';
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
int main(int argc, char** argv)
{
    mode = 1;
    while (mode)
    {
        cout << "mode:";
        cin >> mode;
        if (mode > 2)
            cout<<"Error: invalid mode",mode=0;
        if (mode<=2 && mode>=1)
        {
            mp.clear();
            var.clear();
            cout << "input wff:";
            cin >> input;
            //cout << input.size()<<'\n';
            input += "\\";
            //cout << input;
            //cout << '\n';
            //cout << input.size() << '\n';
            if (mode == 2)
            {
                Climb infix;
                try {
                    prop_tree tree = infix.get_tree(input);
                    int i = 0;
                    //for (int i = 0; i < input.size(); i++)
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
            else
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
        }
    }
    return 0;
}

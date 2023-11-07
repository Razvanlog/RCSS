#include "infix.h"
#include <iostream>
#include <string>
#include <map>
#include <vector>
using namespace std;
string input;
vector <char> var;
map <char, int> mp;
int l,mode,l_c_1,l_c_0;
int create_input()
{
    string s = "";
    for (int i = 0; i < input.size(); i++)
    {
        if ((input[i] >= 'a' && input[i] <= 'z') || (input[i] >= 'A' && input[i] <= 'Z'))
        {
            s.push_back(mp[input[i]] + '0');
        }
        else s.push_back(input[i]);
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
            cout << mp[var[i]] << ' ';
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
    cout << "mode:";
    cin >> mode;
    cout << "input wff:";
    cin >> input;
    if (mode == 2)
    {
        Climb infix;
        try {
            prop_tree tree = infix.get_tree(input);
            for (int i = 0; i < input.size(); i++)
            {
                if ((input[i] >= 'a' && input[i] <= 'z') || (input[i] >= 'A' && input[i] <= 'Z'))
                {
                    if (mp.find(input[i]) == mp.end())
                    {
                        var.push_back(input[i]);
                        cout << input[i] << ' ';
                        mp[input[i]] = 0;
                    }
                }
            }
            l = var.size();
            int syntax=1;
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
            int syntax=1;
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
    return 0;
}

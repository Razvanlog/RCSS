#ifndef INFIX_H_INCLUDED
#define INFIX_H_INCLUDED
#include "proptree.h"
#include <string>
#include <sstream>
class Climb : public prop_tree {
public:
    prop_tree get_tree(const std::string& expression)
    {
        infix.str(expression);
        return parse_climb(1);
    }
private:
    std::istringstream infix;
    prop_tree parse_climb(int);
    prop_tree parse_primary();
    char look_ahead()
    {
        char c; infix >> c;
        infix.putback(c);
        return c;
    }
};
inline int priority(char op)
{
    switch (op) {
    case '!': return 5;
    case '&': return 4;
    case '|': return 3;
    case '$': return 2;
    case '#': return 1;
    }
    return 0;
}
inline bool right_ass(char op) { return op == '!'; }

#endif // INFIX_H_INCLUDED

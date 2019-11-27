#ifndef __DEFS_HH__
#define __DEFS_HH__

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <utility>

template<typename T>
using sptr = std::shared_ptr<T>;

#include "utils.hh"
#include "location.hh"


class Node {
public:
    using loc_type = yy::location;
    Node(const loc_type& loc) : _loc(loc) {}

    void setLocation(const loc_type& loc) {
        _loc = loc;
    }

    void printLoc(std::ostream& os) const {
        os << " (" << _loc.begin.line << ',' << _loc.begin.column
           << '-' << _loc.end.line << ',' << _loc.end.column << ")\n";
    }

    virtual void print(std::ostream& os, int level) const = 0;
    virtual ~Node() {}
    friend std::ostream& operator<<(std::ostream& os, const Node& node) {
        node.print(os, 0);
        return os;
    }
protected:
    loc_type _loc;
};


class Expr : public Node {
public:
    Expr(const loc_type& loc = loc_type()) : Node(loc) {}
};

class Identifier;
class Lvalue;

class Decl : public Node {
public:
    Decl(const loc_type& loc = loc_type()) : Node(loc) {}
};

class Stmt : public Node {
public:
    Stmt(const loc_type& loc = loc_type()) : Node(loc) {}
};

class Type : public Node {
public:
    Type(const loc_type& loc) : Node(loc) {}
};

#endif // ! __DEFS_HH__

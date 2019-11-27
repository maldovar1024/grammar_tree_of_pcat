#ifndef __EXPRESSION_HH__
#define __EXPRESSION_HH__

#include <variant>
#include "ast/defs.hh"


class Constant : public Expr {
public:
    enum ConstType { INTEGER, REAL, STRING };
    Constant() = default;
    Constant(const loc_type& loc, const std::string& val, ConstType type)
        : Expr(loc), _val(val), _type(type) {}
    
    void print(std::ostream& os, int level) const override;
private:
    std::string _val;
    ConstType _type;
};


class Identifier : public Expr {
public:
    Identifier(const loc_type& loc, const std::string& name)
        : Expr(loc), _name(name) {}

    const std::string& getName() const {
        return _name;
    }

    void print(std::ostream& os, int level) const override;
private:
    std::string _name;
};

class Lvalue : public Expr {
public:
    Lvalue(const loc_type& loc) : Expr(loc) {}
};

class IdAccess : public Lvalue {
    sptr<Identifier> _id;
public:
    IdAccess(const loc_type& loc, sptr<Identifier> id)
        : Lvalue(loc), _id(id) {}
    
    void print(std::ostream& os, int level) const override;
};

class RecordAccess : public Lvalue {
    sptr<Lvalue> _target;
    sptr<Identifier> _member;
public:
    RecordAccess(const loc_type& loc, sptr<Lvalue> target, sptr<Identifier> member)
        : Lvalue(loc), _target(target), _member(member) {}

    void print(std::ostream& os, int level) const override;
};

class ArrayAccess : public Lvalue {
    sptr<Lvalue> _target;
    sptr<Expr> _idx;
public:
    ArrayAccess(const loc_type& loc, sptr<Lvalue> target, sptr<Expr> idx)
        : Lvalue(loc), _target(target), _idx(idx) {}

    void print(std::ostream& os, int level) const override;
};

class Operation : public Expr {
public:
    enum Operator {
        POS, NEG, NOT,
        PLUS, MINUS, TIMES, DIVIDE, DIV, MOD, AND, OR,
        GT, LT, GE, LE, EQ, NE 
    };
    static const std::string opName[];
    
    Operation(const loc_type& loc, Operator op) : Expr(loc), _op(op) {}
protected:
    Operator _op;
};

class Unary : public Operation {
    sptr<Expr> _expr;
public:
    Unary(const loc_type& loc, Operator op, sptr<Expr> expr)
        : Operation(loc, op), _expr(expr) {}
    
    void print(std::ostream& os, int level) const override;
};

class Binary : public Operation {
    sptr<Expr> _expr1, _expr2;
public:
    Binary(const loc_type& loc, sptr<Expr> expr1, Operator op, sptr<Expr> expr2)
        : Operation(loc, op), _expr1(expr1), _expr2(expr2) {}

    void print(std::ostream& os, int level) const override;
};

class ProcCall : public Expr {
    sptr<Identifier> _name;
    std::vector<sptr<Expr>> _args;
public:
    ProcCall(const loc_type& loc, sptr<Identifier> name, std::vector<sptr<Expr>>& args)
        : Expr(loc), _name(name), _args(std::move(args)) {}

    void print(std::ostream& os, int level) const override;
};

class CompValue : public Expr {
public:
    using Component = std::pair<sptr<Identifier>, sptr<Expr>>;
    CompValue(const loc_type& loc, sptr<Identifier> id,
        std::vector<Component>& values)
        : Expr(loc), _id(id), _values(std::move(values)) {}

    void print(std::ostream& os, int level) const override;
private:
    sptr<Identifier> _id;
    std::vector<Component> _values;
};

class ArrayValue : public Expr {
public:
    using ArrayComp = std::pair<sptr<Expr>, sptr<Expr>>;
    ArrayValue(const loc_type& loc, sptr<Identifier> id,
        std::vector<ArrayComp>& values)
        : Expr(loc), _id(id), _values(std::move(values)) {}
    
    void print(std::ostream& os, int level) const override;
private:
    sptr<Identifier> _id;
    std::vector<ArrayComp> _values;
};

#endif // ! __EXPRESSION_HH__

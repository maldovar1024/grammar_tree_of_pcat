#ifndef __STATEMENT_HH__
#define __STATEMENT_HH__

#include <variant>
#include "ast/defs.hh"


class StmtList : public Stmt {
    std::vector<sptr<Stmt>> _stmts;
public:
    StmtList(const loc_type& loc = loc_type()) : Stmt(loc) {}
    void addStmt(sptr<Stmt> stmt) {
        _stmts.push_back(stmt);
    }

    void print(std::ostream& os, int level) const override;
};

class Constant;

class Write : public Stmt {
public:
    using WriteExpr = std::variant<sptr<Expr>, sptr<Constant>>;

    Write(const loc_type& loc, std::vector<WriteExpr>& args)
        : Stmt(loc), _writeExprs(std::move(args)) {}

    void print(std::ostream& os, int level) const override;
private:
    std::vector<WriteExpr> _writeExprs;
};

std::ostream& operator<<(std::ostream& os, const Write::WriteExpr& wexp);

class Read : public Stmt {
    std::vector<sptr<Lvalue>> _args;
public:
    Read(const loc_type& loc, std::vector<sptr<Lvalue>>& args)
        : Stmt(loc), _args(std::move(args)) {}

    void print(std::ostream& os, int level) const override;
};

class Assign : public Stmt {
    sptr<Lvalue> _lvalue;
    sptr<Expr> _expr;
public:
    Assign(const loc_type& loc, sptr<Lvalue> lvalue, sptr<Expr> expr)
        : Stmt(loc), _lvalue(lvalue), _expr(expr) {}

    void print(std::ostream& os, int level) const override;
};

class If : public Stmt {
public:
    using IfComponent = std::pair<sptr<Expr>, sptr<StmtList>>;

    If(const loc_type& loc, const IfComponent& if_part, sptr<StmtList> else_part = nullptr)
        : Stmt(loc), _if(if_part),  _else(else_part) {}

    If(const loc_type& loc, const IfComponent& if_part, std::vector<IfComponent>& elsif_part,
        sptr<StmtList> else_part = nullptr)
        : Stmt(loc), _if(if_part), _elsif(std::move(elsif_part)), _else(else_part) {}

    void print(std::ostream& os, int level) const override;
private:
    IfComponent _if;
    std::vector<IfComponent> _elsif;
    sptr<StmtList> _else;
};

class While : public Stmt {
    sptr<Expr> _condition;
    sptr<StmtList> _stmts;
public:
    While(const loc_type& loc, sptr<Expr> expr, sptr<StmtList> stmts)
        : Stmt(loc), _condition(expr), _stmts(stmts) { }

    void print(std::ostream& os, int level) const override;
};

class For : public Stmt {
    sptr<Identifier> _id;
    sptr<Expr> _expr1, _expr2, _expr3;
    sptr<StmtList> _stmts;
public:
    For(const loc_type& loc, sptr<Identifier> id, sptr<Expr> expr1,
        sptr<Expr> expr2, sptr<Expr> expr3, sptr<StmtList> stmts)
        : Stmt(loc), _id(id), _expr1(expr1), _expr2(expr2),
            _expr3(expr3), _stmts(stmts) {}

    void print(std::ostream& os, int level) const override;
};

class Loop : public Stmt {
    sptr<StmtList> _stmts;
public:
    Loop(const loc_type& loc, sptr<StmtList> stmts)
        : Stmt(loc), _stmts(stmts) {}

    void print(std::ostream& os, int level) const override;
};

class Exit : public Stmt {
public:
    Exit(const loc_type& loc) : Stmt(loc) {}
    void print(std::ostream& os, int level) const;
};

class ProcCall;

class ProcCallStmt : public Stmt {
    sptr<ProcCall> _procedure;
public:
    ProcCallStmt(const loc_type& loc, sptr<ProcCall> procedure)
        : Stmt(loc), _procedure(procedure) {}

    void print(std::ostream& os, int level) const override;
};

class Return : public Stmt {
    sptr<Expr> _expr;
public:
    Return(const loc_type& loc, sptr<Expr> expr = nullptr) : Stmt(loc), _expr(expr) {}

    void print(std::ostream& os, int level) const override;
};

#endif // ! __STATEMENT_HH__

#include "ast/statement.hh"
#include "ast/expression.hh"


void StmtList::print(std::ostream& os, int level) const {
    os << blank(level) << "statements";
    printLoc(os);
    for (auto& stmt : _stmts) {
        stmt->print(os, level + 2);
    }
}

void Write::print(std::ostream& os, int level) const {
    os << blank(level) << "write statement";
    printLoc(os);

    level += 2;
    if (_writeExprs.size() == 0) {
        os << blank(level) << "no args\n";
    }
    else {
        for (auto& arg : _writeExprs) {
            switch (arg.index()) {
            case 0:
                os << blank(level) << "expression: \n";
                std::get<sptr<Expr>>(arg)->print(os, level + 2);
                break;
            case 1:
                std::get<sptr<Constant>>(arg)->print(os, level + 2);
                break;
            }
        }
    }
}

std::ostream& operator<<(std::ostream& os, const Write::WriteExpr& wexp) {
    switch (wexp.index()) {
    case 0: os << *std::get<sptr<Expr>>(wexp); break;
    case 1: os << *std::get<sptr<Constant>>(wexp); break;
    }
    return os;
}

void Read::print(std::ostream& os, int level) const {
    os << blank(level) << "read statement";
    printLoc(os);
    for (auto& arg : _args) {
        arg->print(os, level + 2);
    }
}

void Assign::print(std::ostream& os, int level) const {
    os << blank(level) << "assignment statement";
    printLoc(os);
    level += 2;
    _lvalue->print(os, level);
    os << blank(level) << "expression\n";
    _expr->print(os, level + 2);
}

void If::print(std::ostream& os, int level) const {
    os << blank(level) << "if-statement";
    printLoc(os);

    blank nBlank1(level + 2), nBlank2(level + 4);
    level += 6;
    os << nBlank1 << "if-part\n" << nBlank2 << "condition\n";
    _if.first->print(os, level);
    _if.second->print(os, level - 2);
    for (auto& [cond, stmts] : _elsif) {
        os << nBlank1 << "elsif-part\n" << nBlank2 << "condition\n";
        cond->print(os, level);
        stmts->print(os, level - 2);
    }
    if (_else != nullptr) {
        os << nBlank1 << "else-part\n";
        _else->print(os, level - 2);
    }
}

void While::print(std::ostream& os, int level) const {
    os << blank(level) << "while statement";
    printLoc(os);
    level += 2;
    os << blank(level) << "condition\n";
    _condition->print(os, level + 2);
    _stmts->print(os, level);
}

void For::print(std::ostream& os, int level) const {
    os << blank(level) << "for statement";
    printLoc(os);

    blank nBlank(level + 2);
    level += 4;
    os << nBlank << "identifier\n";
    _id->print(os, level);
    os << nBlank << "expression1\n";
    _expr1->print(os, level);
    os << nBlank << "expression2\n";
    _expr2->print(os, level);
    if (_expr3 != nullptr) {
        os << nBlank << "expression3\n";
        _expr3->print(os, level);
    }
    _stmts->print(os, level - 2);
}

void Loop::print(std::ostream& os, int level) const {
    os << blank(level) << "loop-statement";
    printLoc(os);
    _stmts->print(os, level + 2);
}

void Exit::print(std::ostream& os, int level) const {
    os << blank(level) << "exit-statement";
    printLoc(os);
}

void ProcCallStmt::print(std::ostream& os, int level) const {
    _procedure->print(os, level);
}

void Return::print(std::ostream& os, int level) const {
    os << blank(level) << "return-statement";
    printLoc(os);
    if (_expr != nullptr) {
        os << blank(level + 2) << "expression\n";
        _expr->print(os, level + 4);
    }
}

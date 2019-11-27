#include "ast/expression.hh"


void Constant::print(std::ostream& os, int level) const {
    static const std::string type_names[] = {"INTEGER: ", "REAL: ", "string: "};
    os << blank(level) << type_names[_type] << _val;
    printLoc(os);
}

void Identifier::print(std::ostream& os, int level) const {
    os << blank(level) << "identifier: " << _name;
    printLoc(os);
}

void IdAccess::print(std::ostream& os, int level) const {
    os << blank(level) << "l-value: id-access";
    printLoc(os);
    _id->print(os, level + 2);
}

void RecordAccess::print(std::ostream& os, int level) const {
    os << blank(level) << "l-value: record-access";
    printLoc(os);
    os << blank(level + 2) << "record\n";
    _target->print(os, level + 4);
    os << blank(level + 2) << "member\n";
    _member->print(os, level + 4);
}

void ArrayAccess::print(std::ostream& os, int level) const {
    os << blank(level) << "l-value: array-access";
    printLoc(os);
    os << blank(level + 2) << "array\n";
    _target->print(os, level + 4);
    os << blank(level + 2) << "index\n";
    _idx->print(os, level + 4);
}

const std::string Operation::opName[] =  {
    "+", "-", "NOT",
    "+", "-", "*", "/", "DIV", "MOD", "AND", "OR",
    ">", "<", ">=", "<=", "=", "<>"
};

void Unary::print(std::ostream& os, int level) const {
    os << blank(level) << "unary-op";
    printLoc(os);
    level += 2;
    os << blank(level) << "op: " << Operation::opName[_op] << '\n';
    _expr->print(os, level);
}

void Binary::print(std::ostream& os, int level) const {
    os << blank(level) << "binary-op";
    printLoc(os);
    level += 2;
    _expr1->print(os, level);
    os << blank(level) << "op: " << Operation::opName[_op] << '\n';
    _expr2->print(os, level);
}

void ProcCall::print(std::ostream& os, int level) const {
    os << blank(level) << "procedure-call";
    printLoc(os);
    
    blank nBlank(level + 2);
    level += 4;
    os << nBlank << "procedure name\n";
    _name->print(os, level);
    if (_args.size() != 0) {
        os << nBlank << "arguments\n";
        for (auto arg : _args) {
            arg->print(os, level);
        }
    }
    else {
        os << nBlank << "no arguments\n";
    }
}

void CompValue::print(std::ostream& os, int level) const {
    os << blank(level) << "comp-construction";
    printLoc(os);

    level += 2;
    blank nBlank(level);
    _id->print(os, level);
    
    level += 2;
    for (auto& [id, expr] : _values) {
        os << nBlank << "component\n";
        id->print(os, level);
        expr->print(os, level);
    }
}

void ArrayValue::print(std::ostream& os, int level) const {
    os << blank(level) << "array-construction";
    printLoc(os);

    level += 2;
    blank nBlank(level);
    _id->print(os, level);
    
    level += 2;
    for (auto& [expr1, expr2] : _values) {
        os << nBlank << "array-value\n";
        expr1->print(os, level);
        if (expr2 != nullptr)
            expr2->print(os, level);
    }
}

#include "ast/declaration.hh"
#include "ast/expression.hh"
#include "ast/body.hh"


void DeclList::print(std::ostream& os, int level) const {
    if (_decls.size() != 0) {
        os << blank(level) << "declarations";
        printLoc(os);
        for (auto& decl : _decls) {
            decl->print(os, level + 2);
        }
    }
}

void VarDecl::print(std::ostream& os, int level) const {
    os << blank(level) << "var-decl";
    printLoc(os);

    level += 2;
    for (auto id : _ids) {
        id->print(os, level);
    }
   
    if (_type != nullptr) {
        _type->print(os, level);
    }

    os << blank(level) << "init value\n";
    if (_initVal != nullptr)
        _initVal->print(os, level + 2);
    else
        os << "no init value\n";
}

ProcDecl::ProcDecl(const loc_type& loc, sptr<Identifier> name, std::vector<FPSection>& params,
    sptr<Type> retType, sptr<Body> body)
    : Decl(loc), _name(name), _retType(retType), _body(body) {
    for (auto& [param, type] : params) {
        _params.insert(_params.end(), param.begin(), param.end());
        _params.push_back(type);
    }
    params.clear();
}

void ProcDecl::print(std::ostream& os, int level) const {
    os << blank(level) << "procedure-decl";
    printLoc(os);

    blank nBlank(level + 2);
    level += 4;
    
    os << nBlank << "procedure name\n";
    _name->print(os, level);

    if (_params.size() != 0) {
        os << nBlank << "parameters\n";
        for (auto& param : _params) {
            switch (param.index()) {
            case 0: std::get<sptr<Identifier>>(param)->print(os, level); break;
            case 1: std::get<sptr<Type>>(param)->print(os, level); break;
            }
        }
    }
    else {
        os << nBlank << "no parameters\n";
    }

    if (_retType != nullptr) {
        os << nBlank << "return-type\n";
        _retType->print(os, level);
    }
    
    os << nBlank << "procedure ";
    _body->print(os, level);
}

void TypeDecl::print(std::ostream& os, int level) const {
    os << blank(level) << "type-decl";
    printLoc(os);
    level += 2;
    os << blank(level) << "type name\n";
    _id->print(os, level + 2);
    _type->print(os, level);
}

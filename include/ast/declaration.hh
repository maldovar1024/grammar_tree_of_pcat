#ifndef __DECLARATION_HH__
#define __DECLARATION_HH__

#include <variant>
#include "ast/defs.hh"


class DeclList : public Decl {
    std::vector<sptr<Decl>> _decls;
public:
    DeclList(const loc_type& loc = loc_type()) : Decl(loc) {}

    void addDecl(sptr<Decl> decl) {
        _decls.push_back(decl);
    }

    void addDeclList(sptr<DeclList> decls) {
        _decls.insert(_decls.end(), decls->_decls.begin(), decls->_decls.end());
    }

    void print(std::ostream& os, int level) const override;
};

class VarDecl : public Decl {
    std::vector<sptr<Identifier>> _ids;
    sptr<Type> _type;
    sptr<Expr> _initVal;
public:
    VarDecl(const loc_type& loc, std::vector<sptr<Identifier>>& ids,
        sptr<Type> type, sptr<Expr> initVal)
        : Decl(loc), _ids(std::move(ids)), _type(type), _initVal(initVal) { }

    void print(std::ostream& os, int level) const override;
};

class Body;

class ProcDecl : public Decl {
    sptr<Identifier> _name;
    std::vector<std::variant<sptr<Identifier>, sptr<Type>>> _params;
    sptr<Type> _retType;
    sptr<Body> _body;
public:
    using FPSection = std::pair<
        std::vector<sptr<Identifier>>,
        sptr<Type>
    >;

    ProcDecl(const loc_type& loc, sptr<Identifier> name, std::vector<FPSection>& params,
        sptr<Type> retType, sptr<Body> body);

    void print(std::ostream& os, int level) const override;
};

class TypeDecl : public Decl {
    sptr<Identifier> _id;
    sptr<Type> _type;
public:
    TypeDecl(const loc_type& loc, sptr<Identifier> id, sptr<Type> type)
        : Decl(loc), _id(id), _type(type) {}
    
    void print(std::ostream& os, int level) const override;
};

#endif // ! __DECLARATION_HH__

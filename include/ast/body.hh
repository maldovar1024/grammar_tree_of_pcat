#ifndef __ROOT_HH__
#define __ROOT_HH__

#include "ast/declaration.hh"
#include "ast/statement.hh"


class Body : public Node {
    sptr<DeclList> _declList;
    sptr<StmtList> _stmtList;
public:
    Body(const yy::location& loc, sptr<DeclList> declList, sptr<StmtList> stmtList)
        : Node(loc), _declList(declList), _stmtList(stmtList) {}

    void print(std::ostream& os, int level = 0) const override {
        os << "body";
        printLoc(os);
        _declList->print(os, level);
        _stmtList->print(os, level);
    }
};

#endif // ! __ROOT_HH__

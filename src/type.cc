#include "ast/type.hh"
#include "ast/expression.hh"


void TypeId::print(std::ostream& os, int level) const {
    os << blank(level) << "type";
    printLoc(os);
    _id->print(os, level + 2);
}


void ArrayType::print(std::ostream& os, int level) const {
    os << "array-type";
    printLoc(os);
    _baseType->print(os, level + 2);
}

void RecordType::print(std::ostream& os, int level) const {
    os << blank(level) << "record-component\n";
    level += 2;
    for (auto& [id, type] : _components) {
        id->print(os, level);
        type->print(os, level);
    }
}
